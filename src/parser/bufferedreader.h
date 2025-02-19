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

/// @brief A method to read a string from memory from a file
/// @param bf The buffered file to read from
/// @param startIndex The start index of the buffered file to start the string from.
/// @param maxStrLength The max string length before an error is returned.
/// @return Return a string that must be freed by the caller
char* bfGetLine(struct BufferedFile *bf, int startIndex, int maxStrLength, int *strLength);

int getC(struct BufferedFile *bf, int i);

#endif