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

#define NULL_TERMINATOR_SPACE 1
#define NULL_TERMINATOR '\0'

char* bfGetLine(struct BufferedFile *bf, int startIndex, int maxStrLength, int *strLength)
{
  char* str = malloc((maxStrLength + NULL_TERMINATOR_SPACE) * sizeof(int));

  int i = 0, c;
  while((c = getC(bf, startIndex++)) != '\n' && c != -1 && i < maxStrLength)
  {
    str[i++] = c;
  }
  
  if (i > maxStrLength)
  {
    return NULL;
  }

  str[i+1] = NULL_TERMINATOR;
  *strLength = i+1;
  
  return str;
}

int getC(struct BufferedFile *bf, int target)
{
  if (target > bf->maxFileSize)
  {
    printf("ERROR: i > maxFileSize %d", bf->maxFileSize);
    return -1;
  }

  int c;
  while (bf->l <= target && !bf->fileRead && ((c = fgetc(bf->fp)) != EOF))
  {
    bf->contents[bf->l++] = c;
  }

  if(c == EOF)
  {
    bf->fileRead = 1;
  }

  if (target == bf->l)
  {
    return '\n';
  }
  else if (target > bf->l)
  {
    return -1;
  }
  else 
  {
    return bf->contents[target];
  }
}