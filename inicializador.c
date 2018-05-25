#include "funciones.c"

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

	printMemoryLines(buffer, memory);
	
}

void createSemaphores(){
	sem_open(SEM_LOG, O_CREAT | O_EXCL, 0644, 1);
	sem_open(SEM_BLOCK, O_CREAT | O_EXCL, 0644, 1);
	sem_open(SEM_SLEEP, O_CREAT | O_EXCL, 0644, 1);
	sem_open(SEM_RUN, O_CREAT | O_EXCL, 0644, 1);
	sem_open(SEM_MEM, O_CREAT | O_EXCL, 0644, 1);
}

int main(int argc, char * argv []) {
	
	if (argc != 2){
		printf("Cantidad invalida de parametros: ./init cantLineas\n");
	} else {
		int memory = atoi(argv[1]);
	
		FILE *f = fopen(FILESIZE,"w");
		fprintf(f,"%d\n",memory);
		fclose(f);
		
		inicializarMemoria(memory);
		createSemaphores();
		clearFile(FILELOG);
		clearFile(FILEBLOCK);
		clearFile(FILESLEEP);
		clearFile(FILERUN);
	}
	
	return 0;
}
	