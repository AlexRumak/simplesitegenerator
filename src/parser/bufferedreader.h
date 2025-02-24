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

/// @brief Create a BufferedFile struct from a filepointer
/// @param fp The file pointer to read from
/// @param maxFileSizeBytes The max file size in bytes
/// @return A BufferedFile struct
struct BufferedFile *createBufferedFile(FILE *fp, int maxFileSizeBytes);

/// @brief A method to read a string from memory from a file
/// @param bf The buffered file to read from
/// @param startIndex The start index of the buffered file to start the string from.
/// @param maxStrLength The max string length before an error is returned.
/// @return Return a string that must be freed by the caller
char* bfGetLine(struct BufferedFile *bf, int startIndex, int maxStrLength, int *strLength);

/// @brief Gets character at location i in the buffered file
/// @param bf The buffered file to read from
/// @param i The index to read from
/// @return The character at the given index
int getC(struct BufferedFile *bf, int i);

#endif