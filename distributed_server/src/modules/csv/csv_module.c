#include <stdio.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include "../main.h"

void *register_csv(void *params);
void create_marks_csv(char *filename, struct Temperatures *temperatures);
int cfileexists(const char *filename);

void *register_csv(void *params)
{
  struct Temperatures *temperatures = params;
  create_marks_csv("data.csv", temperatures);
}

int cfileexists(const char *filename)
{
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void create_marks_csv(char *filename, struct Temperatures *temperatures)
{
    FILE *fp;
    int file_exists;
    file_exists = cfileexists(filename);
    fp = fopen(filename, "a");
    if (file_exists != 1)
    {
      fprintf(fp, "Data, Hora, Temperatura Interna, Temperatura Externa, Temperatura Referência");
    }
    while(1){
      time_t t = time(NULL);
      struct tm date = *localtime(&t);
      fprintf(fp, "\n%d-%02d-%02d , %02d:%02d:%02d,", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
      fprintf(fp, "%0.2lf , %0.2lf , %0.2lf ", temperatures->ti, temperatures->te, temperatures->tr);
      sleep(2);
    }
    fclose(fp);
}