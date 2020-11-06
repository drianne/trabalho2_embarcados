#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./main.h"

#include "../../inc/json/cJSON.h"

#define PORT 10018
#define IP_DISTRIBUIDO "192.168.0.52"
#define IP_CENTRAL "192.168.0.53"

struct Dados aparelhos_dados;

int configuracaoCliente(){
  int socketCliente;
  struct sockaddr_in servidorAddr;

  if((socketCliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Falha no socker do Servidor\n");

  servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_DISTRIBUIDO);
	servidorAddr.sin_port = htons(PORT);
  
  if(connect(socketCliente, (struct sockaddr *) &servidorAddr, 
							sizeof(servidorAddr)) < 0)
		printf("Erro no connect()\n");

  return socketCliente;
}

void Cliente(char *mensagem){
  int socketCliente, servidorSocket, serverLength, tamanhoMensagem;
  struct sockaddr_in servidorAddr;
  int bytesRecebidos;
  int totalBytesRecebidos;
  char buffer[1024];

  socketCliente = configuracaoCliente();
  tamanhoMensagem = strlen(mensagem);

  if(send(socketCliente, mensagem, tamanhoMensagem, 0) != tamanhoMensagem)
		printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

  totalBytesRecebidos = 0;

	while(totalBytesRecebidos < tamanhoMensagem) {
		if((bytesRecebidos = recv(socketCliente, buffer, 1024-1, 0)) <= 0)
			printf("Não recebeu o total de bytes enviados\n");
		totalBytesRecebidos += bytesRecebidos;
		buffer[bytesRecebidos] = '\0';
		printf("%s\n", buffer);
	}
	close(socketCliente);

	return;
}

void *Servidor(void* arg){
    /*Buffer de entrada (armazena buffer do cliente)*/

	int status;
	const cJSON *cod = NULL;
	const cJSON *humidity = NULL;
	const cJSON *temperature = NULL;
	const cJSON *sensor_pre_1_gpio_25_sala = NULL;
	const cJSON *sensor_pre_2_gpio_26_sala = NULL;
	const cJSON *sensor_aber_1_gpio_5_porta_cozinha = NULL;
	const cJSON *sensor_aber_2_gpio_6_janela_cozinha = NULL;
	const cJSON *sensor_aber_3_gpio_12_porta_sala = NULL;
	const cJSON *sensor_aber_4_gpio_16_janela_sala = NULL;
	const cJSON *sensor_aber_5_gpio_20_janela_quarto_1 = NULL;
	const cJSON *sensor_aber_6_gpio_21_janela_quarto_2 = NULL;
	const cJSON *lamp_1_gpio_17_cozinha = NULL;
	const cJSON *lamp_2_gpio_18_sala = NULL;
	const cJSON *lamp_3_gpio_27_quarto_1 = NULL;
	const cJSON *lamp_4_gpio_22_quarto_2 = NULL; 
	const cJSON *air_1_gpio_23_quarto_1 = NULL;
	const cJSON *air_2_gpio_24_quarto_2 = NULL;
    char buffer_do_cliente[1024];
	int tamanhoRecebido;
    /*Cast do ponteiro*/
    int sockEntrada = *(int *) arg;	
    /*Loop "infinito"*/
    printf("Aguardando as mensagens...\n\n");

	if((tamanhoRecebido = recv(sockEntrada, buffer_do_cliente, 1024, 0)) < 0)
			printf("Erro no recv()\n");

    while (tamanhoRecebido > 0){
		if(send(sockEntrada, buffer_do_cliente, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");

		if((tamanhoRecebido = recv(sockEntrada, buffer_do_cliente, 1024, 0)) < 0)
			printf("Erro no recv()\n");
		
		cJSON *dados_json = cJSON_ParseWithLength(buffer_do_cliente, strlen(buffer_do_cliente));

		if (dados_json == NULL){
			const char *error_ptr = cJSON_GetErrorPtr();
			if (error_ptr != NULL)
			{
				fprintf(stderr, "Error before: %s\n", error_ptr);
			}
			status = 0;
		}
		
		cod = cJSON_GetObjectItemCaseSensitive(dados_json, "cod");

		if (cJSON_IsNumber(cod)){
			
			switch (cod->valueint){
				case 1:
					humidity = cJSON_GetObjectItemCaseSensitive(dados_json, "umidade");
					temperature = cJSON_GetObjectItemCaseSensitive(dados_json, "temperatura");
					sensor_pre_1_gpio_25_sala = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_pre_1_gpio_25_sala");
					sensor_pre_2_gpio_26_sala = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_pre_2_gpio_26_sala");
					sensor_aber_1_gpio_5_porta_cozinha = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_1_gpio_5_porta_cozinha");
					sensor_aber_2_gpio_6_janela_cozinha = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_2_gpio_6_janela_cozinha");
					sensor_aber_3_gpio_12_porta_sala = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_3_gpio_12_porta_sala");
					sensor_aber_4_gpio_16_janela_sala = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_4_gpio_16_janela_sala");
					sensor_aber_5_gpio_20_janela_quarto_1 = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_5_gpio_20_janela_quarto_1");
					sensor_aber_6_gpio_21_janela_quarto_2 = cJSON_GetObjectItemCaseSensitive(dados_json, "sensor_aber_6_gpio_21_janela_quarto_2");
					lamp_1_gpio_17_cozinha = cJSON_GetObjectItemCaseSensitive(dados_json, "lamp_1_gpio_17_cozinha");
					lamp_2_gpio_18_sala = cJSON_GetObjectItemCaseSensitive(dados_json, "lamp_2_gpio_18_sala");
					lamp_3_gpio_27_quarto_1 = cJSON_GetObjectItemCaseSensitive(dados_json, "lamp_3_gpio_27_quarto_1");
					lamp_4_gpio_22_quarto_2 = cJSON_GetObjectItemCaseSensitive(dados_json, "lamp_4_gpio_22_quarto_2"); 
					air_1_gpio_23_quarto_1 = cJSON_GetObjectItemCaseSensitive(dados_json, "air_1_gpio_23_quarto_1");
					air_2_gpio_24_quarto_2 = cJSON_GetObjectItemCaseSensitive(dados_json, "air_2_gpio_24_quarto_2");

					if (cJSON_IsNumber(humidity) && 
						cJSON_IsNumber(temperature) && 
						cJSON_IsNumber(sensor_pre_1_gpio_25_sala) &&
						cJSON_IsNumber(sensor_pre_2_gpio_26_sala) &&
						cJSON_IsNumber(sensor_aber_1_gpio_5_porta_cozinha) &&
						cJSON_IsNumber(sensor_aber_2_gpio_6_janela_cozinha) &&
						cJSON_IsNumber(sensor_aber_3_gpio_12_porta_sala) &&
						cJSON_IsNumber(sensor_aber_4_gpio_16_janela_sala) &&
						cJSON_IsNumber(sensor_aber_5_gpio_20_janela_quarto_1) &&
						cJSON_IsNumber(sensor_aber_6_gpio_21_janela_quarto_2) &&
						cJSON_IsNumber(lamp_1_gpio_17_cozinha) &&
						cJSON_IsNumber(lamp_2_gpio_18_sala) &&
						cJSON_IsNumber(lamp_3_gpio_27_quarto_1) &&
						cJSON_IsNumber(lamp_4_gpio_22_quarto_2) &&
						cJSON_IsNumber(air_1_gpio_23_quarto_1) &&
						cJSON_IsNumber(air_2_gpio_24_quarto_2)){

						aparelhos_dados.humidity = humidity->valuedouble;
						aparelhos_dados.temperature = temperature->valuedouble;
						aparelhos_dados.sensor_pre_1_gpio_25_sala = sensor_pre_1_gpio_25_sala->valueint;
						aparelhos_dados.sensor_pre_2_gpio_26_sala = sensor_pre_2_gpio_26_sala->valueint;
						aparelhos_dados.sensor_aber_1_gpio_5_porta_cozinha = sensor_aber_1_gpio_5_porta_cozinha->valueint;
						aparelhos_dados.sensor_aber_2_gpio_6_janela_cozinha = sensor_aber_2_gpio_6_janela_cozinha->valueint;
						aparelhos_dados.sensor_aber_3_gpio_12_porta_sala = sensor_aber_3_gpio_12_porta_sala->valueint;
						aparelhos_dados.sensor_aber_4_gpio_16_janela_sala = sensor_aber_4_gpio_16_janela_sala->valueint;
						aparelhos_dados.sensor_aber_5_gpio_20_janela_quarto_1 = sensor_aber_5_gpio_20_janela_quarto_1->valueint;
						aparelhos_dados.sensor_aber_6_gpio_21_janela_quarto_2 = sensor_aber_6_gpio_21_janela_quarto_2->valueint;
						aparelhos_dados.lamp_1_gpio_17_cozinha = lamp_1_gpio_17_cozinha->valueint;
						aparelhos_dados.lamp_2_gpio_18_sala = lamp_2_gpio_18_sala->valueint;
						aparelhos_dados.lamp_3_gpio_27_quarto_1 = lamp_3_gpio_27_quarto_1->valueint;
						aparelhos_dados.lamp_4_gpio_22_quarto_2 = lamp_4_gpio_22_quarto_2->valueint;
						aparelhos_dados.air_1_gpio_23_quarto_1 = air_1_gpio_23_quarto_1->valueint;
						aparelhos_dados.air_2_gpio_24_quarto_2 = air_2_gpio_24_quarto_2->valueint;


						printf("\n\n===================== ATUALIZAÇÃO DE DADOS ====================");
						printf("\n Umidade: %lf", humidity->valuedouble);
						printf("\n Temperatura: %lf", temperature->valuedouble);
						printf("\n Sensor presença sala 1: %d", sensor_pre_1_gpio_25_sala->valueint);
						printf("\n Sensor presença sala 2: %d", sensor_pre_2_gpio_26_sala->valueint);
						printf("\n Sensor abertura porta cozinha: %d", sensor_aber_1_gpio_5_porta_cozinha->valueint);
						printf("\n Sensor abertura janela cozinha: %d", sensor_aber_2_gpio_6_janela_cozinha->valueint);
						printf("\n Sensor abertura porta sala: %d", sensor_aber_3_gpio_12_porta_sala->valueint);
						printf("\n Sensor abertura janela sala: %d", sensor_aber_4_gpio_16_janela_sala->valueint);
						printf("\n Sensor abertura janela quarto 1: %d", sensor_aber_5_gpio_20_janela_quarto_1->valueint); 
						printf("\n Sensor abertura janela quarto 2: %d", sensor_aber_6_gpio_21_janela_quarto_2->valueint); 
						printf("\n Lâmpada cozinha: %d", lamp_1_gpio_17_cozinha->valueint);
						printf("\n Lâmpada sala: %d", lamp_2_gpio_18_sala->valueint);
						printf("\n Lâmpada quarto 1: %d", lamp_3_gpio_27_quarto_1->valueint);
						printf("\n Lâmpada quarto 2: %d", lamp_4_gpio_22_quarto_2->valueint);
						printf("\n Ar condicionado quarto 1: %d", air_1_gpio_23_quarto_1->valueint);
						printf("\n Ar condicionado quarto 2: %d\n\n", air_2_gpio_24_quarto_2->valueint);
					}
					
					break;		
				default:
					break;
			}
		}			
	}
}

int configuracaoServidor(){
	/*Cria o descritor*/
    int servidorSocket;
    /*Declaracao da estrutura*/
    struct sockaddr_in serverAddr;
	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

	// Montar a estrutura sockaddr_in
	memset(&serverAddr, 0, sizeof(serverAddr)); // Zerando a estrutura de dados
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
		printf("Falha no Bind\n");

	// Listen
	if(listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");	
	
	return servidorSocket;
}

void menu(){
	int op;

	printf("\n\n\n11 - Acionar estado lâmpada da cozinha \n");
	printf("21 - Acionar lâmpada da sala          \n");
	printf("31 - Acionar lâmpada do quarto 1      \n");
	printf("41 - Acionar lâmpada do quarto 2      \n");
	printf("51 - Acionar ar-condicionado quarto 1 \n");
	printf("61 - Acionar ar-condicionado quarto 2 \n");
	printf("12 - Desligar lâmpada da sala          \n");
	printf("13 - Desligar lâmpada do quarto 1      \n");
	printf("14 - Desligar lâmpada do quarto 2      \n");
	printf("15 - Desligar ar-condicionado quarto 1 \n");
	printf("16 - Desligar ar-condicionado quarto 2 \n");
	printf("17 - Acionar estado lâmpada da cozinha \n");
	scanf("\n>> %d", &op);

	switch (op){
	case 11:
		Cliente(COD_ACIONA_LAMP_COZINHA);
		break;	
	case 21:
		Cliente(COD_ACIONA_LAMP_SALA);
		break;
	case 31:
		Cliente(COD_ACIONA_LAMP_QUARTO_1);
		break;	
	case 41:
		Cliente(COD_ACIONA_LAMP_QUARTO_2);
		break;
	case 51:
		Cliente(COD_ACIONA_AR_QUARTO_1);
		break;	
	case 61:
		Cliente(COD_ACIONA_AR_QUARTO_2);
		break;
	case 12:
		Cliente(COD_DESLIGA_LAMP_SALA);
		break;
	case 13:
		Cliente(COD_DESLIGA_LAMP_QUARTO_1);
		break;	
	case 14:
		Cliente(COD_DESLIGA_LAMP_QUARTO_2);
		break;
	case 15:
		Cliente(COD_DESLIGA_AR_QUARTO_1);
		break;	
	case 16:
		Cliente(COD_DESLIGA_AR_QUARTO_2);
		break;
	case 17:
		Cliente(COD_DESLIGA_LAMP_COZINHA);
		break;	
	default:
		break;
	}
}

int print_dados(){
	// while(1){
	// 	sleep(1);
	// 	printf("\n\n===================== ATUALIZAÇÃO DE DADOS ====================");
	// 	printf("\n %d", aparelhos_dados->humidity);
	// 	printf("\n %d", aparelhos_dados->temperature);
	// 	printf("\n %d", aparelhos_dados->sensor_pre_1_gpio_25_sala);
	// 	printf("\n %d", aparelhos_dados->sensor_pre_2_gpio_26_sala);
	// 	printf("\n %d", aparelhos_dados->sensor_aber_1_gpio_5_porta_cozinha);
	// 	printf("\n %d", aparelhos_dados->sensor_aber_2_gpio_6_janela_cozinha);
	// 	printf("\n %d", aparelhos_dados->sensor_aber_3_gpio_12_porta_sala);
	// 	printf("\n %d", aparelhos_dados->sensor_aber_4_gpio_16_janela_sala);
	// 	printf("\n %d", aparelhos_dados->sensor_aber_5_gpio_20_janela_quarto_1); 
	// 	printf("\n %d", aparelhos_dados->sensor_aber_6_gpio_21_janela_quarto_2); 
	// 	printf("\n %d", aparelhos_dados->lamp_1_gpio_17_cozinha);
	// 	printf("\n %d", aparelhos_dados->lamp_2_gpio_18_sala);
	// 	printf("\n %d", aparelhos_dados->lamp_3_gpio_27_quarto_1);
	// 	printf("\n %d", aparelhos_dados->lamp_4_gpio_22_quarto_2);
	// 	printf("\n %d", aparelhos_dados->air_1_gpio_23_quarto_1);
	// 	printf("\n %d\n\n", aparelhos_dados->air_2_gpio_24_quarto_2);
	// 	fflush(stdout);
	// }	
}

int main(int argc, char *argv[]) {
	int servidorSocket, socketCliente;	

	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;	

	pthread_t thread_server, thread_menu, thread_dados;		

	// Servidor 
	servidorSocket = configuracaoServidor();

	while(1) {
		if (pthread_create(&thread_menu, NULL, menu, NULL)){
			printf("Erro na Thread\n");
			exit(1);
		}

		clienteLength = sizeof(clienteAddr);

		if((socketCliente = accept(servidorSocket, 
			                      (struct sockaddr *) &clienteAddr, 
			                      &clienteLength)) < 0)
			printf("Falha no Accept\n");
		
		printf("\n\nConexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		if (pthread_create(&thread_server, NULL, Servidor, &socketCliente) != 0){
			printf("Erro na Thread\n");
			exit(1);
		}		
	}
	pthread_join(thread_server, NULL);	
	pthread_join(thread_menu, NULL);
	pthread_join(thread_dados, NULL);
	close(servidorSocket);
}
