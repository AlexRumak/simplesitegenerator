#include "parser.h"
#include "token.h"
#include "bufferedreader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NULL_TERMINATOR '\0'
#define NEW_LINE '\n'
#define EMPTY_SPACE ' '
#define ARRAY_CHAR '-'
#define MIN_TOKEN_BUFFER_LENGTH 8


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
  if (line[i] == '\0') {
    return;
  }

  // Array
  if (line[i] == ARRAY_CHAR && line[i] != NULL_TERMINATOR && line[i + 1] == EMPTY_SPACE)
  {
    i += 2;
    char *str = malloc(5 * sizeof(char));
    sprintf(str, "%d", i);
    pushToken(stack, createToken(ARRAYEL, str));
  }
  else if (i > 0)
  {
    char *str = malloc(5 * sizeof(char));
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