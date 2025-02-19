#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H

#include <stdio.h>

struct BufferedFile
{
  int maxFileSize;
  int l;
  int fileRead;
  FILE *fp;
  int *contents;
};

struct BufferedFile *createBufferedFile(FILE *fp, int maxFileSizeBytes);
int getC(struct BufferedFile *bf, int i);

#endif