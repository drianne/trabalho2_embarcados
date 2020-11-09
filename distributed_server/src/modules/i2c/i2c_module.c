#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

#include "bme280.h"
#include "i2c_module.h"

void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct I2cValue *i2c_values);

void *temperature_humidity_i2c(void *temps){
    struct I2cValue *i2c_values = temps;
    struct bme280_dev dev;
    struct identifier id;

    int8_t result = BME280_OK;
    
    if ((id.fd = open(I2C_DEVICE, O_RDWR)) < 0)
    {
        fprintf(stderr, "Failed to open the i2c bus %s\n", I2C_DEVICE);
        exit(1);
    }
    
    id.dev_addr = BME280_I2C_ADDR_PRIM; //i2c Address
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev.intf_ptr = &id;
    result = bme280_init(&dev);

    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", result);
        exit(1);
    }

    result = stream_sensor_data_forced_mode(&dev, i2c_values);

    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to stream sensor data (code %+d).\n", result);
        exit(1);
    }

    return 0;
}

// This function reading the sensor's registers through I2C bus.
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;
    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

// function provides the delay for required time (Microseconds) 
void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

// This function for writing the sensor's registers through I2C bus.
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}


// API reads sensor temperature and humidity data in forced mode.
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct I2cValue *i2c_values)
{    
    int8_t result = BME280_OK; // Variable to define the result 
    uint8_t settings_sel = 0; //  Define the selecting sensors 
    uint32_t req_delay; // Store minimum wait time between consecutive measurement in force mode

    struct bme280_data comp_data; // get temperature and humidity values

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    result = bme280_set_sensor_settings(settings_sel, dev); // Setting sensor settings

    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", result);
        return result;
    }

    req_delay = bme280_cal_meas_delay(&dev->settings); // Calculate minimum delay required between consecutive measurement 

    while(1){
        result = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);

        if (result != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor mode (code %+d).", result);
        }

        dev->delay_us(req_delay, dev->intf_ptr);

        result = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);

        if (result != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor data (code %+d).", result);
        }

        i2c_values->temperature = comp_data.temperature;
        i2c_values->humidity = comp_data.humidity;

        // printf("Temperatura: %f\n", i2c_values->temperature );
        // printf("Umidade: %f\n", i2c_values->humidity );
        sleep(1);
    }

    return result;
}
