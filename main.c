/*
 * 
 * se compila con
 * gcc main.c -o lab1 -std=c11 -ljpeg
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <jpeglib.h>                                                                    
#include "funciones.h"
#include "jpeg.h"


int main(int argc, char *argv[]){

	int nArchivos = 0, nUmbralBin = 0, nUmbralClas = 0;
    char *mask;
    int flag = 0;
    recibirArgumentos(argc, argv, &nArchivos, &nUmbralBin, &nUmbralClas, mask, &flag);

    JpegData jpegDataOri, jpegDataDst, jpegDataDstBn, jpegDataDstBnLap, jpegDataDstBnLapUm;

    // 
    //struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int UMBRAL = 50;
    bool factorOTope = true;

    // src/dst file
    char *src = "./img_test/inv.jpg";
    //char *dst = "./img_test/out.jpg";
    char *dstBn = "./img_test/outBn.jpg";
    char *dstBnLap = "./img_test/outBnLap.jpg";
    char *dstBnLapUm = "./img_test/outBnLapUm.jpg";

    if (!read_jpeg(&jpegDataOri, src, &jerr)){
        free_jpeg(&jpegDataOri);
        return -1;
    }

    printf("Read:  %s\n", src);


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
    int sizeOri = jpegDataOri.width * jpegDataOri.height * jpegDataOri.ch;
    float factoresBN[]={.3,.59,.11};   // factores para conversion a BN
    int lapMasc[3][3] = {   {0, 1, 0},
                            {1,-4, 1},
                            {0, 1, 0} };
    float factor = 1.0/9.0;


    //float val = 0.0;
    int cont=0;
    int i, j, ii, jj;
    int alto=0, ancho=0;
    int pixelXY = 0;

    // Blanco y negro
    for (i=0; i<sizeOri; i+=jpegDataOri.ch) {

        jpegDataDst.data[i]= jpegDataOri.data[i] ;

        if( i%jpegDataOri.ch==0 ){
            jpegDataDstBn.data[cont]= factoresBN[0]*(float)jpegDataOri.data[i] + factoresBN[1]*(float)jpegDataOri.data[i] + factoresBN[2]*(float)jpegDataOri.data[i] ;
            cont++;
        }
    }

       


    // recorrido imagen para laplaciano
    alto = jpegDataDstBn.height;
    ancho = jpegDataDstBn.width;

    // se limpia para test          // umbral
    for (j=1; j<alto-1; j++){       // filas desde la segunda (indice 1) hasta la penultima (indice n-1)
        for(i=1; i<ancho-1; i++){   // columnas desde la segunda (indice 1) hasta la penultima (indice m-1)
            for(jj=-1; jj<=1;jj++){
                for(ii=-1; ii<=1; ii++){
                    pixelXY += lapMasc[jj+1][ii+1] * jpegDataDstBn.data[(i+ii)+ancho*(j+jj)];
                }
            }

            if(factorOTope)
                pixelXY = (int) ((float)pixelXY*factor);
            else
                pixelXY = pixelXY<0?0 : pixelXY>255?255 : pixelXY;

            jpegDataDstBnLap.data[i+ancho*j] = pixelXY;
            pixelXY = 0;
        }
        //printf("\n");
    }

    // umbral
    for(i=0; i<ancho*alto; i++){
        jpegDataDstBnLapUm.data[i] = jpegDataDstBnLap.data[i]>UMBRAL?255:0;
    }


    ///////////////////////////////     ESCRITURAS

    // Escritura bn
    if (!write_jpeg(&jpegDataDstBn, dstBn, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBn);
        return -1;
    }
    printf("Write: %s\n", dstBn);
    free_jpeg(&jpegDataDstBn);


    // Escritura bn-laplaciano
    if (!write_jpeg(&jpegDataDstBnLap, dstBnLap, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBnLap);
        return -1;
    }
    printf("Write: %s\n", dstBnLap);
    free_jpeg(&jpegDataDstBnLap);
    

    // Escritura bn-laplaciano-umbral
    if (!write_jpeg(&jpegDataDstBnLapUm, dstBnLapUm, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBnLapUm);
        return -1;
    }
    printf("Write: %s\n", dstBnLapUm);
    free_jpeg(&jpegDataDstBnLapUm);
       
    return 0;
}