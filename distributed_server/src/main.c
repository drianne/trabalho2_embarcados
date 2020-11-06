#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "./modules/i2c/i2c_module.h"
#include "./modules/gpio/gpio_module.h"
// #include "./modules/csv/module_csv.h"

struct I2cValue i2c_values; // Temperatura e humidade
struct Sensores sensor_values;
struct Aparelhos aparelhos_values;


#define PORT 10018
#define IP_DISTRIBUIDO "192.168.0.52"
#define IP_CENTRAL "192.168.0.53"

void trata_interrupcao(int sinal) {
    // start_resistor(OFF);
    // start_fan(OFF);
    bcm2835_close();
    // endwin();
    exit(0);
}

void TrataClienteTCP(int socketCliente) {
	char buffer[16];
	int tamanhoRecebido;

	if((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
		printf("Erro no recv()\n");

	while (tamanhoRecebido > 0) {
		if(send(socketCliente, buffer, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");
		
		if((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0)
			printf("Erro no recv()\n");
	}
}

void* Servidor(void* arg){
    /*Buffer de entrada (armazena buffer do cliente)*/
    char buffer_do_cliente[256];
	int tamanhoRecebido;
    /*Cast do ponteiro*/
    int sockEntrada = *(int *) arg;
    /*Loop "infinito"*/
    printf("Aguardando as mensagens...\n\n");

	if((tamanhoRecebido = recv(sockEntrada, buffer_do_cliente, 16, 0)) < 0)
			printf("Erro no recv()\n");

    while (tamanhoRecebido > 0){
		if(send(sockEntrada, buffer_do_cliente, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");

		if((tamanhoRecebido = recv(sockEntrada, buffer_do_cliente, 16, 0)) < 0)
			printf("Erro no recv()\n");

	}
}

int configuracaoCliente(){
  int socketCliente;
  struct sockaddr_in servidorAddr;

  if((socketCliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Falha no socker do Servidor\n");

  servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_CENTRAL);
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

void envia_dado(){
	while(1){
		sleep(1);

		char *data_status = (char *)malloc(1024*sizeof(char));

		sprintf(data_status, "{\"cod\": %d, \"temperatura\": %f, \"umidade\": %f, \"sensor_pre_1_gpio_25_sala\": %d, \"sensor_pre_2_gpio_26_sala\": %d, \"sensor_aber_1_gpio_5_porta_cozinha\": %d, \"sensor_aber_2_gpio_6_janela_cozinha\": %d, \"sensor_aber_3_gpio_12_porta_sala\": %d, \"sensor_aber_4_gpio_16_janela_sala\": %d, \"sensor_aber_5_gpio_20_janela_quarto_1\": %d, \"sensor_aber_6_gpio_21_janela_quarto_2\": %d, \"lamp_1_gpio_17_cozinha\": %d, \"lamp_2_gpio_18_sala\": %d, \"lamp_3_gpio_27_quarto_1\": %d, \"lamp_4_gpio_22_quarto_2\": %d, \"air_1_gpio_23_quarto_1\": %d, \"air_2_gpio_24_quarto_2\": %d}", 
				COD_PEGA_DADO, i2c_values.temperature, 
				i2c_values.humidity, sensor_values.sensor_pre_1_gpio_25_sala,
				sensor_values.sensor_pre_2_gpio_26_sala, sensor_values.sensor_aber_1_gpio_5_porta_cozinha, sensor_values.sensor_aber_2_gpio_6_janela_cozinha,
				sensor_values.sensor_aber_3_gpio_12_porta_sala, sensor_values.sensor_aber_4_gpio_16_janela_sala, sensor_values.sensor_aber_5_gpio_20_janela_quarto_1,
				sensor_values.sensor_aber_6_gpio_21_janela_quarto_2, aparelhos_values.lamp_1_gpio_17_cozinha, aparelhos_values.lamp_2_gpio_18_sala,
				aparelhos_values.lamp_3_gpio_27_quarto_1, aparelhos_values.lamp_4_gpio_22_quarto_2,  aparelhos_values.air_1_gpio_23_quarto_1,
				aparelhos_values.air_2_gpio_24_quarto_2);

		fflush(stdout);

		Cliente(data_status);
	}

}

void atualiza_dados(){
	
}

int main(int argc, char *argv[]) {
  pthread_t I2c_thread, Gpio_thread, aparelhos_thread, tCsv, tMenu, thread_data, sensor_thread, thread_server;
  
  // Servidor
  int servidorSocket;
  int socketCliente;
  struct sockaddr_in servidorAddr;
  struct sockaddr_in clienteAddr;
  unsigned short servidorPorta;
  unsigned int clienteLength;	

  init_gpio();
  
  pthread_create(&sensor_thread, NULL, control_sensors, &sensor_values);
  pthread_create(&aparelhos_thread, NULL, control_aparelhos, &aparelhos_values);
  pthread_create(&I2c_thread, NULL, temperature_humidity_i2c, &i2c_values);
  pthread_create(&thread_data, NULL, envia_dado, NULL);

  // Servidor
  servidorSocket = configuracaoServidor();

	while(1) {
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
	pthread_join(sensor_thread, NULL);
	pthread_join(thread_server, NULL);	
	pthread_join(thread_data, NULL);
	pthread_join(I2c_thread, NULL);
	pthread_join(aparelhos_thread, NULL);
	close(servidorSocket);
}