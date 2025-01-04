#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

int main(int argc, char **argv)
{
    //open file in read binary mode
    FILE* filePtr = fopen("vdp1-mic3d/assets/test.bmp","rb");
    if (filePtr == NULL)
        return 0;
    
    fseek(filePtr, 0, SEEK_END);
    long fsize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);  /* same as rewind(f); */

    uint8_t* bitmapMem = malloc(fsize);
    fread(bitmapMem, fsize, 1, filePtr);
    fclose(filePtr);

    BITMAPINFOHEADER header;
    memset(&header, 0, sizeof(header));
    BYTE* bitmap = LoadBitmapFile(bitmapMem, fsize, &header);
	printf("Hello world");


	return 0;
}
