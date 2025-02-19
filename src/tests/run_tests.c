#include <stdio.h>
#include "ctest.h"
#include "run_tests.h"
#include "test_parser.h"
#include "test_bufferedreader.h"

void run_tests(void)
{
  setResourceFilesBasePath("src/tests/resources");

  printf("TEST SUITE: TEST_BUFFERED_READER");
  runTest("test_bufferedReader_readFile", test_bufferedReader_readFile);

  printf("TEST SUITE: TEST_PARSER");
  runTest("test_parser_parseTreeNode", test_parser_parseTreeNode);
  runTest("test_parser_getNode", test_parser_getNode);
  runTest("test_parser_getNode_Deep", test_parser_getNode_Deep);
}

