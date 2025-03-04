#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_bufferedreader.h"
#include "bufferedreader.h"
#include "ctest.h"

void test_bufferedReader_readFile()
{
  FILE *fp = loadResource("settings.yml");
  struct BufferedFile *br = createBufferedFile(fp, 1024);

  assertTrue(getC(br, 1024) == -1, "Should be past end of file");
  assertTrue(getC(br, 0) == 'b', "Should be first char");
  assertTrue(getC(br, 3) == 'g', "Should be fourth char");
}

void test_bufferedReader_readLine()
{
  FILE *fp = loadResource("settings.yml");
  struct BufferedFile *br = createBufferedFile(fp, 1024);

  int length;
  char *line = bfGetLine(br, 0, 100, &length);

  printf("Expected: \"blog:\"\n  Actual: \"%s\"\n", line);

  assertFalse(strcmp(line, "blog:"), "First line should be \"blog:\"");
  free(line);

  printf("Expected: \"  locations: blogs/\"\n  Actual: \"%s\"\n", line);

  line = bfGetLine(br, 6, 100, &length);
  assertFalse(strcmp(line, "  location: blogs/"), "Second should be '  location: blogs/'");
  free(line);
}