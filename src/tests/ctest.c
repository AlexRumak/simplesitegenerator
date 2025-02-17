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

FILE* loadResource(char* filename)
{
  if (cTest_resourceFilesBasePath == NULL)
  {
  }

  char full_path[256];
  int rc = snprintf(full_path, sizeof(full_path), "./%s/settings.yml", cTest_resourceFilesBasePath);

  if (rc > sizeof(full_path)) {
    printf("Error: File path too long\n");
    return;
  }

  if (rc < 0) {
    strerror(rc);
    printf("Error: Error formatting file path\n");
    return;
  }

  FILE *fp = fopen(full_path, "r");
}

void runTest(char* testName, void (*testFunction)())
{
  if (!cTest_TestSuiteStarted) {
    printf("============\tStarting Test Suite\t============\n\n\n");
    cTest_TestSuiteStarted = 1;
  }

  printf("============\tRunning Test: %s\t============\n", testName);
  if (setjmp(endTest) == 0) {
    strcpy(cTest_current_test_name, testName);
    testFunction();
    printf("============\tTest Complete\t============\n");
  } else {
    printf("============\tTest Failed\t============\n");
  }
}

