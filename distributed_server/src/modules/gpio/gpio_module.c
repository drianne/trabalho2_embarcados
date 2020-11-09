#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <unistd.h>
#include "../../main.h"
#include "gpio_module.h"

void settings_pins(){
    // Setting objects 
    bcm2835_gpio_fsel(LAMP_1_GPIO_17_COZINHA, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_2_GPIO_18_SALA, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_3_GPIO_27_QUARTO_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_4_GPIO_22_QUARTO_2, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_fsel(AIR_1_GPIO_23_QUARTO_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AIR_2_GPIO_24_QUARTO_2, BCM2835_GPIO_FSEL_OUTP);

    // Setting sensors
    bcm2835_gpio_fsel(SENSOR_PRE_1_GPIO_25_SALA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_PRE_2_GPIO_26_SALA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_1_GPIO_5_PORTA_COZINHA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_2_GPIO_6_JANELA_COZINHA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_3_GPIO_12_PORTA_SALA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_4_GPIO_16_JANELA_SALA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_5_GPIO_20_JANELA_QUARTO_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABER_6_GPIO_21_JANELA_QUARTO_2, BCM2835_GPIO_FSEL_INPT);
}

void turn_lamp_cozinha(int value){
    bcm2835_gpio_write(LAMP_1_GPIO_17_COZINHA, value & 1 );
}
void turn_lamp_sala(int value){
    bcm2835_gpio_write(LAMP_2_GPIO_18_SALA, value & 1 );
}
void turn_lamp_quarto_1(int value){
    bcm2835_gpio_write(LAMP_3_GPIO_27_QUARTO_1, value & 1 );
}
void turn_lamp_quarto_2(int value){
    bcm2835_gpio_write(LAMP_4_GPIO_22_QUARTO_2, value & 1 );
}

void turn_air_quarto_1(int value){
    bcm2835_gpio_write(AIR_1_GPIO_23_QUARTO_1, value & 1 );
}
void turn_air_quarto_2(int value){
    bcm2835_gpio_write(AIR_2_GPIO_24_QUARTO_2, value & 1 );
}

int get_sensor_pre_1_sala(){
   return bcm2835_gpio_lev(SENSOR_PRE_1_GPIO_25_SALA);
}
int get_sensor_pre_2_sala(){
    return bcm2835_gpio_lev(SENSOR_PRE_2_GPIO_26_SALA);
}

int get_sensor_aber_porta_cozinha(){
    return bcm2835_gpio_lev(SENSOR_ABER_1_GPIO_5_PORTA_COZINHA);
}
int get_sensor_aber_janela_cozinha(){
    return bcm2835_gpio_lev(SENSOR_ABER_2_GPIO_6_JANELA_COZINHA);
}
int get_sensor_aber_porta_sala(){
    return bcm2835_gpio_lev(SENSOR_ABER_3_GPIO_12_PORTA_SALA);
}
int get_sensor_aber_janela_sala(){
    return bcm2835_gpio_lev(SENSOR_ABER_4_GPIO_16_JANELA_SALA);
}
int get_sensor_aber_janela_quarto_1(){
    return bcm2835_gpio_lev(SENSOR_ABER_5_GPIO_20_JANELA_QUARTO_1);
}
int get_sensor_aber_janela_quarto_2(){
    return bcm2835_gpio_lev(SENSOR_ABER_6_GPIO_21_JANELA_QUARTO_2);
}

void init_gpio(){
    if (!bcm2835_init())
        exit(1);  
}

void *control_sensors(void *params){
    struct Sensores *sensores = params;   

    while(1){
        // printf("Setting sensors values");
        sensores->sensor_pre_1_gpio_25_sala = get_sensor_pre_1_sala();
        sensores->sensor_pre_2_gpio_26_sala = get_sensor_pre_2_sala();
        sensores->sensor_aber_1_gpio_5_porta_cozinha = get_sensor_aber_porta_cozinha();
        sensores->sensor_aber_2_gpio_6_janela_cozinha = get_sensor_aber_janela_cozinha();
        sensores->sensor_aber_3_gpio_12_porta_sala = get_sensor_aber_porta_sala();
        sensores->sensor_aber_4_gpio_16_janela_sala = get_sensor_aber_janela_sala();
        sensores->sensor_aber_5_gpio_20_janela_quarto_1 = get_sensor_aber_janela_quarto_1();
        sensores->sensor_aber_6_gpio_21_janela_quarto_2 = get_sensor_aber_janela_quarto_2();
        
        // printf("\n\nMostrando valores");
        // printf("\nPresença 1 SALA: %d", sensores->sensor_pre_1_gpio_25_sala);
        // printf("\nPresença 2 SALA: %d", sensores->sensor_pre_2_gpio_26_sala);
        // printf("\nAbertura Porta Cozinha: %d" , sensores->sensor_aber_1_gpio_5_porta_cozinha);
        // printf("\nAbertura Janela Cozinha: %d" , sensores->sensor_aber_2_gpio_6_janela_cozinha);
        // printf("\nAbertura Porta Sala: %d" , sensores->sensor_aber_3_gpio_12_porta_sala);
        // printf("\nAbertura Janela Sala: %d" , sensores->sensor_aber_4_gpio_16_janela_sala);
        // printf("\nAbertura Janela Quarto 1: %d" , sensores->sensor_aber_5_gpio_20_janela_quarto_1);
        // printf("\nAbertura Janela Quarto 2: %d\n\n" , sensores->sensor_aber_6_gpio_21_janela_quarto_2);
        fflush(stdout);
       
        sleep(1);
    }
}

void *control_aparelhos(void *params){
    struct Aparelhos *aparelhos = params;

    while(1){
        aparelhos->lamp_1_gpio_17_cozinha = get_sensor_pre_1_sala();
        aparelhos->lamp_2_gpio_18_sala = get_sensor_pre_2_sala();
        aparelhos->lamp_3_gpio_27_quarto_1 = get_sensor_aber_porta_cozinha();
        aparelhos->lamp_4_gpio_22_quarto_2 = get_sensor_aber_janela_cozinha();
        aparelhos->air_1_gpio_23_quarto_1 = get_sensor_aber_porta_sala();
        aparelhos->air_2_gpio_24_quarto_2 = get_sensor_aber_janela_sala();
        
        // printf("\n\nMostrando valores aparelhos");
        // printf("\nLâmpada da cozinha: %d",  aparelhos->lamp_1_gpio_17_cozinha);
        // printf("\nLâmpada da SALA: %d", aparelhos->lamp_2_gpio_18_sala);
        // printf("\nLâmpada quarto 1: %d" , aparelhos->lamp_3_gpio_27_quarto_1);
        // printf("\nLâmpada quarto 2: %d" , aparelhos->lamp_4_gpio_22_quarto_2);
        // printf("\nAr quarto 1: %d" , aparelhos->air_1_gpio_23_quarto_1);
        // printf("\nAr quarto 02: %d" ,  aparelhos->air_2_gpio_24_quarto_2);
        fflush(stdout);
       
        sleep(1);
    }
}