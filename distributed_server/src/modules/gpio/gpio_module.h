#define ON 1
#define OFF 0

#define ATIVO 1 
#define INATIVO 0

// Configuring pins 

#define LAMP_1_GPIO_17_COZINHA RPI_V2_GPIO_P1_11 // PIN 11
#define LAMP_2_GPIO_18_SALA RPI_V2_GPIO_P1_12 // PIN 12
#define LAMP_3_GPIO_27_QUARTO_1 RPI_V2_GPIO_P1_13 // PIN 13
#define LAMP_4_GPIO_22_QUARTO_2 RPI_V2_GPIO_P1_15 // PIN 15

#define AIR_1_GPIO_23_QUARTO_1 RPI_V2_GPIO_P1_16 // PIN 16
#define AIR_2_GPIO_24_QUARTO_2 RPI_V2_GPIO_P1_18 // PIN 18

#define SENSOR_PRE_1_GPIO_25_SALA RPI_V2_GPIO_P1_22 // PIN 22
#define SENSOR_PRE_2_GPIO_26_SALA  RPI_V2_GPIO_P1_37 // PIN 37

#define SENSOR_ABER_1_GPIO_5_PORTA_COZINHA RPI_V2_GPIO_P1_29 // PIN 29
#define SENSOR_ABER_2_GPIO_6_JANELA_COZINHA RPI_V2_GPIO_P1_31 // PIN 31
#define SENSOR_ABER_3_GPIO_12_PORTA_SALA RPI_V2_GPIO_P1_32 // PIN 32
#define SENSOR_ABER_4_GPIO_16_JANELA_SALA RPI_V2_GPIO_P1_36 // PIN 36
#define SENSOR_ABER_5_GPIO_20_JANELA_QUARTO_1 RPI_V2_GPIO_P1_38 // PIN 38
#define SENSOR_ABER_6_GPIO_21_JANELA_QUARTO_2 RPI_V2_GPIO_P1_40 // PIN 40

void *control_sensors(void *params);
void *control_aparelhos(void *params);

void turn_lamp_cozinha(int);
void turn_lamp_sala(int);
void turn_lamp_quarto_1(int);
void turn_lamp_quarto_2(int);

void turn_air_quarto_1(int);
void turn_air_quarto_2(int);

int get_sensor_pre_1_sala();
int get_sensor_pre_2_sala();

int get_sensor_aber_porta_cozinha();
int get_sensor_aber_janela_cozinha();
int get_sensor_aber_porta_sala();
int get_sensor_aber_janela_sala();
int get_sensor_aber_janela_quarto_1();
int get_sensor_aber_janela_quarto_2();