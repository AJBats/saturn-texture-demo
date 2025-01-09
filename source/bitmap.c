#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

bool LoadBitmapFile(const uint8_t* bitmapFile, BYTE** outBitmap, RGBQUAD** outColors, RGB555** outColors555, BITMAPINFOHEADER *outBitmapInfoHeader)
{
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    int cursor = 0;

    if(outColors) *outColors = NULL;
    if(outColors555) *outColors555 = NULL;

    //read the bitmap file header
    memcpy(&bitmapFileHeader, &bitmapFile[cursor], sizeof(BITMAPFILEHEADER));
    cursor += sizeof(BITMAPFILEHEADER);

    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        return NULL;
    }

    //read the bitmap info header
    memcpy(outBitmapInfoHeader, &bitmapFile[cursor], sizeof(BITMAPINFOHEADER));
    cursor += outBitmapInfoHeader->biSize;

    // palette reading method 1:
    //set the number of colors
    int numColors = outBitmapInfoHeader->biClrUsed;

    //load the palette for 8 bits per pixel
    if(outBitmapInfoHeader->biBitCount == 8) {    
        if(outColors) {
            *outColors=(RGBQUAD*)malloc(numColors * sizeof(RGBQUAD));
            memcpy(*outColors, &bitmapFile[cursor], numColors * sizeof(RGBQUAD));
            cursor += numColors * sizeof(RGBQUAD);
        } else if(outColors555) {
            RGBQUAD colors[265] = {0};
            int numColorsSize = numColors * sizeof(RGBQUAD);
            int colorSize = (numColorsSize < sizeof(colors)) ? numColorsSize : sizeof(colors);
            memcpy(colors, &bitmapFile[cursor], colorSize);
            cursor += numColors * sizeof(RGBQUAD);

            *outColors555=(RGB555*)malloc(numColors * sizeof(RGB555));
            for(int colorIt = 0; colorIt < numColors; ++colorIt) {
                (*outColors555)[colorIt].rgbBlue = (colors[colorIt].rgbBlue >> 3);
                (*outColors555)[colorIt].rgbGreen = (colors[colorIt].rgbGreen >> 3);
                (*outColors555)[colorIt].rgbRed = (colors[colorIt].rgbRed >> 3);
                (*outColors555)[colorIt].rgbReserved = 0;
            }
        }
        
    }
    /*
    // method 2:
     //NOW LOAD THE COLOR PALETTE IF THERE IS ONE
        if(bitmap->outBitmapInfoHeader.biBitCount == 8)
        {
                _lread(file_handle, &bitmap->palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
 
                //Now set all the flags in the palette correctly and fix the reversed BGR RGBQUAD
                for(i = 0; i < MAX_COLORS_PALETTE; i++)
                {
                        int temp_color = bitmap->palette[i].peRed;
                        bitmap->palette[i].peRed = bitmap->palette[i].peBlue;
                        bitmap->palette[i].peBlue = temp_color;
 
                        bitmap->palette[i].peFlags = PC_NOCOLLAPSE;
                }
        }
    */

    //move file pointer to the beginning of bitmap data
    //fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    cursor = bitmapFileHeader.bfOffBits;

    //allocate enough memory for the bitmap image data
    *outBitmap = (BYTE*)malloc(outBitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!*outBitmap)
    {
        free(*outBitmap);
        if(outColors) {
            free(*outColors);
            *outColors = NULL;
        }
        if(outColors555) {
            free(*outColors555);
            *outColors555 = NULL;
        }
        *outBitmap = NULL;
        return false;
    }

    //read in the bitmap image data
    //fread(bitmapImage,outBitmapInfoHeader->biSizeImage,1,filePtr);
    memcpy(*outBitmap, &bitmapFile[cursor], outBitmapInfoHeader->biSizeImage);

    //swap the R and B values to get RGB (bitmap is BGR)
    //for (imageIdx = 0;imageIdx < outBitmapInfoHeader->biSizeImage;imageIdx+=3)
    //{
    //    tempRGB = *outBitmap[imageIdx];
    //    *outBitmap[imageIdx] = *outBitmap[imageIdx + 2];
    //    *outBitmap[imageIdx + 2] = tempRGB;
    //}

    //close file and return bitmap image data
    return true;
}
