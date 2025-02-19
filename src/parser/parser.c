#include "parser.h"
#include "token.h"
#include "bufferedreader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Constants
#define INITIAL_BUFFER_SIZE 1024
#define MAX_TOKEN_VALUE_LENGTH 1024
#define MIN_TOKEN_BUFFER_LENGTH 8
#define ROTATING_BUFFER_SIZE 1024
#define MAX_FILE_SIZE 10000
#define MAX_LINE_LENGTH 1024
#define LINE_BUFFER_START_SIZE 16
#define NULL_TERMINATOR '\0'
#define NEW_LINE '\n'

// ERROR Definitions
#define NOT_SPACE -1
#define INVALID_CHARACTER -2
#define COULD_NOT_GET_MEMORY -3

static int isSpace(int c)
{
  return c == ' ';
}

static int isSeparator(int c, int n)
{
  return c == ':' && n == ' ';
}

static int isNewLine(int c)
{
  return c == '\n';
}

static int isBannedCharacter(int c)
{
  return c == '\t';
}

static int isValidKeyChar(int c)
{
  return !isBannedCharacter(c);
}

static int numSpaces(char *line, int i)
{
  int c;
  int count = 0;
  while (isSpace(c = line[i++]))
  {
    count++;
  }
  return count;
}

static char* getErrorStr(int errorCode)
{
  switch(errorCode)
  {
    case NOT_SPACE: return "The character at the given index is not a space.";
    case INVALID_CHARACTER: return "The character at the given index is not a valid key character.";
    default: return "Unknown Error";
  }
}

static char* parseKey(char *line, int *i)
{
  int currChar, nextChar;

  int s = MIN_TOKEN_BUFFER_LENGTH, j = 0;
  char *str = malloc(s * sizeof(char));
  while ((currChar = line[*i]) != NULL_TERMINATOR)
  {
    if (isBannedCharacter(currChar))
    {
      *i = INVALID_CHARACTER;
      return NULL;
    }

    nextChar = line[*i + 1];

    if (isSeparator(currChar, nextChar))
    {
      *i += 1; // Skip separator
      break;
    }

    str[j] = currChar;
    j++;

    if (j == s)
    {
      s *= 2;
      char *newstr = realloc(str, s * sizeof(char));
      if (newstr == NULL)
      {
        free(str);
        *i = COULD_NOT_GET_MEMORY;
        return NULL;
      }
    }
    (*i)++;
  }

  str[j] = NULL_TERMINATOR;
  return str;
}

static char* parseValue(char *line, int *i)
{
  int currChar;

  int s = MIN_TOKEN_BUFFER_LENGTH, j = 0;
  char *str = malloc(s * sizeof(char));
  while ((currChar = line[*i]) != NULL_TERMINATOR)
  {
    if (isBannedCharacter(currChar))
    {
      *i = INVALID_CHARACTER;
      return NULL;
    }

    str[j] = currChar;
    j++;

    if (j == s)
    {
      s *= 2;
      char *newstr = realloc(str, s * sizeof(char));
      if (newstr == NULL)
      {
        free(str);
        *i = COULD_NOT_GET_MEMORY;
        return NULL;
      }
    }
    (*i)++;
  }

  str[j] = NULL_TERMINATOR;
  j--;

  // strip whitespace from end
  while (isSpace(str[j])) {
    str[j] = NULL_TERMINATOR;
  }

  return str;
}

static void parseLine(struct TokenStack *stack, char* line)
{
  // CALCULATE NUMBER OF SPACES AT START
  int i = numSpaces(line, 0);

  // EDGE-CASES
  // EDGE-CASE: Empty Line
  if (line[i] == '\0') {
    return;
  }
  // EDGE-CASE: Comment
  if (line[i] == '#') {
    return;
  }

  char *str = malloc(5 * sizeof(char));
  sprintf(str, "%d", i);

  if (i > 0)
  {
    pushToken(stack, createToken(SPACE, str));
  }

  // PARSE KEY
  char *key = parseKey(line, &i);
  int len = strlen(key);
  if (key[len - 1] == ':')
  {
    key[len - 1] = NULL_TERMINATOR;
    pushToken(stack, createToken(KEY, key));
    pushToken(stack, createToken(NEWLINE, NULL));
    return;
  }

  if (key == NULL)
  {
    printf("ERROR - Invalid Key - %s - in line %s\n", getErrorStr(i), line);
    return;
  }
  pushToken(stack, createToken(KEY, key));

  // SKIP SPACES
  i += numSpaces(line, i);

  // PARSEVALUE
  char *value = parseValue(line, &i);
  if (value == NULL)
  {
    printf("ERROR - Invalid Key - %s - in line %s\n", getErrorStr(i), line);
    return;
  }
  pushToken(stack, createToken(VALUE, value));
  pushToken(stack, createToken(NEWLINE, NULL));
  // DONE
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
  struct TokenStack *stack = createTokenStack(INITIAL_BUFFER_SIZE);

  int linesParsedSize = LINE_BUFFER_START_SIZE, currLine = 0;
  char** linesParsed = malloc(sizeof(char*) * linesParsedSize);

  // Unoptimized token parser
  int c, i = 0;
  while ((c = getC(bf, i)) != EOF)
  {
    int lineLength;
    char *line = bfGetLine(bf, i, MAX_LINE_LENGTH, &lineLength);

    linesParsed[currLine % linesParsedSize] = line;

    i+=lineLength;

    parseLine(stack, linesParsed[currLine % linesParsedSize]);

    currLine++;
  }

  printTokenStack(stack);

  // Create root node
  TreeNode *root = addNode(NULL, "root", NULL);

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