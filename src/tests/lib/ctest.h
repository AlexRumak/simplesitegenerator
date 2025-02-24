#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>

// Extern C_Test vars
extern char* cTest_current_test_name;
extern char* cTest_resourceFilesBasePath;
extern int cTest_TestSuiteStarted;

// Compare
int safe_strcmp(const char *str1, const char *str2);

// Assertions
void assertTrue(int condition, char* message);
void assertFalse(int condition, char* message);

// Test Running
void setResourceFilesBasePath(char* path);
FILE* loadResource(char* filename);
void runTest(char* testName, void (*testFunction)());

// Logging
void printError(char* message);
void printInfo(char* message);
void printSuccess(char* message);

#endif