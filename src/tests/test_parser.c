#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ctest.h"

void test_parser_parseTreeNode()
{
  FILE *fp = loadResource("settings.yml");

  if (fp == NULL) {
    printf("Error: Could not open file\n");
    return;
  }

  TreeNode *root = parseTreeNode(fp);

  assertTrue(root != NULL, "Root node is not null");
  assertTrue(root->nodeCount == 2, "Root node does not have 2 children");
  assertTrue(root->nodes[0]->nodeCount == 0, "First child node is not terminal");
  assertTrue(root->nodes[1]->nodeCount == 0, "Second child node is not terminal");
}

TreeNode* build_test_tree()
{
  TreeNode *root = addNode(NULL, "root", NULL);
  addNode(root, "child1", "terminalValue");
  TreeNode *child = addNode(root, "child2", NULL);
  addNode(child, "child3", "terminalValue2");
  return root;
}

void test_parser_getNode()
{
  TreeNode *root = build_test_tree();

  TreeNode *node = getNode(root, "child2");
  assertTrue(node != NULL, "Node is null");
  TreeNode *node2 = getNode(node, "child3");
  assertTrue(node2 != NULL, "Node2 is null");
  assertTrue(strcmp(node2->value, "terminalValue2") == 0, "Node2 value is not terminalValue2");
}

void test_parser_getNode_Deep()
{
  TreeNode *root = build_test_tree();
  TreeNode *node = getNode(root, "child2/child3");
  assertTrue(node != NULL, "Node is null");
  assertTrue(strcmp(node->value, "terminalValue2") == 0, "Node value is not terminalValue2");
}