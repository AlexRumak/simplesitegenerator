#ifndef LEXICAL_H
#define LEXICAL_H

#include "treenode.h"
#include "bufferedreader.h"

////////// TYPES //////////

enum TokenType
{
  ARRAYEL,
  KEY,
  VALUE,
  SPACE,
  NEWLINE
};

struct Token
{
  enum TokenType type;
  char *value;
};

struct TokenStack
{
  int count;
  int bufferSize;
  struct Token **tokens;
};

////////// FUNCTIONS //////////

/// @brief Takes a line, parses it, and modifies the original line. Does not take ownership of the line, 
///        line must still be freed by the caller.
/// @param stack The token stack to push to
/// @param line Line must be mutable
void parseLine(struct TokenStack *stack, char* line);


struct TokenStack *parseFile(struct BufferedFile *bf);

/// @brief 
/// @param stack 
/// @param root 
/// @return 
int parseTokenStack(struct TokenStack *stack, struct TreeNode *root);

/// @brief 
/// @param bufferSize
/// @return
struct TokenStack *createTokenStack(int bufferSize);

/// @brief 
/// @param stack 
void printTokenStack(struct TokenStack *stack);

/// @brief 
/// @param stack 
/// @param token 
void pushToken(struct TokenStack *stack, struct Token *token);

/// @brief 
/// @param tokenType
/// @param value
/// @return
struct Token *createToken(enum TokenType tokenType, char *value);

#endif