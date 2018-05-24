#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#include "funciones.h"

#define TRUE 1
#define FALSE 0
#define FILEKEY "/bin/cat"
#define KEY 1300
#define FILESIZE "size.txt"
#define FILELOG "log.txt"
#define SEM_LOG "semLog"

typedef struct Args{
	int tAccion;
	int tSleep;
	int memory;
	int PID;
	
} Args;

void writeLog(int PID, int tipo, int * msg, int hora[]){
	FILE *file = fopen(FILELOG,"a");
	char tipoStr[15];
	char parseMsg [100];
	getMsg(parseMsg, msg);
	if(tipo == 0){
		strcpy(tipoStr,"Writer");
	} else if (tipo == 1){
		strcpy(tipoStr,"Reader");
	} else {
		strcpy(tipoStr,"Reader Egoista");
	}
	if (tipo == 0){
		fprintf(file,"El proceso %d del tipo %s, a las %d:%d:%d, escribio: %s.\n",PID, tipoStr, hora[0], hora[1], hora[2], parseMsg);
	} else if (tipo == 1) {
		fprintf(file,"El proceso %d del tipo %s, a las %d:%d:%d, leyo: %s.\n", PID, tipoStr, hora[0], hora[1], hora[2], parseMsg);
	} else {
		fprintf(file,"El proceso %d del tipo %s, a las %d:%d:%d, leyo y borro: %s.\n", PID, tipoStr, hora[0], hora[1], hora[2], parseMsg);
	}
	fclose(file);
}

int getMemorySize(){
	FILE *f = fopen(FILESIZE, "r");
	char * line = NULL;
	size_t len = 0;
	getline(&line, &len, f);
	return atoi(line);
}

void printMemoryLines(int * buffer, int memory){
	for(int i = 0; i < 8*memory; i++){
		if(i % 8 == 0 && i != 1){
			printf("\n|%d\t", buffer[i]);
		} else{
			printf("%d\t", buffer[i]);
		}
	}
	printf("\n \n");
}

char * getMsg(char * parseMsg, int * msg){
	if (msg == NULL){
		sprintf(parseMsg, "...Linea Vacia...");
	} else {
		sprintf(parseMsg, "El Writer %d escribio en la linea %d a las %d:%d:%d, el %d/%d/%d.\n", msg[0],msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]);
	}
	printf("%s\n", parseMsg);
}

int * getDate(){
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	
	int * fecha = malloc(6);
	
	fecha[0] = tm_struct->tm_hour;
	fecha[1] = tm_struct->tm_min;
	fecha[2] = tm_struct->tm_sec;
	fecha[3] = tm_struct->tm_mday;
	fecha[4] = tm_struct->tm_mon;
	fecha[5]  = tm_struct->tm_year - 100;
	
	printf("H: %d:%d:%d  F: %d/%d/%d\n", fecha[0],fecha[1],fecha[2],fecha[3],fecha[4],fecha[5]);
	
	return fecha;
}

void clearFile(char * name){
	FILE *f = fopen(name,"w");
	fclose(f);
}

int getRandom(int maxVal){
	srand(time(NULL));
	return (rand() % maxVal) * 8;
}

