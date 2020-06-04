#ifndef HERRAMIENTAS_H
#define HERRAMIENTAS_H

// estructura imagen JPEG
typedef struct {
    uint8_t *data;   // raw data
    uint32_t width;
    uint32_t height;
    uint32_t ch;     // color channels
} JpegData;


void recibirArgumentos(int argc, char *argv[], int *numHijos, int *flag);
#endif
