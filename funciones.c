
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// c:u:m:n:b
void recibirArgumentos(int argc, char *argv[], int *Archivos, int *UmbralBin, int *UmbralClas, char *Mask, int *flag){
	int flags, opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
    
	if(argc <3){
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b] (imprimir conclusion)\n", argv[0]);
		exit(EXIT_FAILURE);
		}
	int nArchivos = -1, nUmbralBin = -1, nUmbralClas = -1;
	flags = 0;
	
	while((opt = getopt(argc, argv, "c:u:m:n:b")) != -1) { 
	
	   switch(opt) {

            case 'c':
                nArchivos = strtol(optarg, &aux3, 10);
                if(optarg!=0 && nArchivos==0){
                    fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b] (imprimir conclusion)\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;

			case 'u':
				nUmbralBin = strtol(optarg, &aux3, 10);
				if(optarg!=0 && nUmbralBin==0){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b] (imprimir conclusion)\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;

			case 'n':
				nUmbralClas = strtol(optarg, &aux3, 10);
				if(optarg!=0 && nUmbralClas==0){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b] (imprimir conclusion)\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;

            case 'm':
				strcpy(Mask, optarg);
				if(optarg[0]=='\0'){
					fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b] (imprimir conclusion)\n", argv[0]);
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
	/*
    if(Mask[0]=='\0'){
		printf("El valor que acompaña a -m debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-c numero de imagenes] [-m archivo mascara] [-u umbral binario] [-n umbral clasificacion] [-b imprimir conclusion]\n", argv[0]);
		exit(EXIT_FAILURE);
		}*/

	//printf("Se obtuvieron los siguientes parametros: %d %d %d %s %s", nArchivos, nUmbralBin, nUmbralClas, texto, flag==0?"Sin Flag":"Flag" );

}

// funcion que lee la mascara para el laplaciano desde archivo
	void getMask(int mascara[3][3], char* rutaArchivo){
		int lapMask[3][3];
		FILE *fptr;

		if ((fptr = fopen(rutaArchivo, "r")) == NULL)
			printf("Error! opening file");

		fscanf(fptr,"%d %d %d\n%d %d %d\n%d %d %d", &lapMask[0][0],&lapMask[0][1],&lapMask[0][2],&lapMask[1][0],&lapMask[1][1],&lapMask[1][2],&lapMask[2][0],&lapMask[2][1],&lapMask[2][2]);
		fclose(fptr); 

		for(int j=0; j<3; j++){
			for(int i=0; i<3; i++){
			mascara[j][i] = lapMask[j][i];  
			//printf("");
			}
		}
	}
