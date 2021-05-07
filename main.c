#include "master.h"
//#include "history.h"
#include "lexer.h"
//#include "alias.h"

void initializeShell();
void getString(char *string);
void processTokens(char **tokens, int tokenCount);
void printTokens(char **tokens);
void extendTokens(char ***tokens, int tokenCount, int tokenIndex);
void destroyTokens(char **tokens);
void executeTokens_Pipe(char **tokens, int pipeCount);
bool executeTokens_Custom(char **tokens);
void executeTokens_Simple(char **tokens);
void deinitializeShell(int status);

/*----- Global Data Structures -----*/
FILE *runCommands;
lexer_t *lexer;
//alist_t *alist;
extern char **environ;

int main()
{
	initializeShell();
	char *string;
	char **tokens;
	
	/*if(runCommands != NULL) // checks if the .*rc file was opened successfully
	{
		while(fgets(string, MAX_STRING, runCommands) != NULL) // loops until the end of the .*rc file is reached
        {
			if(!fillLexer(lexer, string)) // lexically analyzes string, then checks if string is invalid
            {
				printf("-msh: invalid string\n");
				break;
			}
			
			copyLexer(lexer, &tokens);
			processTokens(tokens, lexer->size);
			destroyTokens(tokens);
		}
	}*/
	
	while(true) // loops until the user inputs "exit" or the shell crashes
	{
		getString(string);
		
		if(!fillLexer(lexer, string)) // lexically analyzes string, then checks if string is invalid
		{
			printf("-msh: invalid string\n");
			continue;
		}
		//printLexer(lexer);
		copyLexer(lexer, &tokens);
		//printTokens(tokens);
		processTokens(tokens, lexer->size);
		destroyTokens(tokens);
	}
	
	deinitializeShell(EXIT_SUCCESS); // ensures that memory is freed and the shell exits properly in case the while loop is left without inputting "exit" or a crash
}

/***********************************************************************************
 * Summary:		prints the shell banner and allocates memory for and initializes
 *				all global data structures
***********************************************************************************/
void initializeShell()
{
	printf("\e[1;1H\e[2J"); // clears text on the screen
	printf("|###############################|\n");
	printf("|                               |\n");
	printf("|            MyShell            |\n");
	printf("|   Created by Hunter Casteel   |\n");
	printf("|                               |\n");
	printf("|###############################|\n\n");
	
	//runCommands = fopen(RUN_COMMANDS, "r"); // opens the .*rc file for reading only
	
	if((lexer = createLexer()) == NULL) // creates and allocates memory for lexer, initializes its variables to default values, and checks if the operation failed
	{
		printf("-msh: lexer: createLexer() failed.\n");
		deinitializeShell(EXIT_FAILURE);
	}
	
	/*if((alist = createAliasList()) == NULL) /* initializes alist, then checks if alist was initialized successfully
	{
		printf("-msh: alist: createAliasList() failed\n");
		deinitializeShell(EXIT_FAILURE);
	}*/
}

/***********************************************************************************
 * Summary:		prompts the user for input, collects input from stdin, then stores
 *				the input in a dynamically allocated string
 * Parameters:	string - the dynamically allocated string to perform this function
 *				on
***********************************************************************************/
void getString(char *string)
{
	char *newline;
	printf("\n\nDEBUG1\n\n");
	do
	{
		printf("\n\nDEBUG2\n\n");
		printf("-msh-2.0$ ");
		printf("\n\nDEBUG3\n\n");
		free(string);
		printf("\n\nDEBUG4\n\n");
		fgets(string, MAX_STRING, stdin); // collects user input from stdin, then stores the user input in string
		printf("\n\nDEBUG5\n\n");
	}
	while(true/*!strcmp(string, "\n")*/); // loops until the user inputs at least one non-newline character
	
	
	if(!strcmp(string, "")) // checks if string is empty
	{
		printf("\nGoodbye.\n");
		deinitializeShell(EXIT_SUCCESS);
	}
	
	if((newline = strchr(string, '\n')) != NULL) // sets newline to the address of the first newline character in string, then checks if a newline character was found
	{
		*newline = '\0';
	}
	else // if a newline character wasn't found
	{
		printf("-msh: input exceeds %d characters\n", MAX_STRING);
		deinitializeShell(EXIT_FAILURE);
	}
}

/***********************************************************************************
 * Summary:     separates a string array at semicolons recursively and tracks the
 *              number of pipes in the array, then passes the array to be executed
 * Parameters:  tokens - the dynamically allocated string array to perform this
 *                       function on
 *              tokenCount - the number of elements in tokens
***********************************************************************************/
void processTokens(char **tokens, int tokenCount)
{
    char **tempTokens;
    int i, x, pipeCount = 0;
    //extendTokens(&tokens, tokenCount, 0);

    for(i = 0; i < tokenCount; i++) /* loops until the last element of tokens is
                                       reached */
    {
        if(!strcmp(tokens[i], ";")) //checks if a semicolon was found
        {
            tempTokens = malloc((i + 1) * sizeof(char*));

            for(x = 0; x < i; x++) //loops until the semicolon is reached
                tempTokens[x] = strdup(tokens[x]);

            tempTokens[x] = NULL;
            processTokens(tempTokens, i);
            destroyTokens(tempTokens);
            tempTokens = malloc((tokenCount - i) * sizeof(char*));

            for(x = i + 1; x < tokenCount; x++) /* loops until the last element of
                                                   tokens is reached starting from
                                                   after the semicolon */
                tempTokens[x - i - 1] = strdup(tokens[x]);

            tempTokens[x - i - 1] = NULL;
            processTokens(tempTokens, tokenCount - i - 1);
            destroyTokens(tempTokens);
            return;
        }

        if(!strcmp(tokens[i], "|")) //checks if a pipe was found
        {
            pipeCount++;
            //extendTokens(&tokens, tokenCount, i + 1);
        }
    }

    if(pipeCount > 0) //checks if a pipe was found
        executeTokens_Pipe(tokens, pipeCount);
    else if(!executeTokens_Custom(tokens)) //checks if a custom command wasn't found
        executeTokens_Simple(tokens);
}

/***********************************************************************************
 * Summary:     prints the elements of a string array
 * Parameters:  tokens - the dynamically allocated string array to perform this
 *                       function on
***********************************************************************************/
void printTokens(char **tokens)
{
    int index;
    printf("Contents of Tokens:\n");

    for(index = 0; tokens[index] != NULL; index++) /* loops until the last element
                                                      of tokens is reached */
        printf("\'%s\'\n", tokens[index]);
    
    printf("\'%s\'\n", tokens[index]);
}

void extendTokens(char ***tokens, int tokenCount, int tokenIndex)
{
    /*alias_t *alias = searchAliasList(alist, (*tokens)[tokenIndex]);

    if(alias != NULL)
    {
        int index;
        token_t *token;

        fillLexer(lexer, alias->value);
        *tokens = realloc(*tokens, (tokenCount + lexer->size - 1) * sizeof(char*));
        free((*tokens)[tokenIndex]);

        for(index = tokenCount; index > tokenIndex; index--)
        {
            if((*tokens)[index] != NULL)
            {
                (*tokens)[index + lexer->size - 1] = strdup((*tokens)[index]);
                free((*tokens)[index]);
            }
            else
                (*tokens)[index + lexer->size - 1] = (*tokens)[index];
        }

        token = lexer->head;

        for(index = tokenIndex; token != NULL; index++)
        {
            (*tokens)[index] = strdup(token->value);
            token = token->next;
        }
    }*/
}

/***********************************************************************************
 * Summary:     frees the memory allocated for the elements of an array, then frees
 *              the memory allocated for the array
 * Parameters:  tokens - the dynamically allocated string array to perform this
 *                       function on
***********************************************************************************/
void destroyTokens(char **tokens)
{
    int index;

    for(index = 0; tokens[index] != NULL; index++) /* loops until the last element
                                                      of tokens is reached */
        free(tokens[index]);

    free(tokens);
}

void executeTokens_Pipe(char **tokens, int pipeCount)
{
    pid_t forkPID;
    int i, x = 1, y;
    int pipeFD[2 * pipeCount];
    int commandLocation[pipeCount + 1];
    commandLocation[0] = 0;

    for(i = 0; i < pipeCount; i++)
        if(pipe(pipeFD + i * 2) < 0)
        {
            printf("-msh: pipe() failed\n");
            deinitializeShell(EXIT_FAILURE);
        }

    for(i = 0; tokens[i] != NULL; i++)
        if(!strcmp(tokens[i], "|"))
        {
            tokens[i] = NULL;
            commandLocation[x++] = i + 1;
        }

    x = 0;

    for(i = 0; i < pipeCount + 1; i++)
    {
        if((forkPID = fork()) < 0)
        {
            printf("-msh: pipe() failed\n");
            deinitializeShell(EXIT_FAILURE);
        }

        if(forkPID == 0)
        {
            if(i < pipeCount)
                if(dup2(pipeFD[x + 1], 1) < 0)
                {
                    printf("-msh: dup2() failed\n");
                    deinitializeShell(EXIT_FAILURE);
                }

            if(x != 0)
                if(dup2(pipeFD[x - 2], 0) < 0)
                {
                    printf("-msh: dup2() failed\n");
                    deinitializeShell(EXIT_FAILURE);
                }
            
            for(y = 0; y < 2 * pipeCount; y++)
                close(pipeFD[y]);
                 
            if(execvp(tokens[commandLocation[i]], (tokens + commandLocation[i])) < 0 )
            {
                printf("-msh: %s: command not found\n", *(tokens + commandLocation[i]));
                exit(EXIT_FAILURE); //exits the child process and signals an error
            }
        }

        x += 2;
    }

    for(i = 0; i < 2 * pipeCount; i++)
        close(pipeFD[i]);

    for(i = 0; i < pipeCount + 1; i++)
        wait(NULL);
}

/***********************************************************************************
 * Summary:     checks the elements of a string array for custom commands, then
 *              executes them
 * Parameters:  tokens - the dynamically allocated string array to perform this
 *                       function on
 * Returns:     a bool that indicates if a custom command was executed or not
***********************************************************************************/
bool executeTokens_Custom(char **tokens)
{
    if(!strcmp(tokens[0], "exit")) // checks if the first token is exit
    {
        printf("Goodbye.\n");
        deinitializeShell(EXIT_SUCCESS);
    }

    /*if(!strcmp(tokens[0], "alias")) // checks if the first token is alias
    {
        int index;
        alias_t *alias;

        if(tokens[1] != NULL) // checks if the second token isn't empty
        {
            for(index = 1; tokens[index] != NULL; index++) // loops until the last element of tokens is reached
            {
                alias = searchAliasList(alist, tokens[index]);

                if(tokens[index + 1] != NULL && !strcmp(tokens[index + 1], "="))
                {
                    if(alias == NULL) // checks if tokens[index] wasn't found
                    {
                        alias = createAlias(tokens[index], "");
                        addAlias(alist, alias);
                    }

                    if(tokens[index + 2] != NULL) /* checks if the value token isn't
                                                     empty
                    {
                        free(alias->value);
                        alias->value = strdup(tokens[index + 2]);
                        index += 2;
                    }
                    else // if the value token is empty
                    {
                        free(alias->value);
                        alias->value = strdup("");
                        index++;
                    }
                }
                else // if the equal token is empty or isn't an equal
                {
                    if(alias != NULL) // checks if tokens[index] was found
                        printf("alias %s='%s'\n", alias->name, alias->value);
                    else // if tokens[index] wasn't found
                        printf("-msh: alias: %s: not found\n", tokens[index]);
                }
            }
        }
        else // if the second token is empty
            printAliasList(alist);

        return true;
    }

    if(!strcmp(tokens[0], "unalias")) // checks if the first token is unalias
    {
        int index;
        alias_t *alias;

        if(tokens[1] != NULL) // checks if the second token isn't empty
        {
            if(!strcmp(tokens[1], "-a")) // checks if the second token is -a
                clearAliasList(alist);
            else // if the second token isn't -a
            {
                for(index = 1; tokens[index] != NULL; index++) /* loops until the
                                                                  last element of
                                                                  tokens is reached
                                                                  
                {
                    alias = searchAliasList(alist, tokens[index]);

                    if(alias != NULL) // checks if tokens[index] was found
                        removeAlias(alist, alias);
                    else // if tokens[index] wasn't found
                        printf("-msh: unalias: %s: not found\n", tokens[index]);
                }
            }
        }
        else // if the second token is empty
            printf("unalias: usage: unalias [-a] name [name ...]\n");

        return true;
    }*/

    if(!strcmp(tokens[0], "export")) // checks if the first token is export
    {
        int index;
        char *currentEnviron;

        if(tokens[1] != NULL) // checks if the second token isn't empty
        {
            for(index = 1; tokens[index] != NULL; index++) /* loops until the last
                                                              element of tokens is
                                                              reached */
            {
                if(tokens[index + 1] != NULL && !strcmp(tokens[index + 1], "="))
                {
                    if(tokens[index + 2] != NULL) /* checks if the value token isn't
                                                     empty */
                    {
                        setenv(tokens[index], tokens[index + 2], true);
                        index += 2;
                    }
                    else // if the value token is empty
                    {
                        setenv(tokens[index], "", true);
                        index++;
                    }
                }
                else // if the equal token is empty or isn't an equal
                {
                    printf("\nDEBUG %d\n", index);
                    printf("ADDING: %s\n", tokens[index]);
                    setenv(tokens[index], "", true);
                    printf("\nSECOND %d\n", index);
                }
            }
        }
        else // if the second token is empty
        {
            currentEnviron = *environ;

            for(index = 1; currentEnviron; index++)
            {
                printf("declare -x %s\n", currentEnviron);
                currentEnviron = *(environ + index);
            }
        }

        return true;
    }

    return false;
}

/***********************************************************************************
 * Summary:     creates a child process using fork(), then executes the elements of
 *              a string array using execvp()
 * Parameters:  tokens - the dynamically allocated string array to perform this
 *                       function on
***********************************************************************************/
void executeTokens_Simple(char **tokens)
{
    pid_t forkPID = fork(); // creates a child process

    if(forkPID < 0) // checks if fork() was unsuccessful
    {
        printf("-msh: fork() failed\n");
        deinitializeShell(EXIT_FAILURE);
    }

    if(forkPID == 0) // checks if forkPID is the child process
    {
        if(execvp(tokens[0], tokens) < 0) /* executes the elements of tokens, then
                                             checks if execvp() was unsuccessful */
        {
            printf("-msh: %s: command not found\n", *tokens);
            exit(EXIT_FAILURE); // exits the child process and signals an error
        }
    }
    else // if forkPID is the shell
        wait(NULL); // waits for the child process to finish
}

/***********************************************************************************
 * Summary:     frees the memory allocated for the global data structures, then
 *              exits the shell with a status code
 * Parameters:  status - the status code used to exit the shell
***********************************************************************************/
void deinitializeShell(int status)
{
    //fclose(runCommands);
    destroyLexer(lexer);
    //destroyAliasList(alist);
    exit(status);
}