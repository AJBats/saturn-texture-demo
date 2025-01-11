#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

int LoadBitmapFile(const uint8_t *bitmapFile, picture_t *outBitmap, palette_t *outColors)
{
    BITMAPFILEHEADER bitmapFileHeader; // our bitmap file header
    BITMAPINFOHEADER bitmapInfoHeader;
    int cursor = 0;

    // read the bitmap file header
    memcpy(&bitmapFileHeader, &bitmapFile[cursor], sizeof(BITMAPFILEHEADER));
    cursor += sizeof(BITMAPFILEHEADER);

    // verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType != 0x4D42)
    {
        return 0;
    }

    // read the bitmap info header
    memcpy(&bitmapInfoHeader, &bitmapFile[cursor], sizeof(BITMAPINFOHEADER));
    // special cursor bump past the header size
    cursor += bitmapInfoHeader.biSize;

    // palette reading method 1:
    // set the number of colors
    int numColors = bitmapInfoHeader.biClrUsed < 256 ? bitmapInfoHeader.biClrUsed : 256;

    // load the palette for 8 bits per pixel
    if (bitmapInfoHeader.biBitCount == 8)
    {
        RGBQUAD colors[256] = {0};
        memcpy(colors, &bitmapFile[cursor], numColors * sizeof(RGBQUAD));
        cursor += numColors * sizeof(RGBQUAD);

        RGB555 *outColors555 = (RGB555 *)malloc(numColors * sizeof(RGB555));
        if (outColors555)
        {
            for (int colorIt = 0; colorIt < numColors; ++colorIt)
            {
                outColors555[colorIt].rgbBlue = (colors[colorIt].rgbBlue >> 3);
                outColors555[colorIt].rgbGreen = (colors[colorIt].rgbGreen >> 3);
                outColors555[colorIt].rgbRed = (colors[colorIt].rgbRed >> 3);
                outColors555[colorIt].rgbReserved = 0;
            }

            outColors->data = (void *)outColors555;
            outColors->data_size = numColors * sizeof(RGB555);
        }
        else
        {
            return 1;
        }
    }

    // move file pointer to the beginning of bitmap data
    // fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    cursor = bitmapFileHeader.bfOffBits;

    // allocate enough memory for the bitmap image data
    BYTE *bitmapData = malloc(bitmapInfoHeader.biSizeImage);

    // verify memory allocation
    if (!bitmapData)
    {
        // TODO: figure out a non const container so we can free this.
        // free(outColors->data);
        outColors->data = NULL;
        return 2;
    }

    // read in the bitmap image data
    // fread(bitmapImage,bitmapInfoHeader.biSizeImage,1,filePtr);
    memcpy(bitmapData, &bitmapFile[cursor], bitmapInfoHeader.biSizeImage);

    outBitmap->data = bitmapData;
    outBitmap->data_size = bitmapInfoHeader.biSizeImage;
    outBitmap->width = bitmapInfoHeader.biWidth;
    outBitmap->height = bitmapInfoHeader.biHeight;
    return 3;
}
