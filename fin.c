#include "funciones.c"

#define KILL_Writer "kill -9 $(pidof writer)"
#define KILL_Reader "kill -9 $(pidof reader)"
#define KILL_Egoista "kill -9 $(pidof egoista)"



void deleteSemaphore(char * name){
	sem_t * sem = sem_open(name, 0);
	sem_unlink(name);
	sem_close(sem);
	sem_destroy(sem);
}

void deleteMemory(char * fileKey, int k){
	int key = ftok(fileKey, k);

	int memory = getMemorySize(FILESIZE);

	int id_zone = shmget (key, sizeof(int)*memory, 0777 | IPC_CREAT);

	int *buffer;
	buffer = shmat (id_zone, (char *)0, 0);

	shmdt ((char *)buffer);
	shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
}

int main(int argc, char const *argv[])
{
	system(KILL_Writer);
	system(KILL_Reader);
	system(KILL_Egoista);
	deleteMemory(FILEKEY, KEY);
	deleteSemaphore(SEM_MEM);
	deleteSemaphore(SEM_LOG);
	deleteSemaphore(SEM_BLOCK);
	deleteSemaphore(SEM_RUN);
	deleteSemaphore(SEM_SLEEP);
	
	printf("* PROCESOS MATADOS, MEMORIA Y SEMAFOROS LIBERADOS *\n");
	return 0;
}




