#include <stdlib.h>
#include <stdio.h>
#include "token.h"


const char* getTokenType(enum TokenType type)
{
  switch (type) 
  {
     case STARTARRAY: return "STARTARRAY";
     case ENDARRAY: return "ENDARRAY";
     case STARTDICTIONARY: return "STARTDICTIONARY";
     case ENDDICTIONARY: return "ENDDICTIONARY";
     case KEY: return "KEY";
     case VALUE: return "VALUE";
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
    stack = realloc(stack, stack->bufferSize * sizeof(struct Token*));
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
