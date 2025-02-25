#include "ctest.h"
#include "lexical.h"
#include <string.h>
#include <stdlib.h>

#define PRINT_RESULTS(actual, expected) \
  printInfo("Actual: \n"); \
  printTokenStack(actual); \
  printInfo("Expected: \n"); \
  printTokenStack(expected);

int areTokenStacksEqual(struct TokenStack *stack, struct TokenStack *stack2)
{
  if (stack->count != stack2->count)
  {
    return 0;
  }

  for (int i = 0; i < stack->count; i++) 
  {
    if (stack->tokens[i]->type != stack2->tokens[i]->type)
    {
      return 0;
    }

    if (safe_strcmp(stack->tokens[i]->value, stack2->tokens[i]->value) != 0)
    {
      return 0;
    }
  }
  return 1;
}

void test_lexical_parseYamlBasic()
{
  char* line = "  key: value";
  struct TokenStack *stack = createTokenStack(8);
  parseLine(stack, line);
  
  struct TokenStack *expected = createTokenStack(8);
  pushToken(expected, createToken(SPACE, "2"));
  pushToken(expected, createToken(KEY, "key"));
  pushToken(expected, createToken(VALUE, "value"));
  pushToken(expected, createToken(NEWLINE, NULL));

  PRINT_RESULTS(stack, expected);

  assertTrue(areTokenStacksEqual(stack, expected), "Basic parse failed");
}

void test_lexical_parseYamlArray()
{
  char* line = "    - key: value";
  struct TokenStack *stack = createTokenStack(8);
  parseLine(stack, line);
  
  struct TokenStack *expected = createTokenStack(8);
  pushToken(expected, createToken(ARRAYEL, "6"));
  pushToken(expected, createToken(KEY, "key"));
  pushToken(expected, createToken(VALUE, "value"));
  pushToken(expected, createToken(NEWLINE, NULL));

  PRINT_RESULTS(stack, expected);

  assertTrue(areTokenStacksEqual(stack, expected), "Basic parse failed");
}

void test_lexical_parseYamlArrayComplex()
{
  char* line = "    - key: value";
  struct TokenStack *stack = createTokenStack(8);
  parseLine(stack, line);

}

void test_lexical_parseCommentAtEndOfDict()
{
  char line[] = "      key: value # Hi this is a comment";
  struct TokenStack *stack = createTokenStack(8);
  parseLine(stack, line);

  struct TokenStack *expected = createTokenStack(8);
  pushToken(expected, createToken(SPACE, "6"));
  pushToken(expected, createToken(KEY, "key"));
  pushToken(expected, createToken(VALUE, "value"));
  pushToken(expected, createToken(NEWLINE, NULL));

  PRINT_RESULTS(stack, expected);

  assertTrue(areTokenStacksEqual(stack, expected), "Comment parse failed");
}

void test_lexical_parseSpacesYml()
{
  
}