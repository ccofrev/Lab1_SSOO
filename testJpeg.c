/*******************************************************************************
 * 
 * jpegtest.c
 * 
 * Basado en desarrollo de Kenta Kuramochi publicado en
 * https://gist.github.com/kentakuramochi/f64e7646f1db8335c80f131be8359044
 * se compila con
 * gcc testJpeg.c -o testJpeg -std=c11 -ljpeg
 * 
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <jpeglib.h>

// estructura imagen JPEG
typedef struct {
    uint8_t *data;   // raw data
    uint32_t width;
    uint32_t height;
    uint32_t ch;     // color channels
} JpegData;


// Para reservar memoria para datos imagen
void alloc_jpeg(JpegData *jpegData)
{
    jpegData->data = NULL;
    jpegData->data = (uint8_t*) malloc(sizeof(uint8_t)  *
                                       jpegData->width  *
                                       jpegData->height *
                                       jpegData->ch);
}


// Para liberar memoria
void free_jpeg(JpegData *jpegData)
{
    if (jpegData->data != NULL) {
        free(jpegData->data);
        jpegData->data = NULL;
    }
}

// read JPEG image
// 1. create JPEG decompression object
// 2. specify source data
// 3. read JPEG header
// 4. set parameters
// 5. start decompression
// 6. scan lines
// 7. finish decompression
bool read_jpeg(JpegData *jpegData,
              const char *srcfile,
              struct jpeg_error_mgr *jerr)
{
    // 1.
    struct jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(srcfile, "rb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", srcfile);
        return false;
    }
    // 2.
    jpeg_stdio_src(&cinfo, fp);

    // 3.
    jpeg_read_header(&cinfo, TRUE);

    // 4. omitted
    // 5.
    jpeg_start_decompress(&cinfo);

    jpegData->width  = cinfo.image_width;
    jpegData->height = cinfo.image_height;
    jpegData->ch     = cinfo.num_components;

    alloc_jpeg(jpegData);

    // 6. read line by line
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_read_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 7.
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return true;
}

// write JPEG image
// 1. create JPEG compression object
// 2. specify destination data
// 3. set parameters
// 4. start compression
// 5. scan lines
// 6. finish compression
bool write_jpeg(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr,
                J_COLOR_SPACE jcs)
{
    // 1.
    struct jpeg_compress_struct cinfo;
    jpeg_create_compress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(dstfile, "wb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", dstfile);
        return false;
    }
    // 2.
    jpeg_stdio_dest(&cinfo, fp);

    // 3.
    cinfo.image_width      = jpegData->width;
    cinfo.image_height     = jpegData->height;
    cinfo.input_components = jpegData->ch;
    //cinfo.in_color_space   = JCS_RGBJCS_RGB;
    cinfo.in_color_space   = jcs;
    jpeg_set_defaults(&cinfo);

    // 4.
    jpeg_start_compress(&cinfo, TRUE);

    // 5.
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_write_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 6.
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);

    return true;
}



bool read_jpeg_bn(JpegData *jpegData,
              const char *srcfile,
              struct jpeg_error_mgr *jerr)
{
    // 1.
    struct jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(srcfile, "rb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", srcfile);
        return false;
    }
    // 2.
    jpeg_stdio_src(&cinfo, fp);

    // 3.
    jpeg_read_header(&cinfo, TRUE);

    // 4. omitted
    // 5.
    jpeg_start_decompress(&cinfo);

    jpegData->width  = cinfo.image_width;
    jpegData->height = cinfo.image_height;
    jpegData->ch     = cinfo.num_components;

    alloc_jpeg(jpegData);

    // 6. read line by line
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_read_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 7.
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return true;
}

bool write_jpeg_bn(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr)
{
    // 1.
    struct jpeg_compress_struct cinfo;
    jpeg_create_compress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(dstfile, "wb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", dstfile);
        return false;
    }
    // 2.
    jpeg_stdio_dest(&cinfo, fp);

    // 3.
    cinfo.image_width      = jpegData->width;
    cinfo.image_height     = jpegData->height;
    //cinfo.input_components = jpegData->ch;
    cinfo.input_components = 1;
    cinfo.in_color_space   = JCS_GRAYSCALE;
    jpeg_set_defaults(&cinfo);

    // 4.
    jpeg_start_compress(&cinfo, TRUE);

    // 5.
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_write_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 6.
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);

    return true;
}



int main(void){


    JpegData jpegDataOri, jpegDataDst, jpegDataDstBn, jpegDataDstBnLap, jpegDataDstBnLapUm;

    // 
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int UMBRAL = 100;

    // src/dst file
    char *src = "./img_test/lena_.jpg";
    char *dst = "./img_test/out.jpg";
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
    float arr[]={.3,.59,.11};   // factores para conversion a BN
    int lapMasc[3][3] = {   {1, 0, 1},
                            {0,-4, 0},
                            {1, 0, 1} };


    //float val = 0.0;
    float val;
    int suma = 0, cont=0;
    int i, j, ii, jj;
    int alto=0, ancho=0;
    int pixelXY = 0, pixelXYLap = 0;

    for (i=0; i<sizeOri; i+=jpegDataOri.ch) {

        jpegDataDst.data[i]= jpegDataOri.data[i] ;

        if( i%jpegDataOri.ch==0 ){
            jpegDataDstBn.data[cont]= arr[0]*(float)jpegDataOri.data[i] + arr[1]*(float)jpegDataOri.data[i] + arr[2]*(float)jpegDataOri.data[i] ;
            cont++;
        }
    }

    
    // Escritura
    /*
    if (!write_jpeg(&jpegDataDst, dst, &jerr, JCS_RGB)){
        free_jpeg(&jpegDataDst);
        return -1;
    }

    printf("Write: %s\n", dst);
    free_jpeg(&jpegDataDst);
    */

    // Escritura bn
    if (!write_jpeg(&jpegDataDstBn, dstBn, &jerr, JCS_GRAYSCALE)){
        free_jpeg(&jpegDataDstBn);
        return -1;
    }
    //printf("Write: %s\n", dstBn);
    //free_jpeg(&jpegDataDstBn);

    // recorrido imagen para laplaciano
    alto = jpegDataDstBn.height;
    ancho = jpegDataDstBn.width;


    for (j=1; j<alto-1; j++){     // filas desde la segunda (indice 1) hasta la penultima (indice n-1)
        for(i=1; i<ancho-1; i++){ // columnas desde la segunda (indice 1) hasta la penultima (indice m-1)
            
            for(jj=-1; jj<=1;jj++){
                for(ii=-1; ii<=1; ii++){
                    pixelXY += lapMasc[jj+1][ii+1] * jpegDataDstBn.data[(i+ii)+ancho*(j+jj)];
                }
            }
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