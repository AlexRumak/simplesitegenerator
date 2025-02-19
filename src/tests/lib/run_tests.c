#include <stdio.h>
#include "ctest.h"
#include "run_tests.h"
#include "test_parser.h"
#include "test_bufferedreader.h"
#include "test_lexical.h"

void run_tests(void)
{
  setResourceFilesBasePath("src/tests/resources");

  printf("TEST SUITE: TEST_BUFFERED_READER\n");
  runTest("test_bufferedReader_readFile", test_bufferedReader_readFile);
  runTest("test_bufferedReader_readLine", test_bufferedReader_readLine);

  printf("TEST SUITE: TEST_LEXICAL_PARSER\n");
  runTest("test_lexical_parseYamlBasic", test_lexical_parseYamlBasic);  
  runTest("test_lexical_parseYamlArray", test_lexical_parseYamlArray);
  runTest("test_lexical_parseYamlArrayComplex", test_lexical_parseYamlArrayComplex);  

  printf("TEST SUITE: TEST_PARSER\n");
  runTest("test_parser_parseTreeNode", test_parser_parseTreeNode);
  runTest("test_parser_getNode", test_parser_getNode);
  runTest("test_parser_getNode_Deep", test_parser_getNode_Deep);

}