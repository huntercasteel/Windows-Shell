#ifndef ALIAS_H
#define ALIAS_H
#include "master.h"

typedef struct AliasList alist_t; // creates a struct AliasList and defines it as a new data type, alist_t
typedef struct Alias alias_t; // creates a struct Alias and defines it as a new data type, alias_t

struct AliasList // a data structure that can store, modify, and remove aliases as a linked list
{
    alias_t *head; // stores the address of the first alias in the linked list
	int size; // stores the amount of aliases in the alias list
};

struct Alias // a data structure that can store the name and value of an alias and point to another alias to form a linked list
{
    char *name; // stores the name of the alias
    char *value; // stores the value of the alias
    alias_t *next; // stores the address of the next alias in the linked list
};

alist_t* createAliasList();
alias_t* searchAliasList(alist_t *alist, char *name);
void printAliasList(alist_t *alist);
void clearAliasList(alist_t *alist);
void destroyAliasList(alist_t *alist);
alias_t* createAlias(char *name, char *value);
void addAlias(alist_t *alist, alias_t *alias);
void removeAlias(alist_t *alist, alias_t *alias);

#endif