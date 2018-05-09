#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>

int shmid;
int shmkey = 12222;//u can choose it as your choice

int main(int argc, char * argv []) {
	
	printf("Cant args: %d",argc);
	
	if (argc != 2){
		printf("Cantidad invalida de parametros\n");
	}/*
	//now your main starting
  	shmid = shmget(shmkey,1024,IPC_CREAT);
  	// 1024 = your preferred size for share memory
  	// IPC_CREAT  its a flag to create shared memory

  	//now attach a memory to this share memory
  	char *shmpointer = shmat(shmid,NULL);
	
  	//do your work with the shared memory 
  	//read -write will be done with the *shmppointer
  	//after your work is done deattach the pointer

  	shmdt(&shmpointer, NULL);*/
	
	return 0;
}
	