#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

bool LoadBitmapFile(const uint8_t *bitmapFile, uint8_t **outBitmap, uint16_t **outColors, BITMAPINFOHEADER *outHeader)
{
    BITMAPFILEHEADER bitmapFileHeader; // our bitmap file header
    // BITMAPINFOHEADER bitmapInfoHeader;
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

    // allocate enough memory for the bitmap image data
    (*outBitmap) = malloc(outHeader->biSizeImage);

    // verify memory allocation
    if (*outBitmap == NULL)
    {
        // TODO: figure out a non const container so we can free this.
        free(*outColors);
        (*outColors) = NULL;
        return false;
    }

    // read in the bitmap image data
    // fread(bitmapImage,outHeader->biSizeImage,1,filePtr);
    memcpy(*outBitmap, &bitmapFile[cursor], outHeader->biSizeImage);
    return true;
}
