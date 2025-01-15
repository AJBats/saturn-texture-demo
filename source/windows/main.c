#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "sattex.h"

bool testSaturnTexFile()
{
    // open file in read binary mode
    FILE *filePtr = fopen("assets/test.tex", "rb");
    if (filePtr == NULL)
        return 0;

    fseek(filePtr, 0, SEEK_END);
    long fsize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET); /* same as rewind(f); */

    uint8_t *texFile = malloc(fsize);
    fread(texFile, fsize, 1, filePtr);
    fclose(filePtr);

    SatTexture satTexture;
    uint8_t* satTexData = NULL;
    bool result = ReadSaturnTexture(texFile, &satTexture, &satTexData);

    free(satTexData);
    return result;
}

bool testSaturnPalFile()
{
    // open file in read binary mode
    FILE *filePtr = fopen("assets/test.pal", "rb");
    if (filePtr == NULL)
        return 0;

    fseek(filePtr, 0, SEEK_END);
    long fsize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET); /* same as rewind(f); */

    uint8_t *palFile = malloc(fsize);
    fread(palFile, fsize, 1, filePtr);
    fclose(filePtr);

    SatPalette satTexture;
    uint8_t* satPalData = NULL;
    bool result = ReadSaturnPalette(palFile, &satTexture, &satPalData);

    free(satPalData);
    return result;
}

int main(int argc, char **argv)
{
    // open file in read binary mode
    FILE *filePtr = fopen("assets/test.bmp", "rb");
    if (filePtr == NULL)
        return 0;

    fseek(filePtr, 0, SEEK_END);
    long fsize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET); /* same as rewind(f); */

    uint8_t *bitmapMem = malloc(fsize);
    fread(bitmapMem, fsize, 1, filePtr);
    fclose(filePtr);

    BITMAPINFOHEADER header;
    memset(&header, 0, sizeof(header));
    uint8_t *bitmap = NULL;
    uint16_t *colors555 = NULL;
    bool result = LoadBitmapFile(bitmapMem, &bitmap, &colors555, &header);
    printf("Result %s", result ? "true" : "false");

    free(bitmapMem);

    if (result)
    {
        result = CreateSaturnTextureFile("assets/test", bitmap, colors555, &header);
        printf("Result %s", result ? "true" : "false");

        if (result)
        {
            result = testSaturnTexFile();
            printf("Result %s", result ? "true" : "false");

            result = testSaturnPalFile();
            printf("Result %s", result ? "true" : "false");
        }
    }

    free(bitmap);
    free(colors555);

    return 0;
}
