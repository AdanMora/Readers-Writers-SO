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

		sem_post(sem_block);
		//agregar
		sem_wait(sem_block);

		sem_post(sem_memoria);
	
		printMemoryLines(buffer, args->memory);

		sem_post(sem_block);
		//eliminar
		sem_wait(sem_block);

		sem_post(sem_run);
		//agregar
		sem_wait(sem_run);


		printf("Index Linea %d\n",indexLine);
		indexLine = getNextLine(indexLine, buffer, args->memory*8);
		printf("Index Linea %d\n",indexLine);

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

			printf("Holi\n");

			sem_post(sem_log);
			printf("WAIT\n");
			writeLog(args->PID, 1, msg, fecha);
   			sem_wait(sem_log);
			
			printf("Sleep: %d\n",args->tAccion);
			//sleep(2);
			

		} else {
			break;
		}

		sem_post(sem_run);
		//eliminar
		sem_wait(sem_run);

		sem_post(sem_sleep);
		//agregar
		sem_wait(sem_sleep);

		sem_wait(sem_memoria);
		
	}
	printf("AAAAAAAAAH\n");

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
			
			sleep(1);
		}
	}
	
	return 0;
}