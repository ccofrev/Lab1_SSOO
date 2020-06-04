#ifndef JPEG_H
#define JPEG_H

// estructura imagen JPEG
typedef struct {
    uint8_t *data;   // raw data
    uint32_t width;
    uint32_t height;
    uint32_t ch;     // color channels
} JpegData;


bool read_jpeg(JpegData *jpegData,
              const char *srcfile,
              struct jpeg_error_mgr *jerr);

bool write_jpeg(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr,
                J_COLOR_SPACE jcs);

void free_jpeg(JpegData *jpegData);

#endif
