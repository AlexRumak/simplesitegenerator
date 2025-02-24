#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>

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