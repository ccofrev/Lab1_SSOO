/*******************************************************************************
 jpegtest.c
 $ gcc jpegtest.c -std=c11 -ljpeg

 Basado en desarrollo de Kenta Kuramochi publicado en
 https://gist.github.com/kentakuramochi/f64e7646f1db8335c80f131be8359044

 se compila con
 gcc testJpeg.c -o testJpeg -std=c11 -ljpeg
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
    cinfo.input_components = jpegData->ch;
    cinfo.in_color_space   = JCS_RGB;
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







bool write_jpegBN(const JpegData *jpegData,
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
    cinfo.input_components = jpegData->ch;
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



int main(void)
{
    JpegData jpegData, jpegDataDst;

    // 
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // src/dst file
    char *src = "./img_test/lena.jpg";
    char *dst = "./img_test/out.jpg";

    if (!read_jpeg(&jpegData, src, &jerr)){
        free_jpeg(&jpegData);
        return -1;
    }
    printf("Read:  %s\n", src);

    
    if (!read_jpeg(&jpegDataDst, src, &jerr)){
        free_jpeg(&jpegDataDst);
        return -1;
    }
    printf("ReadDst:  %s\n", src);
    
   
    // reverse all bits
    int size = jpegData.width * jpegData.height * jpegData.ch;
    
    for (int i = 0; i < size; i++) {
        jpegData.data[i] = ~jpegData.data[i];

    }

    /*
    float arr[]={.3,.59,.11};
    //float val = 0.0;
    float val;
    int suma = 0;
    int j;
    for (int i=0; i<size; i+=jpegData.ch) {


       for(j=0; j<jpegData.ch; j++){
           val += (float)jpegData.data[i+j]*arr[j];

           //jpegDataDst.data[i+j] = (int)(float)jpegData.data[i+j]*arr[j];
           
           
           
       }
       jpegDataDst.data[i] = (int)val;
       
        //printf("%s", val<100.0?"#":" ");
        //val = 0.0;
        //if( ((i+jpegData.ch)%jpegData.width)==0 )
        //printf("\n");
       

    }

    
    for(int y=0; y<jpegData.height; y++){
      for(int x=0; x<jpegData.width; x++){
        for(int ch=0; ch<jpegData.ch; ch++){
          //printf("%03d, ", jpegData.data[y*jpegData.width+x+ch]);
          val += arr[ch]*(float)(jpegData.data[y*jpegData.width+x+ch]);
          //printf("%03d, ", jpegData.data[y*jpegData.width+x*3+ch]);
          //printf("%3d ", y*jpegData.height+x*jpegData.ch+ch);
          //printf("%.1f,", val);
          }
        //printf("%3d ", y*jpegData.height+x);
        printf("%c ", val>100.0?'_':'#');
        //printf("%s",val<100.0?" # ":" _ ");
        //printf("%3d ", (int)val);
        val = 0.0;
        //printf(" ");
      }
      printf("\n");
    }*/

    



    if (!write_jpeg(&jpegDataDst, dst, &jerr)){
        free_jpeg(&jpegDataDst);
        return -1;
    }
    printf("Write: %s\n", dst);

    free_jpeg(&jpegDataDst);

    return 0;
}