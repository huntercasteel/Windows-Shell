#include "alias.h"

/***********************************************************************************
 * Summary:     creates and allocates memory for an alias list, then initializes its
				variables to default values
 * Returns:     the address of the alias list that is created
***********************************************************************************/
alist_t* createAliasList()
{
    alist_t *alist = malloc(sizeof(alist_t));
    alist->head = NULL;
    alist->size = 0;
    return alist;
}

/***********************************************************************************
 * Summary:     searches an alias list for an alias with a specific name
 * Parameters:  alist - the alias list to search
 *              name - the name of the alias to search for
 * Returns:     the address of the alias with the specified name or NULL if no
				matching alias is found
***********************************************************************************/
alias_t* searchAliasList(alist_t *alist, char *name)
{
    if(alist != NULL && name != NULL) // checks if the input parameters aren't empty
    {
        alias_t *currentAlias = alist->head; // stores the address of the current alias being checked in the alias list

        while(currentAlias != NULL) // loops until the end of the alias list is reached
        {
            if(!strcmp(currentAlias->name, name)) // checks if the current alias matches the alias being searched for
                return currentAlias;

            currentAlias = currentAlias->next; // traverses to the next alias in the alias list
        }
    }
	else
		printf("[Error: searchAliasList() received NULL parameter]\n");

    return NULL; // returns NULL if a matching alias isn't found or if either of the input parameters are empty
}

/***********************************************************************************
 * Summary:     prints the names and values of the aliases in an alist
 * Parameters:  alist - the alist_t to perform this function on
***********************************************************************************/
void printAliasList(alist_t *alist)
{
    if(alist->head != NULL) // checks if alist isn't empty
    {
        alias_t *currentAlias = alist->head;
        
        while(currentAlias != NULL) /* loops until the end of the linked list is
                                       reached */
        {
            printf("alias %s='%s'\n", currentAlias->name, currentAlias->value);
            currentAlias = currentAlias->next;
        }
    }
}

/***********************************************************************************
 * Summary:     frees the memory allocated for the aliases in an alist
 * Parameters:  alist - the alist_t to perform this function on
***********************************************************************************/
void clearAliasList(alist_t *alist)
{
    alias_t *previousAlias, *currentAlias = alist->head;

    while(currentAlias != NULL) // loops until the end of the linked list is reached
    {
        previousAlias = currentAlias;
        currentAlias = currentAlias->next;
        free(previousAlias);
    }

    alist->head = NULL;
    alist->size = 0;
}

/***********************************************************************************
 * Summary:     frees the memory allocated for the aliases in an alist, then frees
 *              the memory allocated for the alist
 * Parameters:  alist - the alist_t to perform this function on
***********************************************************************************/
void destroyAliasList(alist_t *alist)
{
    if(alist != NULL) // checks if alist was initialized successfully
    {
        alias_t *previousAlias, *currentAlias = alist->head;

        while(currentAlias != NULL) /* loops until the end of the linked list is
                                       reached */
        {
            previousAlias = currentAlias;
            currentAlias = currentAlias->next;
            free(previousAlias);
        }

        free(alist);
    }
}

/***********************************************************************************
 * Summary:     allocates memory for an alias_t, then initializes its variables
 * Parameters:  name - the dynamically allocated string to copy to the name of the
 *                     alias to be created
 *              value - the dynamically allocated string to copy to the value of the
 *                      alias to be created
 * Returns:     a pointer to the alias_t that was created
***********************************************************************************/
alias_t* createAlias(char *name, char *value)
{
    alias_t *alias = malloc(sizeof(alias_t));

    if(name != NULL) // checks if name isn't empty
        alias->name = strdup(name);
    else // if name is empty
        alias->name = NULL;

    if(value != NULL) // checks if value isn't empty
        alias->value = strdup(value);
    else // if value is empty
        alias->value = NULL;

    alias->next = NULL;
    return alias;
}

/***********************************************************************************
 * Summary:     adds an alias to the end of the linked list in an alist
 * Parameters:  alist - the alist_t to perform this function on
 *              alias - the alias_t to be added to the end of the linked list
***********************************************************************************/
void addAlias(alist_t *alist, alias_t *alias)
{
    if(alist->head != NULL) // checks if alist isn't empty
    {
        alias_t *currentAlias = alist->head;

        while(currentAlias->next != NULL) /* loops until the last alias of the
                                             linked list is reached */
        {
            if(!strcmp(currentAlias->name, alias->name)) /* checks if alias->name
                                                            was found */
            {
                free(currentAlias->value);
                currentAlias->value = strdup(alias->value);
                return;
            }
            
            currentAlias = currentAlias->next;
        }
        
        currentAlias->next = alias;
        alist->size++;
    }
    else // if alist is empty
    {
        alist->head = alias;
        alist->size = 1;
    }
}

/***********************************************************************************
 * Summary:     removes an alias from the linked list in an alist
 * Parameters:  alist - the alist_t to perform this function on
 *              alias - the alias_t to be removed from the linked list
***********************************************************************************/
void removeAlias(alist_t *alist, alias_t *alias)
{
    if(alist->head != NULL) // checks if alist isn't empty
    {
        alias_t *previousAlias, *currentAlias = alist->head;

        while(currentAlias != alias) // loops until alias is reached
        {
            previousAlias = currentAlias;
            currentAlias = currentAlias->next;
        }

        if(currentAlias != alist->head) /* checks if currentAlias isn't the first
                                           alias in the linked list */
            previousAlias->next = currentAlias->next;
        else // if currentAlias is the first alias in the linked list
            alist->head = currentAlias->next;

		free(currentAlias);
    }
}