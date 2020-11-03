#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 10018
#define IP_DISTRIBUIDO "192.168.0.52"
#define IP_CENTRAL "192.168.0.53"

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
    printf("Enviado! %s \n", buffer);
}

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
  char buffer[256];

  printf("Estamos no cliente"); 

  socketCliente = configuracaoCliente();
  tamanhoMensagem = strlen(mensagem);

  if(send(socketCliente, mensagem, tamanhoMensagem, 0) != tamanhoMensagem)
		printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

  totalBytesRecebidos = 0;

	while(totalBytesRecebidos < tamanhoMensagem) {
		if((bytesRecebidos = recv(socketCliente, buffer, 16-1, 0)) <= 0)
			printf("Não recebeu o total de bytes enviados\n");
		totalBytesRecebidos += bytesRecebidos;
		buffer[bytesRecebidos] = '\0';
		printf("%s\n", buffer);
	}
	close(socketCliente);

	return;
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

		printf("Enviado! %s \n", buffer_do_cliente);
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

int main(int argc, char *argv[]) {
	int servidorSocket;
	int socketCliente;
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;	

	pthread_t thread_server;

	Cliente("Teste");
	printf("Continuando");

	// Servidor 
	servidorSocket = configuracaoServidor();

	while(1) {
		clienteLength = sizeof(clienteAddr);

		if((socketCliente = accept(servidorSocket, 
			                      (struct sockaddr *) &clienteAddr, 
			                      &clienteLength)) < 0)
			printf("Falha no Accept\n");
		
		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		if (pthread_create(&thread_server, NULL, Servidor, &socketCliente) != 0){
			printf("Erro na Thread\n");
			exit(1);
		}

		// TrataClienteTCP(socketCliente);
	}
	pthread_join(thread_server, NULL);

	printf("Thread criada\n");

	
	close(servidorSocket);
}
