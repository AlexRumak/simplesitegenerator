#ifndef TREENODE_H
#define TREENODE_H

#include <stdio.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  char *key;
  char *value; // For terminal nodes
  int nodeCount; // Node count == 0 for terminal nodes
  int bufferSize;
  TreeNode **nodes;
};

TreeNode *parseTreeNode(FILE *fp);
TreeNode *getNode(TreeNode* level, char* key);
TreeNode *addNode(TreeNode* toAddTo, char* key, char* value);
void printTree(TreeNode* root, int level);
int isTerminalNode(TreeNode* node);
void freeTree(TreeNode* root);

#endif