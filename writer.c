#include "funciones.c"

int getNextLine(int idx, int * buffer, int memSize){
	while(idx != memSize){
		if (buffer[idx] == -1){
			return idx;
		} else{
			idx += 8;
		}
	}
	
	return -1;
}

void * writeLine(void * param){
	
	Args * args = param;

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

	int indexLine;

	while(TRUE){

		sem_wait(sem_block);
		appendText(FILEBLOCK, args->PID, 0);
		sem_post(sem_block);

		sem_wait(sem_memoria);

		sem_wait(sem_block);
		removeText(FILEBLOCK, args->PID, 0);
		sem_post(sem_block);

		sem_wait(sem_run);
		appendText(FILERUN, args->PID, 0);
		sem_post(sem_run);

	
		indexLine = getNextLine(0, buffer, args->memory*8);
	
		if (indexLine != -1){
		
			int * fecha = getDate();

			int msg[8] = {args->PID, indexLine / 8, fecha[0], fecha[1], fecha[2], fecha[3], fecha[4], fecha[5]};

			buffer[indexLine] = args->PID;
			indexLine ++;
			buffer[indexLine] = msg[1];
			indexLine ++;
			buffer[indexLine] = fecha[0];
			indexLine ++;
			buffer[indexLine] = fecha[1];
			indexLine ++;
			buffer[indexLine] = fecha[2];
			indexLine ++;
			buffer[indexLine] = fecha[3];
			indexLine ++;
			buffer[indexLine] = fecha[4];
			indexLine ++;
			buffer[indexLine] = fecha[5];
			indexLine ++;

			sem_wait(sem_log);
			writeLog(args->PID, 0, msg, fecha);
   			sem_post(sem_log);

			printf("\nWriter %d Escribiendo...\n", args->PID);
			sleep(args->tAccion);

		}

		sem_wait(sem_run);
		removeText(FILERUN, args->PID, 0);
		sem_post(sem_run);

		sem_wait(sem_sleep);
		appendText(FILESLEEP, args->PID, 0);
		sem_post(sem_sleep);

		sem_post(sem_memoria);

		printf("\nSleep\n\n############\n");
		sleep(args->tSleep);

		sem_wait(sem_sleep);
		removeText(FILESLEEP, args->PID, 0);
		sem_post(sem_sleep);
	}

}

int main(int argc, char * argv []) {
	
	if (argc != 4){
		printf("Cantidad invalida de parametros: ./writer cant tWrite tSleep\n");
	} else {
		int cantWriters = atoi(argv[1]);
		int tWrite = atoi(argv[2]);
		int tSleep = atoi(argv[3]);
		int memory = getMemorySize();

		pthread_t writer;
		
		for(int i = 0; i < cantWriters; i ++){
			Args * args = malloc(sizeof(Args));
			args->tAccion = tWrite;
			args->tSleep = tSleep;
			args->memory = memory;
			args->PID = i;
			
			
			pthread_create(&writer, 0, writeLine,(void *)args);
			
			//sleep(tSleep);
		}

		pthread_join(writer, NULL);
	}
	
	return 0;
}