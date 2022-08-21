#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    int bitDepth,
    bool writeBuffer);

void grayScale(
    FILE *streamIn,
    FILE *streamOut,
    const ImgSize *imgSize,
    int bitDepth,
    byte *colorTable,
    byte *header,
    byte *buffer);

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

    imageReader(streamIn, &imgSize, &bitDepth, header, colorTable, buffer);
    grayScale(streamIn,
              streamOut,
              &imgSize,
              bitDepth,
              colorTable,
              header,
              buffer);
    imageWritter(streamOut, &imgSize, header, colorTable, buffer, bitDepth, false);

    printf("Success! \n");
    printf("Width: %d\nHeight: %d\n", imgSize.width, imgSize.height);

    fclose(streamIn);
    fclose(streamOut);
}

ImgSize getImgSize(FILE *streamIn)
{
    byte _header[BMP_HEADER_SIZE];

    for (size_t i = 0; i < BMP_HEADER_SIZE; i++)
    {
        _header[i] = getc(streamIn);
    }

    rewind(streamIn);

    ImgSize s = {
        height : *(int *)&_header[22],
        width : *(int *)&_header[18]
    };

    return s;
}

void grayScale(
    FILE *streamIn,
    FILE *streamOut,
    const ImgSize *imgSize,
    int bitDepth,
    byte *colorTable,
    byte *header,
    byte *buffer)
{
    fseek(streamIn, BMP_HEADER_SIZE, SEEK_SET);
    fseek(streamOut, BMP_HEADER_SIZE, SEEK_SET);

    int _imgSize = imgSize->height * imgSize->width;

    for (size_t i = 0; i < _imgSize; i++)
    {
        byte r = getc(streamIn);
        byte g = getc(streamIn);
        byte b = getc(streamIn);

        int gray_pixel = (r * 0.3) + (g * 0.59) + (b * 0.11);

        for (size_t c = 0; c < 3; c++)
            putc(gray_pixel, streamOut);
    }

    rewind(streamIn);
    rewind(streamOut);
}

void imageReader(
    FILE *streamIn,
    const ImgSize *imgSize,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer)
{
    for (size_t i = 0; i < BMP_HEADER_SIZE; i++)
    {
        _header[i] = getc(streamIn);
    }

    *_bitDepth = *(int *)&_header[28];

    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(byte), BMP_COLOR_TABLE_SIZE, streamIn);
    }

    int _imgSize = imgSize->height * imgSize->width;
    fread(buffer, sizeof(byte), _imgSize, streamIn);

    rewind(streamIn);
}

void imageWritter(
    FILE *streamOut,
    const ImgSize *imgSize,
    byte *header,
    byte *colorTable,
    byte *buffer,
    int bitDepth,
    bool writeBuffer)
{
    fwrite(header, sizeof(byte), BMP_HEADER_SIZE, streamOut);

    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(byte), BMP_COLOR_TABLE_SIZE, streamOut);
    }

    if (writeBuffer)
    {
        int _imgSize = imgSize->height * imgSize->width;
        fwrite(buffer, sizeof(byte), _imgSize, streamOut);
    }

    rewind(streamOut);
}