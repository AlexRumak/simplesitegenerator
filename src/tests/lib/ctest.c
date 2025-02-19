#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "ctest.h"

int cTest_TestSuiteStarted = 0;

jmp_buf endTest;

void assertTrue(int condition, char* message) 
{
  if (!condition) {
    printError(message);
    longjmp(endTest, 1);
  }
}

void assertFalse(int condition, char* message)
{
  if (condition) {
    printError(message);
    longjmp(endTest, 1);
  }
}

void setResourceFilesBasePath(char* path)
{
  strcpy(cTest_resourceFilesBasePath, path);
}

FILE* loadResource(char* filename)
{
  if (cTest_resourceFilesBasePath[0] == '\0')
  {
    printf("Error: Resource files base path not set\n");
    return NULL;
  }

  char full_path[256];
  int rc = snprintf(full_path, sizeof(full_path), "./%s/settings.yml", cTest_resourceFilesBasePath);

  if (rc > sizeof(full_path)) {
    printf("Error: File path too long\n");
    return NULL;
  }

  if (rc < 0) {
    strerror(rc);
    printf("Error: Error formatting file path\n");
    return NULL;
  }

  FILE *fp = fopen(full_path, "r");

  return fp;
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
    printSuccess("Test Passed");
    printf("============\tTest Complete\t============\n");
  } else {
    printf("============\tTest Failed\t============\n");
  }
}

void printSuccess(char* message)
{
  printf("\033[0;32m");      
  printf("%s - %s\n", cTest_current_test_name, message);
  printf("\033[0m");
}

void printError(char* message)
{
  printf("\033[0;31m");      
  printf("%s - Assertion failed: %s\n", cTest_current_test_name, message);
  printf("\033[0m");
}

void printInfo(char* message)
{
  printf("%s - INFO: %s\n", cTest_current_test_name, message);
}