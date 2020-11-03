#include "../../../inc/bme/bme280_defs.h"
#include "../../main.h"

#define I2C_DEVICE "/dev/i2c-1"

struct identifier {
    uint8_t dev_addr; // device address
    int8_t fd; // file descriptor 
};

// Function for reading the sensor's registers through I2C bus.
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

// Function for writing the sensor's registers through I2C bus.
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

// Function reads temperature, humidity and pressure data in forced mode.
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct I2cValue *i2c_values);
void *temperature_humidity_i2c(void *params);