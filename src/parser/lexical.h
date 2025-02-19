#ifndef LEXICAL_H
#define LEXICAL_H

#include "token.h"

/// @brief Takes a line, parses it, and modifies the original line. Does not take ownership of the line, 
///        line must still be freed by the caller.
/// @param stack The token stack to push to
/// @param line Line must be mutable
void parseLine(struct TokenStack *stack, char* line);

#endif