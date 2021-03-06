#include "bmp.h"

int bmp_read(unsigned char *image, int xsize, int ysize, char *filename)
{
    char fname_bmp[128];
    sprintf_s(fname_bmp, "%s.bmp", filename);

    FILE *fp;
	if (!( fopen_s(&fp ,fname_bmp, "rb")))
        return -1;

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);
    fread(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 3, fp);
    fclose(fp);
    return 0;
}

int bmp_write(unsigned char *image, int xsize, int ysize, char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };

    long file_size = (long)xsize * (long)ysize * 3 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    long width = xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;

    long height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;

    char fname_bmp[128];
    sprintf_s(fname_bmp, "%s.bmp", filename);

    FILE *fp;
	if (!(fopen_s(&fp,fname_bmp, "wb")))
        return -1;

    fwrite(header, sizeof(unsigned char), 54, fp);
    fwrite(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 3, fp);
    fclose(fp);
    return 0;
}

void BYTEcpy(BYTE* output,const BYTE* input,int length)
{
    int i;
    for ( i = 0 ; i < length ; i++ )
    {
        output[i] = input[i];
    }
}
