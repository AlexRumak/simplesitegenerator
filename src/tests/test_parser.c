#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ctest.h"

void test_parser()
{
  printf("Testing parser...\n");

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

  if (fp == NULL) {
    printf("Error: Could not open file\n");
    return;
  }

  TreeNode* root = parseTreeNode(fp);
  assertTrue(root != NULL, "Root node is not null");
  assertTrue(root->nodeCount == 2, "Root node does not have 2 children");
  assertTrue(root->nodes[0]->nodeCount == 0, "First child node is not terminal");
  assertTrue(root->nodes[1]->nodeCount == 0, "Second child node is not terminal");
}

int main(int argc, char *argv[])
{
  setResourceFilesBasePath(argv[1]);
  runTest("test_parser", test_parser);
  return 0;
}