#include "bitmap.h"
#include <malloc.h>
#include <string.h>

BYTE* LoadBitmapFile(const uint8_t* bitmapFile, int bitmapSize, BITMAPINFOHEADER *bitmapInfoHeader)
{
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable
    int cursor = 0;

    //read the bitmap file header
    memcpy(&bitmapFileHeader, &bitmapFile[cursor], sizeof(BITMAPFILEHEADER));
    cursor += sizeof(BITMAPFILEHEADER);

    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        return NULL;
    }

    //read the bitmap info header
    memcpy(bitmapInfoHeader, &bitmapFile[cursor], sizeof(BITMAPINFOHEADER));
    cursor += sizeof(BITMAPINFOHEADER);

    // palette reading method 1:
    //set the number of colours
    int numColours=1 << bitmapInfoHeader->biBitCount;
    RGBQUAD* colours = NULL;

    //load the palette for 8 bits per pixel
    if(bitmapInfoHeader->biBitCount == 8) {       
        colours=(RGBQUAD*)malloc(numColours * sizeof(RGBQUAD));
        memcpy(colours, &bitmapFile[cursor], numColours * sizeof(RGBQUAD));
        cursor += numColours * sizeof(RGBQUAD);
    }
    /*
    // method 2:
     //NOW LOAD THE COLOR PALETTE IF THERE IS ONE
        if(bitmap->bitmapinfoheader.biBitCount == 8)
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
    bitmapImage = (BYTE*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        free(colours);
        return NULL;
    }

    //read in the bitmap image data
    //fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);
    memcpy(bitmapImage, &bitmapFile[cursor], bitmapInfoHeader->biSizeImage);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        free(bitmapImage);
        free(colours);
        return NULL;
    }

    //swap the R and B values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap image data
    return bitmapImage;
}
