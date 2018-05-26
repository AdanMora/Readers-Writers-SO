#include "funciones.c"

int hayLineas(int * buffer, int memSize){
	int idx = 0;
	while(idx != memSize){
		if (buffer[idx] != -1){
			return 1;
		} else{
			idx += 8;
		}
	}
	
	return 0;
}

int getNextLine(int idx, int * buffer, int memSize){
	while(hayLineas(buffer, memSize)){
		if (idx == memSize){
			return -1;
		}else if (buffer[idx] != -1){
			return idx;
		} else{
			idx += 8;
		}
	}
	
	return -1;
}

void * readLine(void * param){
	
	Args * args = param;

	//sem_t * sem_log = sem_open(SEM_LOG,0);

	sem_t * sem_log = getSemaphore(SEM_LOG);
	sem_t * sem_block = getSemaphore(SEM_BLOCK);
	sem_t * sem_sleep = getSemaphore(SEM_SLEEP);
	sem_t * sem_run = getSemaphore(SEM_RUN);
	sem_t * sem_memoria = getSemaphore(SEM_MEM);
	
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
	
	while (TRUE){

		sem_wait(sem_block);
		appendText(FILEBLOCK, args->PID, 1);
		sem_post(sem_block);

		sem_wait(sem_memoria);

		sem_wait(sem_block);
		removeText(FILEBLOCK, args->PID, 1);
		sem_post(sem_block);

		sem_wait(sem_run);
		appendText(FILERUN, args->PID, 1);
		sem_post(sem_run);

		indexLine = getNextLine(0, buffer, args->memory*8);

		while (indexLine != -1){
			
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

			sem_wait(sem_log);
			writeLog(args->PID, 1, msg, fecha);
   			sem_post(sem_log);
			
			printf("\nReader %d Leyendo...\n", args->PID);
			sleep(args->tAccion);

			indexLine = getNextLine(indexLine, buffer, args->memory*8);
		}

		indexLine = getNextLine(0, buffer, args->memory*8);

		sem_wait(sem_run);
		removeText(FILERUN, args->PID, 1);
		sem_post(sem_run);

		sem_wait(sem_sleep);
		appendText(FILESLEEP, args->PID, 1);
		sem_post(sem_sleep);

		sem_post(sem_memoria);

		printf("\nSleep\n\n############\n");
		sleep(args->tSleep);

		sem_wait(sem_sleep);
		removeText(FILESLEEP, args->PID, 1);
		sem_post(sem_sleep);
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

		pthread_t reader;
		
		for(int i = 0; i < cantReaders; i ++){
			Args * args = malloc(sizeof(Args));
			args->tAccion = tRead;
			args->tSleep = tSleep;
			args->memory = memory;
			args->PID = i;
			
			pthread_create(&reader, 0, readLine,(void *)args);
			
			//sleep(1);
		}

		pthread_join(reader, NULL);
	}
	
	return 0;
}