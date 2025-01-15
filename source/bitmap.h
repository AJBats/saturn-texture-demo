#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdbool.h>

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    uint16_t bfType;      // specifies the file type
    uint32_t bfSize;     // specifies the size in bytes of the bitmap file
    uint16_t bfReserved1; // reserved; must be 0
    uint16_t bfReserved2; // reserved; must be 0
    uint32_t bfOffBits;  // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;         // specifies the number of bytes required by the struct
    int32_t biWidth;         // specifies width in pixels
    int32_t biHeight;        // specifies height in pixels
    uint16_t biPlanes;        // specifies the number of color planes, must be 1
    uint16_t biBitCount;      // specifies the number of bits per pixel
    uint32_t biCompression;  // specifies the type of compression
    uint32_t biSizeImage;    // size of image in bytes
    int32_t biXPelsPerMeter; // number of pixels per meter in x axis
    int32_t biYPelsPerMeter; // number of pixels per meter in y axis
    uint32_t biClrUsed;      // number of colors used by the bitmap
    uint32_t biClrImportant; // number of colors that are important
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} RGBQUAD;

typedef struct tagRGB555
{
    uint8_t rgbBlue : 5;
    uint8_t rgbGreen : 5;
    uint8_t rgbRed : 5;
    uint8_t rgbReserved : 1;
} RGB555;

#pragma pack(pop)

bool LoadBitmapFile(const uint8_t *bitmapFile, uint8_t **outBitmap, uint16_t **outColors, BITMAPINFOHEADER* outHeader);

#endif // BITMAP_H
