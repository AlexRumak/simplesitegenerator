#include <stdio.h>
#include <string.h>
#include "ctest.h"
#include <setjmp.h>

jmp_buf endTest;

void assertTrue(int condition, char* message) 
{
  if (!condition) {
    printf("%s - Assertion failed: %s\n", cTest_current_test_name, message);
    longjmp(endTest, 1);
  }
}

void assertFalse(int condition, char* message)
{
  if (condition) {
    printf("%s - Assertion failed: %s\n", cTest_current_test_name, message);
    longjmp(endTest, 1);
  }
}

void setResourceFilesBasePath(char* path)
{
  strcpy(cTest_resourceFilesBasePath, path);
}

void runTest(char* testName, void (*testFunction)())
{
  printf("============\tRunning Test: %s\t============\n", testName);
  if (setjmp(endTest) == 0) {
    strcpy(cTest_current_test_name, testName);
    testFunction();
    printf("============\tTest Complete\t============\n");
  } else {
    printf("============\tTest Failed\t============\n");
  }
}

