#include "treenode.h"
#include "bufferedreader.h"
#include "lexical.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Constants
#define MAX_TOKEN_VALUE_LENGTH 1024
#define ROTATING_BUFFER_SIZE 1024
#define MAX_FILE_SIZE 10000

void freeTokenStack(struct TokenStack *stack) 
{
  for (int i = 0; i < stack->count; i++)
  {
    free(stack->tokens[i]->value);
    free(stack->tokens[i]);
  }
  free(stack->tokens);
  free(stack);
}

TreeNode *parseTreeNode(FILE *fp) 
{
  if (fp == NULL)
  {
    return NULL;
  }

  // The circular buffer to forward and backwards search
  struct BufferedFile *bf = createBufferedFile(fp, MAX_FILE_SIZE);

  ////////////// lexical parsing of file - first pass //////////////
  struct TokenStack *stack = parseFile(bf);

  ////////////// parsing the lexical notation //////////////
  TreeNode *root;
  int rc = parseTokenStack(stack, root);
  if (rc) 
  {
    printf("Error: Could not parse token stack\n");
    return NULL;
  }

  freeTokenStack(stack);

  return root;
}

TreeNode *getNode(TreeNode* root, char* key) 
{
  if (root == NULL)
  {
    return NULL;
  }

  if (root->nodes == NULL) {
    return NULL;
  }

  if (key == NULL)
  {
    return NULL;
  }

  // Check if key is a path
  char *path = strdup(key);
  char *token = strtok(path, "/");

  for(TreeNode **p = root->nodes; *p; p++)
  {
    if (strcmp((*p)->key, key) == 0) {
      return (*p);
    }
  }

  return NULL;
}

void printTree(TreeNode* root, int level) 
{
  if (root == NULL)
  {
    return;
  }

  for (int i = 0; i < level; i++)
  {
    fputs("|", stdout);
  }
  fputs("-", stdout);

  if (isTerminalNode(root))
  {
    printf("%s = %s\n", root->key, root->value);
    return;
  }

  printf("%s\n", root->key);
  
  for (int i = 0; i < root->nodeCount; i++)
  {
    printTree(root->nodes[i], level + 1);
  }
}

TreeNode *addNode(TreeNode* toAddTo, char* key, char* value) 
{
  // Create TreeNode
  TreeNode *newNode = malloc(sizeof(TreeNode));
  newNode->key = strdup(key);
  newNode->value = value ? strdup(value) : NULL;
  newNode->nodeCount = 0;
  newNode->bufferSize = 2;
  newNode->nodes = malloc(newNode->bufferSize * sizeof(TreeNode*));

  // Initialize nodes to NULL
  for (int i = 0; i < newNode->bufferSize; i++)
  {
    newNode->nodes[i] = NULL;
  }

  // Add toAddTo as a child of newNode
  if (toAddTo == NULL)
  {
    return newNode;
  }

  if (toAddTo->nodeCount + 1 == toAddTo->bufferSize)
  {
    newNode->bufferSize *= 2;
    newNode->nodes = realloc(newNode->nodes, newNode->bufferSize * sizeof(TreeNode*));

    // Initialize new nodes to NULL
    for (int i = toAddTo->nodeCount + 1; i < newNode->bufferSize; i++)
    {
      newNode->nodes[i] = NULL;
    }
  }

  toAddTo->nodes[toAddTo->nodeCount++] = newNode;

  return newNode;
}

int isTerminalNode(TreeNode* node) 
{
  if (node == NULL)
  {
    printf("Warning: Trying to check if a NULL node is terminal\n");
    return 0; // Undefined?
  }

  if (node->nodeCount == 0 && node->value == NULL) {
    printf("Warning: Terminal node has no value\n");
    return 0;
  }

  if (node->nodeCount == 0 && node->value != NULL)
  {
    return 1;
  }

  return 0;
}

void freeTree(TreeNode* root) 
{
  if (root == NULL)
  {
    return;
  }

  for (int i = 0; i < root->nodeCount; i++)
  {
    freeTree(root->nodes[i]);
  }

  free(root->key);
  if (root->value)
  {
    free(root->value);
  }
  free(root->nodes);
  free(root);
}