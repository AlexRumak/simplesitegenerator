#include "bufferedreader.h"
#include <stdlib.h>

struct BufferedFile *createBufferedFile(FILE *fp, int maxFileSizeBytes)
{
  struct BufferedFile *bf = malloc(sizeof(struct BufferedFile));
  bf->maxFileSize = maxFileSizeBytes;
  bf->l = 0;
  bf->fp = fp;
  bf->fileRead = 0;
  bf->contents = malloc(maxFileSizeBytes * sizeof(int)); //TODO: Optimize me
  return bf;
}

int getC(struct BufferedFile *bf, int i)
{
  int c;
  while (bf->l < i && !bf->fileRead && ((c = fgetc(bf->fp)) != EOF))
  {
    bf->contents[bf->l++] = c;
  }

  if(c == EOF)
  {
    bf->fileRead = 1;
  }

  if (i > bf->l)
  {
    return -1;
  }
  else 
  {
    return bf->contents[i];
  }
}