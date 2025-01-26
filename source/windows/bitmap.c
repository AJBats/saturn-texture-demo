#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

uint8_t *ProcessImageData(const uint8_t *bmpImageData, BITMAPINFOHEADER *outHeader)
{
    int paddedWidthSize = (outHeader->biWidth + 3) & ~3; // Align to 4 bytes
    int packedImageSize = outHeader->biWidth * outHeader ->biHeight;

    // change the header to report the packed size from now on.
    outHeader->biSizeImage = packedImageSize;
    
    uint8_t* imageData = malloc(packedImageSize);

    if(!imageData) {
        return NULL;
    }

    // Remove padding row by row
    for (int row = 0; row < outHeader ->biHeight; row++) {
        // Copy only the valid pixel data from each row
        memcpy(imageData + (row * outHeader->biWidth), bmpImageData + (row * paddedWidthSize), outHeader->biWidth);
    }
    return imageData;
}

bool LoadBitmapFile(const uint8_t *bitmapFile, uint8_t **outBitmap, uint16_t **outColors, BITMAPINFOHEADER *outHeader)
{
    BITMAPFILEHEADER bitmapFileHeader; // our bitmap file header
    int cursor = 0;

    // outputs are not optional
    if (bitmapFile == NULL || outBitmap == NULL || outColors == NULL || outHeader == NULL)
    {
        return false;
    }

    // read the bitmap file header
    memcpy(&bitmapFileHeader, &bitmapFile[cursor], sizeof(BITMAPFILEHEADER));
    cursor += sizeof(BITMAPFILEHEADER);

    // verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType != 0x4D42)
    {
        return false;
    }

    // read the bitmap info header
    memcpy(outHeader, &bitmapFile[cursor], sizeof(BITMAPINFOHEADER));
    // special cursor bump past the header size
    cursor += outHeader->biSize;

    // palette reading method 1:
    // set the number of colors
    outHeader->biClrUsed = outHeader->biClrUsed < 256 ? outHeader->biClrUsed : 256;

    // load the palette for 8 bits per pixel
    if (outHeader->biBitCount == 8)
    {
        RGBQUAD colors[256] = {0};
        memcpy(colors, &bitmapFile[cursor], outHeader->biClrUsed * sizeof(RGBQUAD));
        cursor += outHeader->biClrUsed * sizeof(RGBQUAD);

        (*outColors) = (uint16_t *)malloc(outHeader->biClrUsed * sizeof(uint16_t));
        if (*outColors)
        {
            for (int colorIt = 0; colorIt < outHeader->biClrUsed; ++colorIt)
            {
                RGB555 oneColor;

                oneColor.rgbBlue = (colors[colorIt].rgbBlue >> 3);
                oneColor.rgbGreen = (colors[colorIt].rgbGreen >> 3);
                oneColor.rgbRed = (colors[colorIt].rgbRed >> 3);
                oneColor.rgbReserved = 0;

                (*outColors)[colorIt] = oneColor.rgbBlue |
                           (oneColor.rgbGreen << 5) |
                           (oneColor.rgbRed << 10) |
                           (oneColor.rgbReserved << 15);
            }
        }
        else
        {
            return false;
        }
    }

    // move file pointer to the beginning of bitmap data
    // fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    cursor = bitmapFileHeader.bfOffBits;

    (*outBitmap) = ProcessImageData(&bitmapFile[cursor], outHeader);

    // verify memory allocation
    if (*outBitmap == NULL)
    {
        free(*outColors);
        (*outColors) = NULL;
        return false;
    }

    return true;
}
