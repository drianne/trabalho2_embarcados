#define COD_PEGA_DADO "1"
#define COD_ACIONA_LAMP_COZINHA "117"
#define COD_ACIONA_LAMP_SALA "218"
#define COD_ACIONA_LAMP_QUARTO_1 "327"
#define COD_ACIONA_LAMP_QUARTO_2 "422"
#define COD_ACIONA_AR_QUARTO_1 "123"
#define COD_ACIONA_AR_QUARTO_2 "224"

#define COD_DESLIGA_LAMP_COZINHA "711"
#define COD_DESLIGA_LAMP_SALA "812"
#define COD_DESLIGA_LAMP_QUARTO_1 "723"
#define COD_DESLIGA_LAMP_QUARTO_2 "224"
#define COD_DESLIGA_AR_QUARTO_1 "321"
#define COD_DESLIGA_AR_QUARTO_2 "422"

#define COD_ACIONA_ALARME "2"

struct I2cValue{
    double temperature;
    double humidity;
};

struct Sensores{
    int sensor_pre_1_gpio_25_sala;
    int sensor_pre_2_gpio_26_sala; 
    int sensor_aber_1_gpio_5_porta_cozinha;
    int sensor_aber_2_gpio_6_janela_cozinha; 
    int sensor_aber_3_gpio_12_porta_sala; 
    int sensor_aber_4_gpio_16_janela_sala; 
    int sensor_aber_5_gpio_20_janela_quarto_1; 
    int sensor_aber_6_gpio_21_janela_quarto_2; 
};

struct Aparelhos{
    int lamp_1_gpio_17_cozinha;
    int lamp_2_gpio_18_sala;
    int lamp_3_gpio_27_quarto_1;
    int lamp_4_gpio_22_quarto_2;
    int air_1_gpio_23_quarto_1;
    int air_2_gpio_24_quarto_2;
};

struct Dados{
    double temperature;
    double humidity;
    int sensor_pre_1_gpio_25_sala;
    int sensor_pre_2_gpio_26_sala; 
    int sensor_aber_1_gpio_5_porta_cozinha;
    int sensor_aber_2_gpio_6_janela_cozinha; 
    int sensor_aber_3_gpio_12_porta_sala; 
    int sensor_aber_4_gpio_16_janela_sala; 
    int sensor_aber_5_gpio_20_janela_quarto_1; 
    int sensor_aber_6_gpio_21_janela_quarto_2;
    int lamp_1_gpio_17_cozinha;
    int lamp_2_gpio_18_sala;
    int lamp_3_gpio_27_quarto_1;
    int lamp_4_gpio_22_quarto_2;
    int air_1_gpio_23_quarto_1;
    int air_2_gpio_24_quarto_2;
};

void get_data();
void aciona_aparelhos(int aparelho);