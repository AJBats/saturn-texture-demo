/*
 sattex.h - Very simple saturn texture/pallete file types.
*/

#ifndef SATTEX_H
#define SATTEX_H

#include <stdint.h>

#ifdef _WIN32
#include "bitmap.h"
#else
#include <stdbool.h>
#endif 


#pragma pack(push, 1)

typedef struct tagSatTexture
{
    uint32_t headerSize; // size of this header, doubles as version control.
    uint16_t data_size;
    uint16_t width;
    uint16_t height;
} SatTexture;

typedef struct tagSatPalette
{
    uint32_t headerSize; // size of this header, doubles as version control.
    uint16_t data_size;
} SatPalette;

#pragma pack(pop)

#ifdef _WIN32
// Win32 util
bool CreateSaturnTextureFile(const char *outFileName, uint8_t *bitmap, uint16_t *palette, BITMAPINFOHEADER *header);
#endif // _WIN32

// Saturn runtime
bool ReadSaturnTexture(const uint8_t *texFileBytes, SatTexture* outHeader, uint8_t **outData);
bool ReadSaturnPalette(const uint8_t *palFileBytes, SatPalette* outHeader, uint8_t **outData);


#endif // SATTEX_H