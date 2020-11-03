
#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include "../../main.h"
#include "module_menu.h"

void tempInfo(struct Temperatures *temperatures);
void menuBorders();
void menuUser(struct Temperatures *temperatures);

void *menu(void *params) 
{
    struct Temperatures *temperatures = params;
    int row,col;		
    initscr ();
    
    curs_set (0);

    while (1) {
        char str[80] = "";
        menuBorders();
        tempInfo(temperatures);
        menuUser(temperatures);
        usleep(500000);
        refresh ();
        clear();
    }
    /* End ncurses mode */
    endwin();
    return 0;
}

void tempInfo(struct Temperatures *temperatures) {
    mvprintw (2+MARGIN, 65+MARGIN, "Informações do sistema");
    mvprintw (6+MARGIN, 60+MARGIN, "Temperatura Interior: %0.2lf", temperatures->ti);
    mvprintw (7+MARGIN, 60+MARGIN, "Temperatura Exterior: %0.2lf", temperatures->te);
    mvprintw (8+MARGIN, 60+MARGIN, "Temperatura Referência: %0.2lf", temperatures->tr);
    mvprintw (9+MARGIN, 60+MARGIN, "Histeresse: %0.2lf", temperatures->hysteresis);
    mvprintw (10+MARGIN, 60+MARGIN, "Controle: %s", temperatures->inputType);
}

void menuBorders() {
    char doubleBordX[200] = {"==================================================================================================="};
    char singleBordx[200] = {"---------------------------------------------------------------------------------------------------"};
    mvprintw (0+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (4+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (14+MARGIN, 1+MARGIN, singleBordx);
    for(int i=1; i<14; i++) {
      mvprintw (i+MARGIN, 0+MARGIN, "|");
      mvprintw (i+MARGIN, 50+MARGIN, "|");
      mvprintw (i+MARGIN, 100+MARGIN, "|");
    }
}

void menuUser(struct Temperatures *temperatures) {
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    char choice[10] = {'$'};
    // char breakline;
    char controlType;
    mvprintw (2+MARGIN, 20+MARGIN, "Menu do Sistema");
    mvprintw(5+MARGIN,2+MARGIN,"a. Alterar temperatura");
    mvprintw(6+MARGIN,2+MARGIN,"b. Alterar histerese");
    mvprintw(7+MARGIN,2+MARGIN,"c. Alterar tipo de controle");
    mvprintw(8+MARGIN,2+MARGIN,"Pressione CTRL + C para sair do sistema");

    mvprintw(9+MARGIN,2+MARGIN,"Digite a sua escolha:");
    if( poll(&mypoll, 1, 0) ){
      getstr(choice);
    }

    if(!strcmp(choice, "a")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite a temperatura: ");
      scanw(" %4lf", &temperatures->tr);
      strcpy(temperatures->inputType, "t");
    }
    else if(!strcmp(choice, "b")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o valor da histerese: ");
      scanw(" %4lf", &temperatures->hysteresis) ;
    }
    else if(!strcmp(choice, "c")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o controle desejado");
      mvprintw(12+MARGIN,2+MARGIN,"(p -> Potenciômetro/ t -> teclado):");
      char controle[10];
      getstr(controle);
      if (!strcmp(controle, "p") || !strcmp(controle, "t")) {
        strcpy(temperatures->inputType, controle);
        mvprintw(13+MARGIN,2+MARGIN,"Controle alterado");
      }
      else {
        mvprintw(13+MARGIN,2+MARGIN,"Escolha inválida");
      }
    }
    else if(!strcmp(choice, "$")) {
      mvprintw(10+MARGIN,2+MARGIN,"");
    }
    else {
      mvprintw(10+MARGIN,2+MARGIN,"Escolha inválida");
    }
}