#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  char* key;
  char* value; // For terminal nodes
  int nodeCount; // Node count == 0 for terminal nodes
  TreeNode* nodes[];
};

TreeNode* parseTreeNode(FILE *fp);
TreeNode* getKey(TreeNode* level, char* key);
int isTerminalNode(TreeNode* node);
int freeTree(TreeNode* root);

#endif