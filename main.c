/*
 * se compila con
 * gcc main.c -o lab1 -std=c11 -ljpeg
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <jpeglib.h>    
#include <string.h>                                                                
#include "funciones.h"
#include "jpeg.h"


int main(int argc, char *argv[]){

    ////////////// VARIABLES

    /// Parametros recibidos por consola
	int nArchivos = 0;      // -c cantidad de imagenes
    int nUmbralBin = 0;     // -u umbral para binarizar imagen
    int nUmbralClas = 0;    // -n umbral para clasificacion nearly black    
    char archivoMascara[20];// -m nombre archivo mascara
    int flag = 0;           // -b bandera para resultado clasificacion en pantalla

    // se reciben parametros
    recibirArgumentos(argc, argv, &nArchivos, &nUmbralBin, &nUmbralClas, archivoMascara, &flag);

    // Se inician estructuras de datos para imagenes a usar
    JpegData jpegDataOri, jpegDataDstBn, jpegDataDstBnLap, jpegDataDstBnLapUm, jpegDataDst;

    //struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // var para iteracion imagenes
    int i_img = 0;

    /// NOMBRES DE ARCHIVOS
    char imgSrcPath[30];
    char imgSrcName[10];
    char imgDstPath[30];
    char imgDstName[10];

    //// for para recorrer todos los archivos
    for(i_img=1; i_img<=nArchivos; i_img++){

        // se arman nombres archivos y ubicaciones
        sprintf(imgSrcName, "imagen_%d", i_img);
        sprintf(imgSrcPath, "./imagenes_prueba/%s.jpg", imgSrcName);

        sprintf(imgDstName, "salida_%d", i_img);
        sprintf(imgDstPath, "./salida_ejemplo/%s.jpg", imgDstName);

        // Se lee archivo origen
        if (!read_jpeg(&jpegDataOri, imgSrcPath, &jerr)){
            free_jpeg(&jpegDataOri);
            return -1;
        }
        //printf("Read:  %s\n", src);

        // Imagen destino escala de grises
        init_jpeg(&jpegDataDstBn, jpegDataOri.width, jpegDataOri.height, 1);
        
        // Imagen destino escala grises + laplaciano
        init_jpeg(&jpegDataDstBnLap, jpegDataOri.width, jpegDataOri.height, 1);

        // Imagen destino escala grises + laplaciano + umbral
        init_jpeg(&jpegDataDstBnLapUm, jpegDataOri.width, jpegDataOri.height, 1);

        // Imagen destino escala grises + laplaciano + umbral
        init_jpeg(&jpegDataDst, jpegDataOri.width, jpegDataOri.height, 1);


        // se propone una imagen guardada en un solo arreglo
        // así, una imagen de dimensiones m*n (ancho x alto) que se ve
        // 0 1 2 3 .... m
        // n+1 n+2 .... 2m
        // .
        // .
        // ............ m*n
        // queda representada de la siguiente forma
        // 0, 1, 2, 3, ... m, m+1, m+2, ... m*n

        int sizeDst = jpegDataOri.width * jpegDataOri.height;   // ancho*alto (solo 1 canal)
        int sizeOri = sizeDst * jpegDataOri.ch;                 // ancho*alto*canales (varios canales)
        
        float factoresBN[]={.3,.59,.11};   // factores para conversion a BN
        
        /*
        //mascara laplaciano hardcodeada
        int lapMasc[3][3] = {   {0, 1, 0},
                                {1,-4, 1},
                                {0, 1, 0} };
                                */
        
        // mascara laplaciano desde archivo
        int lapMasc[3][3];
        getMask(lapMasc, archivoMascara);

        // factor para "suavizar"
        //float factor = 1.0/9.0;

        int cont=0;
        int i, j, ii, jj;
        int pixelXY = 0;


        // Escala de grises
        for (i=0; i<sizeOri; i+=jpegDataOri.ch) {
            if( i%jpegDataOri.ch == 0 ){
                for(j=0; j<jpegDataOri.ch; j++){
                    jpegDataDstBn.data[cont] += factoresBN[j]*(float)jpegDataOri.data[i+j] ;
                }
                cont++;
            }
        }
        
        // Laplaciano
        for(j=1; j<jpegDataDstBn.height-1; j++){      // filas desde la segunda (indice 1) hasta la penultima (indice n-1)
            for(i=1; i<jpegDataDstBn.width-1; i++){   // columnas desde la segunda (indice 1) hasta la penultima (indice m-1)

                for(jj=0; jj<3;jj++){
                    for(ii=0; ii<3; ii++){
                        pixelXY += lapMasc[jj][ii] * jpegDataDstBn.data[((i+ii)-1)+jpegDataDstBn.width*((j+jj)-1)];
                    }
                }
                //pixelXY = (int) ((float)pixelXY*factor);        // se aplica factor para suavizar
                pixelXY = pixelXY>255?255:pixelXY<0?0:pixelXY;  // se controlan sobrepasos
                jpegDataDstBnLap.data[i+jpegDataDstBn.width*j] = pixelXY;
                pixelXY = 0;
            }
        }

        // Umbral
        for(i=0; i<sizeDst; i++){
            jpegDataDstBnLapUm.data[i] = jpegDataDstBnLap.data[i]>nUmbralBin?255:0;
        }

        for(i=0; i<sizeDst; i++){
            jpegDataDst.data[i] = jpegDataDstBnLap.data[i]>nUmbralBin?255:0;
        }

        // Analisis "Nearly Black"
        int cuentaNegros = 0;   // en esta parte también se cuentan pixeles negros para clasificacion
        int tasaNegros = 0;     // tasa porcentual pixeles negros
        for(i=0; i<sizeDst; i++){
            if(jpegDataDstBnLap.data[i]<=nUmbralBin)
                cuentaNegros++;
        }
        tasaNegros = 100*cuentaNegros/sizeDst;

        // Impresion de info nearly black
        if(flag==1){
            //  Se calcula un porcentaje de pixeles negros, se compara con el valor recibido en parametro -n
            if(i_img==1)
                printf("|\tImagen\t\t|\tNearly Black\t|\n|-----------------------|-----------------------|\n");
            if( tasaNegros > nUmbralClas)
                printf("|\t%s\t|\t    si\t\t|\n", imgSrcName);
            else
                printf("|\t%s\t|\t    no\t\t|\n", imgSrcName);
        }

        ////////////////////////////////////////////////     ESCRITURAS

        // Escritura bn-laplaciano-umbral
        if (!write_jpeg(&jpegDataDst, imgDstPath, &jerr, JCS_GRAYSCALE)){
            free_jpeg(&jpegDataDst);
            return -1;
        }
        //printf("Write: %s\n", imgDstPath);
        free_jpeg(&jpegDataDst);

    }

    return 0;

}