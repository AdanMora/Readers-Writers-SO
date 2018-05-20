#include "funciones.c"

int getNextLine(int idx, int * buffer, int memSize){
	
	while(idx != memSize){
		if (buffer[idx] != -1){
			return idx;
		} else{
			idx += 8;
		}
	}
	
	return -1;
}

void * readLine(void * param){
	
	Args * args = param;
	
	int key = ftok(FILEKEY, KEY);
	if(key == -1){
		printf("Error con la llave\n");
		exit(1);
	}

	// Creamos la memoria compartida
	int id = shmget (key, sizeof(int)*(8*args->memory), 0777 | IPC_CREAT);
	if (id == -1) {
		printf("Error creando la memoria compartida \n");
		exit(1);
	}
	printf("* MEMORIA ACCEDIDA CON EXITO *\n");
	
	int * buffer = shmat (id, (char *)0, 0);
	if (buffer == NULL) {
		printf("Error reservando la memoria compartida\n");
		exit(1);
	}
	
	int indexLine = 0;
	
	printMemoryLines(buffer, args->memory);
	
	while (1){
		indexLine = getNextLine(indexLine, buffer, args->memory*8);
		
		if (indexLine != -1){
		
			int * fecha = getDate();
			int msg[8];

			msg[0] = buffer[indexLine];
			indexLine ++;
			msg[1] = buffer[indexLine];
			indexLine ++;
			msg[2] = buffer[indexLine];
			indexLine ++;
			msg[3] = buffer[indexLine];
			indexLine ++;
			msg[4] = buffer[indexLine];
			indexLine ++;
			msg[5] = buffer[indexLine];
			indexLine ++;
			msg[6] = buffer[indexLine];
			indexLine ++;
			msg[7] = buffer[indexLine];
			indexLine ++;

			writeLog(args->PID, 1, msg, fecha);
			
			sleep(args->tAccion);

		} else {
			break;
		}
		
	}

}

int main(int argc, char * argv []) {
	
	if (argc != 4){
		printf("Cantidad invalida de parametros: ./reader cant tRead tSleep\n");
	} else {
		int cantReaders = atoi(argv[1]);
		int tRead = atoi(argv[2]);
		int tSleep = atoi(argv[3]);
		int memory = getMemorySize();
		
		for(int i = 0; i < cantReaders; i ++){
			Args * args = malloc(sizeof(Args));
			args->tAccion = tRead;
			args->tSleep = tSleep;
			args->memory = memory;
			args->PID = i;
			
			pthread_t reader;
			pthread_create(&reader, 0, readLine,(void *)args);
			
			sleep(tSleep);
		}
	}
	
	return 0;
}