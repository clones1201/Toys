#ifndef __BMP_H_
#define __BMP_H_

#include "global.h"

int bmp_read(unsigned char *image, int xsize, int ysize, char *filename);
int bmp_write(unsigned char *image, int xsize, int ysize, char *filename);
void BYTEcpy(BYTE* output,const BYTE *input,int length);

#endif
