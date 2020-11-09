# Automação

Este trabalho tem por objetivo a criação de um sistema distribuído de automação residencial para controle e monitoramento de sensores e dispositivos. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura, sensores de presença, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado e alarme.

## Requisitos

Os sistema de controle possui os seguintes requisitos:

1. O código do Servidor Distribuído deve ser desenvolvido em C/C++;
2. O código do Servidor Central pode ser desenvolvido em Python, C ou C++;

3. O servidor central tem as seguintes responsabilidades:

    3.1 Manter conexão com o servidor distribuído; 

    3.2 Prover uma interface que mantenha o usuário atualizado sobre o estado de cada dispositivo (atualizado a cada 1 segundo), incluindo a temperatura e umidade;

    3.3 Prover mecanismo para que o usuário possa acionar manualmente lâmpadas e aparelhos de ar-condicionado;

    3.4 Prover mecanismo para que o usuário possa definir uma temperatura desejada em que ambos os aparelhos de ar-condicionado deverão ser controlados (Controle on/off) em torno desta temperatura).
    3.5 Prover mecanismo para acionamento de uma alarme que, quando estiver ligado, deve tocar um som de alerta ao detectar presenças ou abertura de portas/janelas;
    3.6 Manter log (Em arqvuio CSV) dos comandos acionados pelos usuários e do acionamento dos alarmes;

4. O Servidor distribuído deve fazer interface direta com os dispositivos com as seguintes responsabilidades:

    4.1 Manter os valores de temperatura / umidade atualizados a cada 1 segundo;

    4.2 Acionar Lâmpadas e Aparelhos de Ar-Condicionado (mantendo informação sobre seu estado) conforme comandos do Servidor Central;

    4.3 Manter o estado dos sensores de presença / abertura de portas/janelas informando ao servidor central imediatamente (mensagem push) quando detectar o acionamento de qualquer um deles;

* Os códigos em C/C++ devem possuir Makefile para compilação;
Descrever no README do repositório o modo de instalação/execução e o modo de uso do programa.


## Instalação

Para instalar é preciso clonar o projeto e na rasp, dentro da pasta trabalhos2_embarcados:

1. Compilar o servidor: make distribuido ou make central
2. Realizar a execução dos executáveis nas devidas rasps com ./distribuido ou ./central 

## Features

- Implementação de comunicação entre os servidores usando o protocolo TCP/IP.
- Interface (linha de comando) apresentando o estado de cada dispositivo, a temperatura e a umidade.
- Mecanismo para acionamento de dispositivos
- Leitura e armazenamento dos valores de temperatura / umidade a cada 1 segundo.
- Correto acionamento de lâmpadas e aparelhos de ar-condicionado pelo comando do Servidor Central.

#### Limitações

- Não permite definir a temperatura do Ar 
- Não implementa mecanismo de ligar/desligar alarme 
- Não guarda log de comandos em CSV

#### Problemas Conhecidos

- Execução de threads está impedindo a execução correta do menu com coleta de opção selecionada
- Não possui menu dividido