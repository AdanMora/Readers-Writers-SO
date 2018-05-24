#ifndef FUNCIONES
#define FUNCIONES

void writeLog(int, int, int *, int *);

int getMemorySize();

void printMemoryLines(int *, int);

char * getMsg(char *, int *);

int * getDate();

void clearFile(char *);

int getRandom(int);

sem_t * getSemaphore(char *);

#endif /* FUNCIONES */