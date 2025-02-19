#ifndef TOKEN_H
#define TOKEN_H

enum TokenType
{
  STARTARRAY,
  ENDARRAY,
  STARTDICTIONARY,
  ENDDICTIONARY,
  KEY,
  VALUE
};

const char* getTokenType(enum TokenType type);

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

struct TokenStack *createTokenStack(int bufferSize);
void printTokenStack(struct TokenStack *stack);

void pushToken(struct TokenStack *stack, struct Token *token);
struct Token *createToken(enum TokenType tokenType, char *value);

#endif