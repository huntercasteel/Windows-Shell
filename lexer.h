#ifndef LEXER_H
#define LEXER_H
#include "master.h"

typedef struct Lexer lexer_t; // creates a struct Lexer and defines it as a new data type, lexer_t
typedef struct Token token_t; // creates a struct Token and defines it as a new data type, token_t

struct Lexer // a data structure that can lexically analyze a string, separate it into bash-syntax tokens, and store the tokens as a linked list
{
    token_t *head; // stores the address of the first token in the linked list
	int size; // stores the amount of tokens in the lexer
};

struct Token // a data structure that can store the value of a token and point to another token to form a linked list
{
    char *value; // stores the value of the token
    token_t *next; // stores the address of the next token in the linked list
};

enum TokenType // indicates the type of a token that is being created
{
	type_Ampersand = '&',
	type_Bang = '!',
	type_DollarSign = '$',
	type_DoubleQuote = '\"',
	type_Equals = '=',
	type_Escape = '\\',
	type_GreaterThan = '>',
	type_LesserThan = '<',
	type_General = -1,
	type_Pipe = '|',
	type_Semicolon = ';',
	type_SingleQuote = '\'',
	type_Space = ' ',
};

enum TokenState // indicates the state of a token that is being created
{
	state_DoubleQuote,
	state_Escape,
	state_General,
    state_SingleQuote,
};

lexer_t* createLexer();
bool fillLexer(lexer_t *lexer, char *string);
void copyLexer(lexer_t *lexer, char ***tokens);
void printLexer(lexer_t *lexer);
void clearLexer(lexer_t *lexer);
void destroyLexer(lexer_t *lexer);
token_t* createToken();
void addToken(lexer_t *lexer, token_t *token);
int getTokenType(char character);
void append(char **string, char character);

#endif