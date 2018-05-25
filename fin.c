#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define FILEKEY "/bin/cat"
#define KEY 1300
#define FILE_SIZE "size.txt"

int getMemorySize(char * filename){
	FILE *f = fopen(filename, "r");
	char * line = NULL;
	size_t len = 0;

	getline(&line, &len, f);
	return atoi(line);
}

void deleteMemory(char * fileKey, int k){
	int key = ftok(fileKey, k);

	int memory = getMemorySize(FILE_SIZE);

	int id_zone = shmget (key, sizeof(int)*memory, 0777 | IPC_CREAT);

	int *buffer;
	buffer = shmat (id_zone, (char *)0, 0);

	shmdt ((char *)buffer);
	shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
}

int main(int argc, char const *argv[])
{
	
	deleteMemory(FILEKEY, KEY);
	
	printf("* MEMORIA Y SEMAFOROS LIBERADOS *\n");
	return 0;
}




