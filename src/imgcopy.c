#include <stdio.h>
#include <stdlib.h>

#define BMP_HEADER_SIZE 54
#define BMP_COLOR_TABLE_SIZE 1024

#define byte unsigned char

void imageReader(
    const char *image_name,
    int *_height,
    int *_width,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer);

void imageWritter(
    const char *imageName,
    const int *_height,
    const int *_width,
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

    imageReader(imgName, &height, &width, &bitDepth, header, colorTable, buffer);
    imageWritter(copyImgName, &height, &width, header, colorTable, buffer, bitDepth);

    printf("Success! \n");
}

void imageReader(
    const char *image_name,
    int *_height,
    int *_width,
    int *_bitDepth,
    byte *_header,
    byte *_colorTable,
    byte *buffer)
{
    FILE *stream_in;

    stream_in = fopen("images/cameraman.bmp", "rb");

    if (stream_in == (FILE *)0)
    {
        printf("Unable to open the file\n");
        exit(1);
    }

    for (size_t i = 0; i < 54; i++)
    {
        _header[i] = getc(stream_in);
    }

    *_width = *(int *)&_header[18];
    *_height = *(int *)&_header[22];
    *_bitDepth = *(int *)&_header[28];

    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(byte), 1024, stream_in);
    }

    int imgSize = *_height * *_width;

    fread(buffer, sizeof(byte), imgSize, stream_in);

    fclose(stream_in);
}

void imageWritter(
    const char *imageName,
    const int *_height,
    const int *_width,
    byte *header,
    byte *colorTable,
    byte *buffer,
    int bitDepth)
{
    FILE *stream_out;
    stream_out = fopen("images/cameraman_copy.bmp", "wb");

    fwrite(header, sizeof(byte), 54, stream_out);

    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(byte), 1024, stream_out);
    }

    int imgSize = *_height * *_width;

    fwrite(buffer, sizeof(byte), imgSize, stream_out);

    fclose(stream_out);
}