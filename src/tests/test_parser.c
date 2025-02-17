#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ctest.h"

void test_parser()
{


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