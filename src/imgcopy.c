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

ImgSize getImgSize(const char *image_name);

void imageReader(
    const char *imageName,
    const ImgSize *imgSize,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer);

void imageWritter(
    const char *imageName,
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

    ImgSize imgSize = getImgSize(imgName);

    imageReader(imgName, &imgSize, &bitDepth, header, colorTable, buffer);
    imageWritter(copyImgName, &imgSize, header, colorTable, buffer, bitDepth);

    printf("Success! \n");
    printf("Width: %d\nHeight: %d\n", imgSize.width, imgSize.height);
}

ImgSize getImgSize(const char *imageName)
{
    byte _header[BMP_HEADER_SIZE];
    FILE *stream_in;

    stream_in = fopen(imageName, "rb");

    if (stream_in == (FILE *)0)
    {
        printf("Unable to open the file\n");
        exit(1);
    }

    for (size_t i = 0; i < 54; i++)
    {
        _header[i] = getc(stream_in);
    }

    ImgSize s = {
        height : *(int *)&_header[22],
        width : *(int *)&_header[18]
    };

    return s;
}

void imageReader(
    const char *imageName,
    const ImgSize *imgSize,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer)
{
    FILE *stream_in;

    stream_in = fopen(imageName, "rb");

    if (stream_in == (FILE *)0)
    {
        printf("Unable to open the file\n");
        exit(1);
    }

    for (size_t i = 0; i < 54; i++)
    {
        _header[i] = getc(stream_in);
    }

    *_bitDepth = *(int *)&_header[28];

    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(byte), 1024, stream_in);
    }

    int _imgSize = imgSize->height * imgSize->width;
    fread(buffer, sizeof(byte), _imgSize, stream_in);

    fclose(stream_in);
}

void imageWritter(
    const char *imageName,
    const ImgSize *imgSize,
    byte *header,
    byte *colorTable,
    byte *buffer,
    int bitDepth)
{
    FILE *stream_out;
    stream_out = fopen(imageName, "wb");

    fwrite(header, sizeof(byte), 54, stream_out);

    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(byte), 1024, stream_out);
    }

    int _imgSize = imgSize->height * imgSize->width;
    fwrite(buffer, sizeof(byte), _imgSize, stream_out);

    fclose(stream_out);
}