distribuido: 
	gcc distributed_server/src/main.c distributed_server/inc/bme/bme280.c distributed_server/src/modules/i2c/i2c_module.c distributed_server/src/modules/gpio/gpio_module.c -I distributed_server/inc/bme -lpthread -lwiringPi -lbcm2835 -o distribuido

central: 
	gcc ./central_server/src/modules/main.c ./central_server/src/modules/cJSON.c -lpthread -o central

