#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>

// Global buffer
#define BUFFER_SIZE 1024
char cTest_current_test_name[BUFFER_SIZE];

#define RESOURCE_FILE_BUFFER_SIZE 1024
char cTest_resourceFilesBasePath[RESOURCE_FILE_BUFFER_SIZE];

int cTest_TestSuiteStarted = 0;

// Assertions
void assertTrue(int condition, char* message);
void assertFalse(int condition, char* message);

// Test Running
void setResourceFilesBasePath(char* path);
FILE* loadResource(char* filename);
void runTest(char* testName, void (*testFunction)());

#endif