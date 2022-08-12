#include <stdio.h>

#define byte unsigned char

int main()
{
    FILE *stream_in, *stream_out;

    stream_in = fopen("images/cameraman.bmp", "rb");
    stream_out = fopen("images/cameraman_copy.bmp", "wb");

    if (stream_in == (FILE *)0)
    {
        printf("Unable to open the file\n");
        return 1;
    }

    byte header[54];
    byte color_table[1024];

    for (size_t i = 0; i < 54; i++)
    {
        header[i] = getc(stream_in);
    }

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int bit_depth = *(int *)&header[28];

    printf("width: %d\n", width);
    printf("height: %d\n", height);
    printf("bit_depth: %d\n\n", bit_depth);

    if (bit_depth <= 8)
    {
        fread(color_table, sizeof(byte), 1024, stream_in);
    }

    fwrite(header, sizeof(byte), 54, stream_out);

    byte buffer[height * width];

    fread(buffer, sizeof(byte), height * width, stream_in);

    if (bit_depth <= 8)
    {
        fwrite(color_table, sizeof(byte), 1024, stream_out);
    }

    fwrite(buffer, sizeof(byte), height * width, stream_out);

    fclose(stream_in);
    fclose(stream_out);
}