#include "funciones.c"

void verEstado(char* name){
	FILE *archivo;
	char ch='s';
	archivo = fopen(name,"r");
	while (ch != EOF)

        {

            printf("%c", ch);

            ch = getc(archivo);

        }
}

void menu(){
	int memory = getMemorySize(FILESIZE);
	int choice;
	do{
		printf("#####################\n");
		printf("Bienvenido al menu del espia, digite una opcion\n");
		printf("1 - Ver estado de la memoria\n");
		printf("2 - Ver estado de los Writers\n");
		printf("3 - Ver estado de los Readers\n");
		printf("4 - Ver estado de los Readers Egoistas\n");
		
		printf("5 - Salir\n");
		scanf("%d",&choice);
		printf("\n");

		switch(choice){
			case 1:
				//seeMemoryState(FILEKEY, KEY, memory);
				break;
			case 2:	
				printf("======Writers======\n");
				verEstado(FILERUN);
				printf("===================\n");
				break;
			case 3:
				printf("======Readers======\n");
				verEstado(FILERUN);
				printf("===================\n");
				break;
			case 4:
				printf("======Writers======\n");
				verEstado(FILERUN);
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
