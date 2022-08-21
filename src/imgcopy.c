#include <stdio.h>
#include <stdlib.h>

#define BMP_HEADER_SIZE 54
#define BMP_COLOR_TABLE_SIZE 1024

#define byte unsigned char

struct ImgSize
{
    int height;
    int width;
};

typedef struct ImgSize ImgSize;

ImgSize getImgSize(FILE *streamIn);

void imageReader(
    FILE *streamIn,
    const ImgSize *imgSize,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer);

void imageWritter(
    FILE *streamOut,
    const ImgSize *imgSize,
    byte *header,
    byte *colorTable,
    byte *buffer,
    int bitDepth);

int main()
{
    int width, height, bitDepth;

    byte header[BMP_HEADER_SIZE];
    byte colorTable[BMP_COLOR_TABLE_SIZE];
    byte buffer[1024 * 1024];

    const char imgName[] = "images/man.bmp";
    const char copyImgName[] = "images/man_copy.bmp";

    FILE *streamIn = fopen(imgName, "rb");
    FILE *streamOut = fopen(copyImgName, "wb");

    if (streamIn == (FILE *)0)
    {
        printf("Cannot open the file %s\n", imgName);
        return -1;
    }

    if (streamOut == (FILE *)0)
    {
        printf("Cannot use the file \"%s\" as output\n", copyImgName);
        return -1;
    }

    ImgSize imgSize = getImgSize(streamIn);

    rewind(streamIn);

    imageReader(streamIn, &imgSize, &bitDepth, header, colorTable, buffer);
    imageWritter(streamOut, &imgSize, header, colorTable, buffer, bitDepth);

    printf("Success! \n");
    printf("Width: %d\nHeight: %d\n", imgSize.width, imgSize.height);

    fclose(streamIn);
    fclose(streamOut);
}

ImgSize getImgSize(FILE *streamIn)
{
    byte _header[BMP_HEADER_SIZE];

    for (size_t i = 0; i < 54; i++)
    {
        _header[i] = getc(streamIn);
    }

    ImgSize s = {
        height : *(int *)&_header[22],
        width : *(int *)&_header[18]
    };

    return s;
}

void imageReader(
    FILE *streamIn,
    const ImgSize *imgSize,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer)
{
    for (size_t i = 0; i < 54; i++)
    {
        _header[i] = getc(streamIn);
    }

    *_bitDepth = *(int *)&_header[28];

    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(byte), 1024, streamIn);
    }

    int _imgSize = imgSize->height * imgSize->width;
    fread(buffer, sizeof(byte), _imgSize, streamIn);
}

void imageWritter(
    FILE *streamOut,
    const ImgSize *imgSize,
    byte *header,
    byte *colorTable,
    byte *buffer,
    int bitDepth)
{
    fwrite(header, sizeof(byte), 54, streamOut);

    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(byte), 1024, streamOut);
    }

    int _imgSize = imgSize->height * imgSize->width;
    fwrite(buffer, sizeof(byte), _imgSize, streamOut);
}