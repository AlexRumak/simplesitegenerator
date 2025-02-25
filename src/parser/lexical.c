#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexical.h"

#define INITIAL_BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 1024
#define LINE_BUFFER_START_SIZE 16

#define NULL_TERMINATOR '\0'
#define NEW_LINE '\n'
#define EMPTY_SPACE ' '
#define ARRAY_CHAR '-'
#define MIN_TOKEN_BUFFER_LENGTH 8
#define INT_MAX_DIGITS 11

// ERROR Definitions
#define NOT_SPACE -1
#define INVALID_CHARACTER -2
#define COULD_NOT_GET_MEMORY -3

static int isSeparator(int c, int n)
{
  return c == ':' && n == ' ';
}

static int isBannedCharacter(int c)
{
  return c == '\t';
}

const char* getTokenType(enum TokenType type)
{
  switch (type) 
  {
     case ARRAYEL: return "ARRAYEL";
     case KEY: return "KEY";
     case VALUE: return "VALUE";
     case SPACE: return "SPACE";
     case NEWLINE: return "NEWLINE";
     default: return "ERROR";
  }
}

struct TokenStack *createTokenStack(int bufferSize)
{
  struct TokenStack *stack = malloc(sizeof(struct TokenStack));
  stack->count = 0;
  stack->bufferSize = bufferSize;
  stack->tokens = malloc(bufferSize * sizeof(struct Token*));
  return stack;
}

void printTokenStack(struct TokenStack *stack)
{
  for(int i = 0; i < stack->count; i++)
  {
    printf("%s %s\n", getTokenType(stack->tokens[i]->type), stack->tokens[i]->value == NULL ? "" : stack->tokens[i]->value);
  }
}

void pushToken(struct TokenStack *stack, struct Token *token)
{
  if (stack->count == stack->bufferSize)
  {
    stack->bufferSize *= 2;
    stack->tokens = realloc(stack->tokens, stack->bufferSize * sizeof(struct Token*));
  }
  stack->tokens[stack->count++] = token;
}

struct Token *createToken(enum TokenType tokenType, char *value)
{
  struct Token *token = malloc(sizeof(struct Token));
  token->type = tokenType;
  token->value = value;
  return token;
}

static int numSpaces(char *line, int i)
{
  int c;
  int count = 0;
  while ((c = line[i++]) == EMPTY_SPACE)
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
  while (str[j] == EMPTY_SPACE) {
    str[j] = NULL_TERMINATOR;
  }

  return str;
}

static void stripComments(char line[])
{
  for(int i = 0; line[i] != NULL_TERMINATOR; i++)
  {
    if (line[i] == '#')
    {
      if (i != 0) {
        if (line[i - 1] == EMPTY_SPACE && line[i+1] == EMPTY_SPACE) {
          // Valid comment, terminate line
          line[i] = '\0';
        }
      }
    }
  }
}

void parseLine(struct TokenStack *stack, char line[])
{
  // Strip Commments - scan forward for comments
  stripComments(line);

  // CALCULATE NUMBER OF SPACES AT START
  int i = numSpaces(line, 0);

  // EDGE-CASES
  // EDGE-CASE: Empty Line
  if (line[i] == NULL_TERMINATOR) {
    return;
  }

  // Array
  if (line[i] == ARRAY_CHAR && line[i] != NULL_TERMINATOR && line[i + 1] == EMPTY_SPACE)
  {
    i += 2;
    char *str = malloc(INT_MAX_DIGITS * sizeof(char));
    sprintf(str, "%d", i);
    pushToken(stack, createToken(ARRAYEL, str));
  }
  else if (i > 0)
  {
    char *str = malloc(INT_MAX_DIGITS * sizeof(char));
    sprintf(str, "%d", i);
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

struct TokenStack *parseFile(struct BufferedFile *bf)
{
  struct TokenStack *stack = createTokenStack(INITIAL_BUFFER_SIZE);

  int linesParsedSize = LINE_BUFFER_START_SIZE, currLine = 0;
  char** linesParsed = malloc(sizeof(char*) * linesParsedSize);

  // Unoptimized token parser
  int c, i = 0;
  while ((c = getC(bf, i)) != EOF)
  {
    int lineLength;
    char *line = bfGetLine(bf, i, MAX_LINE_LENGTH, &lineLength);

    if (currLine > linesParsedSize)
    {
      char *l = linesParsed[currLine % linesParsedSize];
      free(l);
    }

    linesParsed[currLine % linesParsedSize] = line;
    i+=lineLength;

    parseLine(stack, linesParsed[currLine % linesParsedSize]);
    currLine++;
  }

  // Free
  for(int i = 0; i < currLine && i < linesParsedSize; i++)
  {
    free(linesParsed[i]);
  }

  return stack;
}

enum ParserState 
{
  NEXT,
  KEYSTATE,
  SPACESTATE,
  VALUESTATE,
  UPSTATE
};

struct Stack
{
  size_t size;
  int top;
  struct TreeNode **arr;
};

static struct Stack *initStack(size_t size)
{
  struct Stack *stack = malloc(sizeof(struct Stack));
  stack->top = -1;
  stack->size = size;
  stack->arr = malloc(size * sizeof(TreeNode*));
  return stack; 
}

static TreeNode *peekStack(struct Stack *stack) 
{
  if (stack->top < 0)
  {
    // no elements
    return NULL;
  }

  return stack->arr[stack->top];
}

static int push(struct Stack *stack, TreeNode *next) 
{
  if (stack->top + 1 == stack->size) 
  {
    stack->size *= 2;
    TreeNode **newArr = realloc(stack->arr, stack->size * sizeof(TreeNode*));
    if (newArr == NULL)
    {
      printf("Failed to increase stack size.\n");
      return -1;   
    }
    stack->arr = newArr;
  }

  stack->arr[++stack->top] = next;

  return 0;
}

static TreeNode *pop(struct Stack *stack) 
{
  if (stack->top < 0)
  {
    printf("Attempted to pop off empty stack\n");
    return NULL;
  }
  return stack->arr[stack->top--];
}

static void freeStack(struct Stack *stack)
{
  free(stack->arr);
  free(stack);
}

int parseTokenStack(struct TokenStack *stack, struct TreeNode *root) 
{
  // Parser State machine
  enum ParserState state = NEXT;

  int expectedLevels = 4;
  struct Stack *levelStack = initStack(expectedLevels);
  int lastDepth = 0;

  int i = 0;
  root = addNode(NULL, "root", NULL);
  push(levelStack, root);


  struct Token *nextToken = NULL;
  while(1)
  {
    switch(state) 
    {
      case NEXT:
        if (i >= stack->count)
        {
          break;
        }
        nextToken = stack->tokens[i++];

        switch(nextToken->type)
        {
          case KEY:
          {
            TreeNode *level = peekStack(levelStack);
            TreeNode *newNode = addNode(level, nextToken->value, NULL);
            push(levelStack, newNode);
            // Next state == NEXT
            break;
          }
          case VALUE:
          {
            TreeNode *level = peekStack(levelStack);

            if (level->value != NULL)
            {
              printf("Attempted to set value to key %s twice\n", level->key);
              return 1;
            }

            level->value = nextToken->value;
            state = UPSTATE;
            break;
          }
          case ARRAYEL:
            printf("Not yet supported\n");
            return 1;
            break;
          case NEWLINE:
            // Do nothing?
            break;
          case SPACE:
            lastDepth = atoi(nextToken->value);
            state = SPACESTATE;
            break;
          default:
            printf("Unknown token returned - critical failure\n");
            return 1;
        }

        break;
      case SPACESTATE:

        if (i >= stack->count)
        {
          break;
        }
        nextToken = stack->tokens[i++];

        switch(nextToken->type)
        {
          case KEY:

            break;
          case VALUE:

            break;
          case ARRAYEL:
            printf("Not yet supported\n");
            return 1;
            break;
          case NEWLINE:
            // Do nothing?
            break;
          case SPACE:
            lastDepth = atoi(nextToken->value);
            state = SPACESTATE;
            break;
          default:
            printf("Unknown token returned - critical failure\n");
            return 1;
        }

        break;
      case KEYSTATE:  
        break;
      case UPSTATE:
        break;
      case VALUESTATE:
        break;
      default:
        printf("Unknown state reach - critical failure\n");
        return 1;
    }
  }

  freeStack(levelStack);
  return 0;
}