#include "lexer.h"

/***********************************************************************************
 * Summary:		creates and allocates memory for a lexer and initializes its
 *				variables to default values
 * Returns:		the address of the lexer that is created
***********************************************************************************/
lexer_t* createLexer()
{
	lexer_t *lexer = malloc(sizeof(lexer_t)); // allocates memory for a lexer and stores its address in memory
	lexer->head = NULL;
	lexer->size = 0;
	return lexer;
}

/***********************************************************************************
 * Summary:		lexically analyzes a string and stores the token stream in a lexer
 * Parameters:	lexer - the address of the lexer to store the token stream in
 *				string - the string to lexically analyze
 * Returns:		true if the input string is valid or false if the input string is
 *				not valid
***********************************************************************************/
bool fillLexer(lexer_t *lexer, char *string)
{
	if(lexer != NULL && string != NULL) // checks if the input parameters are not empty
	{
		int index; // stores the index of the current position in the input string
		int tokenState = state_General; // stores the token state of the current token being created
		int tokenType; // stores the token type of the current element of the input string
		token_t *currentToken = createToken(); // stores the address of the current token being created
		
		clearLexer(lexer);
		
		for(index = 0; string[index] != '\0'; index++) // loops untils the end of the input string is reached
		{
			tokenType = getTokenType(string[index]); // finds the token type of the current element of the input string
			
			if(tokenState == state_General) // checks if the current token being created is in the general state
			{
				switch(tokenType) // checks if the current element of the input string is one following types
				{
					case type_DoubleQuote: // if it is a double quote
						if(string[++index] == '\0') // traverses to the next element of the input string and checks if the end of the input string has been reached
						{
							clearLexer(lexer);
							return false; // signals that input string is not valid
                        }
						
						tokenType = getTokenType(string[index]); // finds the token type of the current element of the input string
						
						if(tokenType != type_DoubleQuote) // checks if the current element of the input string is not a double quote
						{
							tokenState = state_DoubleQuote;
							append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
						}
						else // if the current element of the input string is a double quote
						{
							currentToken->value = strdup("");
						}
						
						break;
					case type_Escape: // if it is an escape
						if(string[index + 1] == '\0') //checks if the end of the input string has been reached
						{
							clearLexer(lexer);
							return false; // signals that input string is not valid
						}
						
						tokenState = state_Escape;
						append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
						break;
					case type_General: // if it's a general character
						append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
						break;
					case type_SingleQuote: // if it is a single quote
						if(string[++index] == '\0') // traverses to the next element of the input string and checks if the end of the input string has been reached
						{
							clearLexer(lexer);
							return false; // signals that input string is not valid
						}
						
						tokenType = getTokenType(string[index]); // finds the token type of the current element of the input string
						
						if(tokenType != type_SingleQuote) // checks if the current element of the input string is not a single quote
						{
							tokenState = state_SingleQuote;
							append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
						}
						else // if the current element of the input string is not a single quote
						{
							currentToken->value = strdup("");
						}
						
						break;
					case type_Space: // if it's a space
						if(currentToken->value != NULL) // checks if the value of current token being created is not empty
						{
							addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
							currentToken = createToken(); // creates an empty token
						}
						
						break;
					case type_Ampersand: // if it is an ampersand
					case type_Bang: // or a bang
					case type_DollarSign: // or a dollar sign
					case type_Equals: // or an equals
					case type_GreaterThan: // or a greater than
					case type_LesserThan: // or a lesser than
					case type_Pipe: // or a pipe
					case type_Semicolon: // or a semicolon
						if(currentToken->value != NULL) // checks if the value of current token being created is not empty
						{
							addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
							currentToken = createToken(); // creates an empty token
						}
						
						append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
						addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
						currentToken = createToken(); // creates an empty token
						break;
				}
			}
			else if(tokenState == state_DoubleQuote) // checks if the current token being created is in the double quote state
			{
				if(tokenType != type_DoubleQuote) // checks if the current element of the input string is not a double quote
					append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
				else // if the current element of the input string is a double quote
				{
					addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
					currentToken = createToken(); // creates an empty token
					tokenState = state_General;
				}
			}
			else if(tokenState == state_Escape) // checks if the current token being created is in the escape state
			{
				if(isdigit(string[index])) // checks if the current element of the input string is a digit
				{
					int max = index + MAX_DIGITS; // stores the index of the last possible character in the special character
					
					while(index < max && string[index] != '\0') // loops until the last possible character in the special character or the end of the input string has been reached
					{
						if(isdigit(string[index])) // checks if the current element of the input string is a digit
						{
							append(&currentToken->value, string[index++]); // appends the current element of the input string to the value of the current token being created and traverses to the next element of the input string
						}
						else // if the current element of the input string is not a digit
						{
							break;
						}
					}
					
					index--; // ensures that an element of the input string will not be skipped
				}
				else // if the current element of the input string is not a digit
				{
					append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
				}
				
				addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
				currentToken = createToken(); // creates an empty token
				tokenState = state_General;
			}
			else if(tokenState == state_SingleQuote) // checks if the current token being created is in the single quote state
			{
				if(tokenType != type_SingleQuote) // checks if the current element of the input string is not a single quote
				{
					append(&currentToken->value, string[index]); // appends the current element of the input string to the value of the current token being created
				}
				else // if the current element of the input string is a single quote
				{
					addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
					currentToken = createToken(); // creates an empty token
					tokenState = state_General;
				}
			}
		}
		
		if(currentToken->value != NULL) // checks if the value of the current token being created is not empty
		{
			addToken(lexer, currentToken); // adds the current token being created to the end of the lexer
		}
		
		if(tokenState != state_General) // checks if the last token created was in the general state
		{
			clearLexer(lexer);
			return false; // signals that the input string is not valid
		}
		
		return true; // signals that the input string is valid
	}
	else // if the input parameters are empty
	{
		printf("-msh: lexer: fillLexer() received NULL parameter.\n");
	}
	
	return false; // returns false if the input string is not valid or if either of the input parameters are empty
}

/***********************************************************************************
 * Summary:		copies the token stream in a lexer to an array of strings
 * Parameters:	lexer - the address of the lexer to copy the token stream from
 *				array - the address of the string array to copy the token stream to
***********************************************************************************/
void copyLexer(lexer_t *lexer, char ***array)
{
	if(lexer != NULL && *array != NULL) // checks if the input parameters are not empty
	{
		if(lexer->head != NULL) // checks if the input lexer is not empty
		{
			int index = 0; // stores the index of the current element in the input array
			token_t *currentToken = lexer->head; // stores the address of the current token being copied in the input lexer
			
			/* --- Should I clear the array here first? --- */
			*array = malloc((lexer->size + 1) * sizeof(char*)); // allocates memory for one more string than the size of the input lexer and stores their addresses in memory
			
			while(currentToken != NULL) // loops until the end of the input lexer is reached
			{
				(*array)[index++] = strdup(currentToken->value); // copies the value of the current token in the input lexer to the current element in the input array and traverses to the next element in the input array
				currentToken = currentToken->next; // traverses to the next token in the input lexer
			}
			
			(*array)[index] = NULL;
		}
	}
	else // if the input parameters are empty
	{
		printf("-msh: lexer: copyLexer() received NULL parameter.\n");
	}
}

/***********************************************************************************
 * Summary:		prints the token stream in a lexer to the screen
 * Parameters:	lexer - the address of the lexer to print the token stream from
***********************************************************************************/
void printLexer(lexer_t *lexer)
{
	if(lexer != NULL) // checks if the input parameter is not empty
	{
		if(lexer->head != NULL) // checks if the input lexer is not empty
		{
			token_t *currentToken = lexer->head; // stores the address of the current token being printed in the input lexer
			
			printf("Contents of Lexer:\n");
			
			while(currentToken != NULL) // loops until the end of the input lexer is reached
			{
				printf("\'%s\'\n", currentToken->value); // prints the current token in the input lexer to the screen
				currentToken = currentToken->next; // traverses to the next token in the input lexer
			}   
		}
		else // if the input lexer is empty
		{
			printf("The lexer is empty.\n");
		}
	}
	else // if the input parameter is empty
	{
		printf("-msh: lexer: printLexer() received NULL parameter.\n");
	}
}

/***********************************************************************************
 * Summary:		frees the memory allocated for the token stream in a lexer
 * Parameters:	lexer - the address of the lexer to clear the token stream from
***********************************************************************************/
void clearLexer(lexer_t *lexer)
{
	if(lexer != NULL) // checks if the input parameter is not empty
	{
		token_t *currentToken = lexer->head; // stores the address of the current token being freed in the input lexer
		token_t *previousToken; // stores the address of the previous token in the input lexer

		while(currentToken != NULL) // loops until the end of the input lexer is reached
		{
			previousToken = currentToken;
			currentToken = currentToken->next; // traverses to the next token in the input lexer
			free(previousToken); // frees the memory allocated for the previous token in the input lexer
		}

		lexer->head = NULL;
		lexer->size = 0;
	}
	else // if the input parameter is empty
	{
		printf("-msh: lexer: clearLexer() received NULL parameter.\n");
	}
}

/***********************************************************************************
 * Summary:		frees the memory allocated for the token stream in a lexer and frees
 *				the memory allocated for the lexer
 * Parameters:	lexer - the address of the lexer to clear the token stream from and
 *				free from memory
***********************************************************************************/
void destroyLexer(lexer_t *lexer)
{
	if(lexer != NULL) // checks if the input parameter is not empty
	{
		clearLexer(lexer); // frees the memory allocated for the token stream in the input lexer
		free(lexer); // frees the memory allocated for the input lexer
	}
	else // if the input parameter is empty
	{
		printf("-msh: lexer: destroyLexer() received NULL parameter.\n");
	}
}


/***********************************************************************************
 * Summary:		creates and allocates memory for a token and initializes its
 *				variables to default values
 * Returns:		the address of the token that is created
***********************************************************************************/
token_t* createToken()
{
    token_t *token = malloc(sizeof(token_t)); // allocates memory for a token and stores its address in memory
    token->value = NULL;
    token->next = NULL;
    return token;
}

/***********************************************************************************
 * Summary:		adds a token to the end of the token stream in a lexer
 * Parameters:	lexer - the address of the lexer to add a token to
 *				token - the address of the token to add to the end of the token
 *				stream
***********************************************************************************/
void addToken(lexer_t *lexer, token_t *token)
{
	if(lexer != NULL && token != NULL) // checks if the input parameters are not empty
	{
		if(lexer->head != NULL) // checks if the input lexer is not empty
		{
			token_t *currentToken = lexer->head; // stores the address of the current token in the input lexer

			while(currentToken->next != NULL) // loops until the end of the last token of the input lexer is reached
			{
				currentToken = currentToken->next; // traverses to the next token in the input lexer
			}
			
			currentToken->next = token;
			lexer->size++;
		}
		else // if the input lexer is empty
		{
			lexer->head = token;
			lexer->size = 1;
		}
	}
	else // if the input parameters are empty
	{
		printf("-msh: lexer: addToken() received NULL parameter.\n");
	}
}

/***********************************************************************************
 * Summary:		finds the token type of a character
 * Parameters:	character - the character to find the token type of
 * Returns:		the token type of the character
***********************************************************************************/
int getTokenType(char character)
{
    switch(character) // checks if the input character is one of the following types
    {
        case '&': // if it is an ampersand
            return type_Ampersand;
        case '!': // if it is a bang
            return type_Bang;
        case '$': // if it is a dollar sign
            return type_DollarSign;
        case '\"': // if it is a double quote
            return type_DoubleQuote;
        case '=': // if it is an equals
            return type_Equals;
        case '\\': // if it is an escape
            return type_Escape;
        case '>': // if it is a greater than
            return type_GreaterThan;
        case '<': // if it is a lesser than
            return type_LesserThan;
        case '|': // if it is a pipe
            return type_Pipe;
        case ';': // if it is a semicolon
            return type_Semicolon;
        case '\'': // if it is a single quote
            return type_SingleQuote;
        case ' ': // if it is a space
            return type_Space;
        default: // if it is not one of those
            return type_General;
    }
}

/***********************************************************************************
 * Summary:		appends a character to the end of a string
 * Parameters:	string - the string to append a character to (must be passed by
 *				reference)
 *				character - the character to append to a string
***********************************************************************************/
void append(char **string, char character)
{
    int length = 0; // stores the length of the input string
    char *tempString; // stores the value of the input string

    if(*string != NULL) // checks if the input string is not empty
    {
        length = strlen(*string); // finds the length of the input string
        tempString = strdup(*string); // copies the input string to the temporary string
        free(*string); // frees the memory allocated for the input string
        *string = malloc((length + 1) * sizeof(char)); // allocates memory for a string with one more character than the size of the input string and stores its address in memory
        *string = strdup(tempString); // copies the temporary string to the input string
    }
    else // if the input string is empty
	{
        *string = malloc(2 * sizeof(char)); // allocates memory for a string with two characters and stores its address in memory
	}
	
    (*string)[length] = character;
    (*string)[length + 1] = '\0';
}