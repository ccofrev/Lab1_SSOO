#ifndef JPEG_H
#define JPEG_H

// estructura imagen JPEG
typedef struct {
    uint8_t *data;      // arreglo datos
    uint32_t width;     //ancho
    uint32_t height;    //alto
    uint32_t ch;        // canales
} JpegData;

// reserva de memoria para estructura imagen
void init_jpeg(JpegData *jpegData, unsigned int image_width, unsigned int image_height, int num_components);

// lectura imagen archivo
bool read_jpeg(JpegData *jpegData,
              const char *srcfile,
              struct jpeg_error_mgr *jerr);

// escritura imagen archivo
bool write_jpeg(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr,
                J_COLOR_SPACE jcs);

// liberacion de memoria estructura imagen
void free_jpeg(JpegData *jpegData);

#endif
