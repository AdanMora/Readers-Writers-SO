#include "funciones.c"

int * inicializarMemoria(int memory){
	
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
	printf("* MEMORIA ACCEDIDA CON EXITO *\n");

	int *buffer; /* shared buffer */

	/* we declared to zone to share */
	buffer = shmat (id, (char *)0, 0);
	if (buffer == NULL) {
		printf("Error reservando la memoria compartida\n");
		exit(1);
	}

	return buffer;
}

void verEstado(char tipo){
	FILE * archivo;
	bool flag = 0;
	char ch ='s';
	archivo = fopen(FILEBLOCK,"r");
	while(ch!= EOF){
		ch=getc(archivo);
		if(ch == tipo){
			while(ch!= '\n'){
				printf("%c",ch);
				ch = getc(archivo);
			}
			printf(" esta bloqueado\n");
		}
		
			
	}

	fclose(archivo);
	ch='e';
	archivo = fopen(FILERUN,"r");
	
	while(ch!= EOF){
		ch=getc(archivo);
		if(ch == tipo){
			while(ch!= '\n'){
				printf("%c",ch);
				ch = getc(archivo);
			}
			printf(" esta corriendo\n");
		}
	}
	ch='s';
	
	fclose(archivo);
	
	archivo = fopen(FILESLEEP,"r");
	while(ch!= EOF){
		ch=getc(archivo);
		if(ch == tipo){
			while(ch!= '\n'){
				printf("%c",ch);
				ch = getc(archivo);
			}
			printf(" esta durmiendo\n");
		}
	}

	fclose(archivo);

	}

void verEstadoAux(int tipo){
	char proceso;
	if(tipo == 0){
		proceso = 'W';
		verEstado(proceso);
	}
	else if(tipo ==1){
		proceso = 'R';
		verEstado(proceso);		
	}else{
		proceso = 'E';
		verEstado(proceso);
	}

	
}

void menu(){
	int memory = getMemorySize(FILESIZE);
	int * buffer = inicializarMemoria(memory);
	sem_t * sem_block = getSemaphore(SEM_BLOCK);
	sem_t * sem_sleep = getSemaphore(SEM_SLEEP);
	sem_t * sem_run = getSemaphore(SEM_RUN);
	sem_t * sem_memoria = getSemaphore(SEM_MEM);

	int choice;
	do{
		printf("#####################\n\n");
		printf("Bienvenido al menu del espia, digite una opcion\n\n");
		printf("1 - Ver estado de la memoria\n");
		printf("2 - Ver estado de los Writers\n");
		printf("3 - Ver estado de los Readers\n");
		printf("4 - Ver estado de los Readers Egoistas\n");
		
		printf("5 - Salir\n");
		scanf("%d",&choice);
		printf("\n");

		switch(choice){
			case 1:
				sem_wait(sem_memoria);
				printMemoryLines(buffer, memory);
				sem_post(sem_memoria);
				break;
			case 2:	
				printf("======Writers======\n");
				sem_wait(sem_block);
				sem_wait(sem_sleep);
				sem_wait(sem_run);
				verEstadoAux(0);
				sem_post(sem_run);
				sem_post(sem_sleep);
				sem_post(sem_block);
				printf("===================\n");
				break;
			case 3:
				printf("======Readers======\n");
				sem_wait(sem_block);
				sem_wait(sem_sleep);
				sem_wait(sem_run);
				verEstadoAux(1);
				sem_post(sem_run);
				sem_post(sem_sleep);
				sem_post(sem_block);
				printf("===================\n");
				break;
			case 4:
				printf("======Egoistas======\n");
				sem_wait(sem_block);
				sem_wait(sem_sleep);
				sem_wait(sem_run);
				verEstadoAux(2);
				sem_post(sem_run);
				sem_post(sem_sleep);
				sem_post(sem_block);
				printf("===================\n");;
				break;

		}
	} while(choice != 5);
	printf("ADIOS ESPIA!\n");
}

int main(int argc, char const *argv[])
{
	menu();
	return 0;
}
