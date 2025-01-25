
#include "sattex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t swap_endian_32(uint32_t value)
{
    return ((value & 0x000000FF) << 24) |
           ((value & 0x0000FF00) << 8) |
           ((value & 0x00FF0000) >> 8) |
           ((value & 0xFF000000) >> 24);
}

uint16_t swap_endian_16(uint16_t value)
{
    return (value >> 8) | (value << 8);
}

#ifdef _WIN32
// Win32 util
bool CreateSaturnTextureFile(const char *outFileName, uint8_t *bitmap, uint16_t *palette, BITMAPINFOHEADER *header)
{
    const uint16_t NAME_LEN = 260;
    if (outFileName == NULL || bitmap == NULL || palette == NULL || header == NULL)
    {
        return false;
    }

    if (strlen(outFileName) > (NAME_LEN - 5))
    {
        return false;
    }
    char fileTexOut[NAME_LEN];
    char filePalOut[NAME_LEN];
    sprintf(fileTexOut, "%s.tex", outFileName);
    sprintf(filePalOut, "%s.pal", outFileName);

    FILE *filePtr = fopen(fileTexOut, "wb");
    if (filePtr == NULL)
        return false;

    SatTexture satTex;
    SatPalette satPal;

    // Swap from LE to BE for saturn
    satTex.headerSize = swap_endian_32(sizeof(SatTexture));
    satTex.data_size = swap_endian_16(header->biSizeImage);
    satTex.width = swap_endian_16(header->biWidth);
    satTex.height = swap_endian_16(header->biHeight);

    fwrite(&satTex, sizeof(SatTexture), 1, filePtr);
    fwrite(bitmap, header->biSizeImage, 1, filePtr);

    fclose(filePtr);

    filePtr = fopen(filePalOut, "wb");
    if (filePtr == NULL)
        return false;

    satPal.headerSize = swap_endian_32(sizeof(SatPalette));
    satPal.data_size = swap_endian_16(header->biClrUsed * sizeof(uint16_t));

    // for(int it = 0; it < header->biClrUsed; ++it)
    //{
    //     uint16_t* swapBytes = (uint16_t*)palette;
    //     swapBytes[it] = swap_endian_16(swapBytes[it]);
    // }

    fwrite(&satPal, sizeof(SatPalette), 1, filePtr);
    fwrite(palette, header->biClrUsed, sizeof(uint16_t), filePtr);

    fclose(filePtr);

    return true;
}
#endif // _WIN32

bool ReadSaturnTexture(const uint8_t *texFileBytes, SatTexture *outHeader, uint8_t **outData)
{
    if(texFileBytes == NULL || outHeader == NULL || outData == NULL)
    {
        return false;
    }

    uint32_t cursor = 0;
    memcpy(outHeader, &texFileBytes[cursor], sizeof(SatTexture));
    #ifdef _WIN32
    // for validation swap back to LE when loading on windows.
    outHeader->headerSize = swap_endian_32(outHeader->headerSize);
    outHeader->data_size = swap_endian_16(outHeader->data_size);
    outHeader->width = swap_endian_16(outHeader->width);
    outHeader->height = swap_endian_16(outHeader->height);
    #endif
    cursor += outHeader->headerSize;

    (*outData) = malloc(outHeader->data_size);
    if((*outData) == NULL)
    {
        return false;
    }

    memcpy(*outData, &texFileBytes[cursor], outHeader->data_size);

    return true;
}

bool ReadSaturnPalette(const uint8_t *palFileBytes, SatPalette *outHeader, uint8_t **outData)
{
    if(palFileBytes == NULL || outHeader == NULL || outData == NULL)
    {
        return false;
    }

    uint32_t cursor = 0;
    memcpy(outHeader, &palFileBytes[cursor], sizeof(SatPalette));
    #ifdef _WIN32
    // for validation swap back to LE when loading on windows.
    outHeader->headerSize = swap_endian_32(outHeader->headerSize);
    outHeader->data_size = swap_endian_16(outHeader->data_size);
    #endif
    cursor += outHeader->headerSize;

    (*outData) = malloc(outHeader->data_size);
    if((*outData) == NULL)
    {
        return false;
    }

    memcpy(*outData, &outHeader[cursor], outHeader->data_size);

    return true;
}
