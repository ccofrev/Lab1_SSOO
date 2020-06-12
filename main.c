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

    /// Parametros recibidos por consola
	int nArchivos = 0;      // -c cantidad de imagenes
    int nUmbralBin = 0;     // -u umbral para binarizar imagen
    int nUmbralClas = 0;    // -n umbral para clasificacion nearly black    
    char archivoMascara[20];// -m nombre archivo mascara
    int flag = 0;           // -b bandera para resultado clasificacion en pantalla

    // se reciben parametros
    recibirArgumentos(argc, argv, &nArchivos, &nUmbralBin, &nUmbralClas, archivoMascara, &flag);

    // Se inician estructuras de datos para imagenes a usar
    JpegData jpegDataOri, jpegDataDst, jpegDataDstBn, jpegDataDstBnLap, jpegDataDstBnLapUm;

    //struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int UMBRAL = nUmbralBin;

    // src/dst file
    char src[30];
    char imgStr[10];
    
    sprintf(imgStr, "imagen_%d", nArchivos);
    sprintf(src, "./img_test/%s.jpg", imgStr);

    char *dstBn = "./img_test/outBn.jpg";
    char *dstBnLap = "./img_test/outBnLap.jpg";
    char *dstBnLapUm = "./img_test/outBnLapUm.jpg";

    if (!read_jpeg(&jpegDataOri, src, &jerr)){
        free_jpeg(&jpegDataOri);
        return -1;
    }

    //printf("Read:  %s\n", src);

    if (!read_jpeg(&jpegDataDst, src, &jerr)){
        free_jpeg(&jpegDataDst);
        return -1;
    }

    if (!read_jpeg(&jpegDataDstBn, src, &jerr)){
        free_jpeg(&jpegDataDstBn);
        return -1;
    }
    jpegDataDstBn.ch = 1; // se fuerza 1 canal en el destino BN

    if (!read_jpeg(&jpegDataDstBnLap, src, &jerr)){
        free_jpeg(&jpegDataDstBnLap);
        return -1;
    }
    jpegDataDstBnLap.ch = 1; // se fuerza 1 canal en el destino BN

    if (!read_jpeg(&jpegDataDstBnLapUm, src, &jerr)){
        free_jpeg(&jpegDataDstBnLapUm);
        return -1;
    }
    jpegDataDstBnLapUm.ch = 1; // se fuerza 1 canal en el destino BN

    //printf("Read:  %s\n", src);

    // se propone una imagen guardada en un solo arreglo
    // así, una imagen de dimensiones m*n (ancho x alto) que se ve
    // 0 1 2 3 .... m
    // n+1 n+2 .... 2m
    // .
    // .
    // ............ m*n
    // queda representada de la siguiente forma
    // 0, 1, 2, 3, ... m+1, m+2, ... m*n
    
    // reverse all bits

    int sizeDst = jpegDataOri.width * jpegDataOri.height;   // ancho*alto (solo 1 canal)
    int sizeOri = sizeDst * jpegDataOri.ch;                 // ancho*alto*canales
    
    float factoresBN[]={.3,.59,.11};   // factores para conversion a BN
   
    
    /*
    int lapMasc[3][3] = {   {1, 1, 1},
                            {1,-8, 1},
                            {1, 1, 1} };
                            */
    
    int lapMasc[3][3];
    getMask(lapMasc, archivoMascara);

    //float factor = 1.0/9.0;
    float factor = 1.0;


    //float val = 0.0;
    int cont=0;
    int i, j, ii, jj;
    int alto=0, ancho=0;
    int pixelXY = 0;

    // Blanco y negro
    for (i=0; i<sizeOri; i+=jpegDataOri.ch) {
        jpegDataDst.data[i]= jpegDataOri.data[i] ;
        if( i%jpegDataOri.ch == 0 ){
            jpegDataDstBn.data[cont] = factoresBN[0]*(float)jpegDataOri.data[i] + factoresBN[1]*(float)jpegDataOri.data[i] + factoresBN[2]*(float)jpegDataOri.data[i] ;
            cont++;
        }
    }

    // recorrido imagen para laplaciano
    alto = jpegDataDstBn.height;
    ancho = jpegDataDstBn.width;
    
    // se limpia para test
    for(j=1; j<alto-1; j++){       // filas desde la segunda (indice 1) hasta la penultima (indice n-1)
        for(i=1; i<ancho-1; i++){   // columnas desde la segunda (indice 1) hasta la penultima (indice m-1)

            for(jj=0; jj<3;jj++){
                for(ii=0; ii<3; ii++){
                    pixelXY += lapMasc[jj][ii] * jpegDataDstBn.data[((i+ii)-1)+ancho*((j+jj)-1)];
                }
            }
            pixelXY = (int) ((float)pixelXY*factor);
            pixelXY = pixelXY>255?255:pixelXY<0?0:pixelXY;  // se controlan sobrepasos
            jpegDataDstBnLap.data[i+ancho*j] = pixelXY;
            pixelXY = 0;
        }
    }



    // umbral
    int cuentaNegros = 0;
    for(i=0; i<sizeDst; i++){
        //jpegDataDstBnLapUm.data[i] = jpegDataDstBnLap.data[i]>UMBRAL?255:0;
        if(jpegDataDstBnLap.data[i]>UMBRAL){
            jpegDataDstBnLapUm.data[i] = 255;
        }else{
            jpegDataDstBnLapUm.data[i] = 0;
            cuentaNegros++;
        }
    }

    
    if(flag==1){
        //  Se calcula un porcentaje de pixeles negros, se compara con el valor recibido en parametro -n
        printf("|\tImagen\t\t|\tNearly Black\t|\n|-----------------------|-----------------------|\n");
        if(100*cuentaNegros/sizeDst > nUmbralClas)
            printf("|\t%s\t|\t    si\t\t|\n\n", imgStr);
        else
            printf("|\t%s\t|\t    no\t\t|\n\n", imgStr);
    }


    ////////////////////////////////////////////////     ESCRITURAS
    // Escritura bn
    if (!write_jpeg(&jpegDataDstBn, dstBn, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBn);
        return -1;
    }
    //printf("Write: %s\n", dstBn);
    free_jpeg(&jpegDataDstBn);


    // Escritura bn-laplaciano
    if (!write_jpeg(&jpegDataDstBnLap, dstBnLap, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBnLap);
        return -1;
    }
    //printf("Write: %s\n", dstBnLap);
    free_jpeg(&jpegDataDstBnLap);
    

    // Escritura bn-laplaciano-umbral
    if (!write_jpeg(&jpegDataDstBnLapUm, dstBnLapUm, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBnLapUm);
        return -1;
    }
    //printf("Write: %s\n", dstBnLapUm);
    free_jpeg(&jpegDataDstBnLapUm);
       
    return 0;
}