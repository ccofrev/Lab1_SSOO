#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void recibirArgumentos(int argc, char *argv[], int *Archivos, int *UmbralBin, int *UmbralClas, char *Mask, int *flag){
	int flags, opt;
	char *aux3, *mask;
	aux3 = malloc(10*sizeof(char));
    
	if(argc <3){
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n", argv[0]);
		exit(EXIT_FAILURE);
		}
	int nArchivos = -1, nUmbralBin = -1, nUmbralClas = -1;
	flags = 0;
	
	while((opt = getopt(argc, argv, "c:u:m:n:b")) != -1) { 
	
	   switch(opt) {

            case 'c':
                nArchivos = strtol(optarg, &aux3, 10);
                if(optarg!=0 && nArchivos==0){
                    fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;

			case 'u':
				nUmbralBin = strtol(optarg, &aux3, 10);
				if(optarg!=0 && nUmbralBin==0){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;

			case 'n':
				nUmbralClas = strtol(optarg, &aux3, 10);
				if(optarg!=0 && nUmbralClas==0){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;

            case 'm':
                mask=malloc(strlen(optarg));
                strcpy(mask,optarg);
                printf("wena choro: %s\n", mask);
				if(optarg!=0 && mask[0]=='\0'){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;

            case 'b':
				flags = 1;
				break;

       }
    }


	(*flag) = flags==1?1:0;

	(*Archivos) = nArchivos;
    (*UmbralBin) = nUmbralBin;
	(*UmbralClas) = nUmbralClas;

    strcpy(Mask, mask);
	
    
	if(nArchivos<=0){
		printf("El valor que acompaña a -c debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
		exit(EXIT_FAILURE);
		}

	if(nUmbralBin<=0){
		printf("El valor que acompaña a -u debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
		exit(EXIT_FAILURE);
		}
	if(nUmbralClas<=0){
		printf("El valor que acompaña a -n debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
		exit(EXIT_FAILURE);
		}
    if(mask[0]=='\0'){
		printf("El valor que acompaña a -m debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
		exit(EXIT_FAILURE);
		}

	//printf("Se obtuvieron los siguientes parametros: %d %d %d %s %s", nArchivos, nUmbralBin, nUmbralClas, texto, flag==0?"Sin Flag":"Flag" );


}

void main(int argc, char *argv[]){

    int nArchivos = 0, nUmbralBin = 0, nUmbralClas = 0;
    char *mask;
    int flag = 0;
    recibirArgumentos(argc, argv, &nArchivos, &nUmbralBin, &nUmbralClas, mask, &flag);

    printf("Opcion h: %d\nOpcion u: %d\nOpcion n: %d\nFlag: %d\nMascara: %s\n", nArchivos, nUmbralBin, nUmbralClas, flag, mask);
	//recibirArgumentos(argc, argv, &nArchivos, &nUmbralBin, &nUmbralClas, &flag);

}