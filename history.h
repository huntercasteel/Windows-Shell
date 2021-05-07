// history.h
////////////////////////////////////////////////////////////////////////////////////
#ifndef HISTORY_H
#define HISTORY_H
#include "master.h"

typedef struct History history_t; // defines struct History as a new type, history_t
typedef struct Command command_t; // defines struct Command as a new type, command_t

struct History /* a data structure that can track the current command-line number
                  and store commands as a queue */
{
    command_t *head; // stores the address of the first command in the queue
    int size; // stores the amount of commands in history
    int line; // stores the current command-line number
};

struct Command /* a data structure that can store the line and value of a command
                  and point to another command to form a queue */
{
    int line; // stores the command-line number of the command
    char *value; // stores the value of the command
    command_t *next; // stores the address of the next command in the linked list
};

history_t* createHistory();
command_t* searchHistory(history_t *history, int line);
void printHistory(history_t *history);
void clearHistory(history_t *history);
void destroyHistory(history_t *history);
command_t* createCommand(int line, char *value);
void addCommand(history_t *history, command_t *command);

#endif