#ifndef WINYAUL_TYPES_H
#define WINYAUL_TYPES_H

#include <cstdint>

typedef struct picture {
    const void *data;
    uint16_t data_size;
    uint16_t width;
    uint16_t height;
    uint16_t palette_index;
} picture_t;

typedef struct palette {
    const void *data;
    uint16_t data_size;
} palette_t;

#endif // WINYAUL_TYPES_H