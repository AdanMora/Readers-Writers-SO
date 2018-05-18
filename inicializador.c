#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <string.h>
#include <time.h>

#define FILEKEY "/bin/cat"
#define KEY 1300
#define FILESIZE "size.txt"

int shmid1;
int shmid2;
int shmkey = 12222;//u can choose it as your choice

void inicializarMemoria(int memory){
	
	int key = ftok(FILEKEY, KEY);
	if(key == -1){
		printf("Error con la llave\n");
		exit(1);
	}

	// Creamos la memoria compartida
	int id = shmget (key, sizeof(int)*(8*memory), 0777 | IPC_CREAT);
	if (id == -1) {
		printf("Error creando la memoria compartida \n");
		exit(1);
	}
	printf("* MEMORIA CREADA CON EXITO *\n");

	int *buffer; /* shared buffer */

	/* we declared to zone to share */
	buffer = shmat (id, (char *)0, 0);
	if (buffer == NULL) {
		printf("Error reservando la memoria compartida\n");
		exit(1);
	}

	for (int i = 0; i < (8*memory); i++){
		buffer[i] = -1;
	}
	
	FILE *f = fopen(FILESIZE,"w");
	fprintf(f,"%d\n",memory);
	fclose(f);

	for(int i = 0; i < 8*memory; i++){
		if(i % 8 == 0 && i != 1){
			printf("\n|%d\t", buffer[i]);
		} else{
			printf("%d\t", buffer[i]);
		}
	}
	printf("\n \n");
	
	writeLine(2,5, id);
	
	for(int i = 0; i < 8*memory; i++){
		if(i % 8 == 0 && i != 1){
			printf("\n|%d\t", buffer[i]);
		} else{
			printf("%d\t", buffer[i]);
		}
	}
	printf("\n \n");
	
	
}

void writeLine(int line, int PID, int id){
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	int hour = tm_struct->tm_hour;
	int min = tm_struct->tm_min;
	int sec = tm_struct->tm_sec;
	int day = tm_struct->tm_mday;
	int mon = tm_struct->tm_mon;
	int year = tm_struct->tm_year;
	
	int *buffer = shmat (id, (char *)0, 0);
	if (buffer == NULL) {
		printf("Error reservando la memoria compartida\n");
		exit(1);
	}
	
	int indexLine = line * 8;
	
	buffer[indexLine] = line;
	indexLine ++;
	buffer[indexLine] = PID;
	indexLine ++;
	buffer[indexLine] = hour;
	indexLine ++;
	buffer[indexLine] = min;
	indexLine ++;
	buffer[indexLine] = sec;
	indexLine ++;
	buffer[indexLine] = day;
	indexLine ++;
	buffer[indexLine] = mon;
	indexLine ++;
	buffer[indexLine] = year;
	
	printf("H: %d:%d:%d  F: %d/%d/%d\n",hour,min,sec,day,mon,year);
	
}

int main(int argc, char * argv []) {
	
	int memory = atoi(argv[1]);
	
	if (argc != 2){
		printf("Cantidad invalida de parametros\n");
	} else {
		inicializarMemoria(memory);
	}
	
	/*int key1 = ftok(FILEKEY, KEY);
	
	printf("Key -> %d\n",key1);
	
	int id1 = shmget (key1, sizeof(int)*memory, 0777 | IPC_CREAT);
	
	printf("ID -> %d\n",id1);
	
	buffer = shmat (id1, (char *)0, 0);*/
	
	return 0;
}
	