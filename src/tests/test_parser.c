#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void test_parser(char* resourceFilePath)
{
  printf("Testing parser...\n");

  char full_path[256];
  int rc = snprintf(full_path, sizeof(full_path), "./%s/settings.yml", resourceFilePath);

  if (rc > sizeof(full_path)) {
    printf("Error: File path too long\n");
    return;
  }

  if (rc < 0) {
    strerror(rc);
    printf("Error: Error formatting file path\n");
    return;
  }

  printf("Opening file: %s\n", full_path);
  FILE *fp = fopen(full_path, "r");

  if (fp == NULL) {
    printf("Error: Could not open file\n");
    return;
  }

  TreeNode* root = parseTreeNode(fp);
  assert(root != NULL);
  assert(root->nodeCount == 2);
  assert(root->nodes[0]->nodeCount == 0);
  assert(root->nodes[1]->nodeCount == 0);
}

int main(int argc, char *argv[])
{
  char* resourceFilePath = argv[1];
  printf("Resource file path: %s\n", resourceFilePath);

  test_parser(resourceFilePath);
  return 0;
}