// history.c
////////////////////////////////////////////////////////////////////////////////////
#include "history.h"

history_t* createHistory()
{
    history_t *history = malloc(sizeof(history_t));
    history->head = NULL;
    history->size = 0;
    history->line = 1;
    return history;
}

command_t* searchHistory(history_t *history, int line)
{
    if(history->head != NULL && line != 0)
    {
        command_t *currentCommand = history->head;
        
        while(currentCommand != NULL)
        {
            if(currentCommand->line == line)
                return currentCommand;

            currentCommand = currentCommand->next;
        }
    }

    return NULL;
}

void printHistory(history_t *history)
{
    if(history->head != NULL)
    {
        command_t *currentCommand = history->head;
    
        while(currentCommand != NULL)
        {
            printf("%d %s\n", currentCommand->line, currentCommand->value);
            currentCommand = currentCommand->next;
        }
    }
}

void clearHistory(history_t *history)
{
    command_t *previousCommand, *currentCommand = history->head;

    while(currentCommand != NULL)
    {
        previousCommand = currentCommand;
        currentCommand = currentCommand->next;
        free(previousCommand);
    }

    history->head = NULL;
    history->size = 0;
}

void destroyHistory(history_t *history)
{
    if(history != NULL)
    {
        command_t *previousCommand, *currentCommand = history->head;

        while(currentCommand != NULL)
        {
            previousCommand = currentCommand;
            currentCommand = currentCommand->next;
            free(previousCommand);
        }

        free(history);
    }
}

command_t* createCommand(int line, char *value)
{
    command_t *command = malloc(sizeof(command_t));
    command->line = line;

    if(value != NULL)
        command->value = strdup(value);
    else
        command->value = NULL;

    command->next = NULL;
    return command;
}

void addCommand(history_t *history, command_t *command)
{
    if(history->head != NULL)
    {
        command_t *currentCommand = history->head;

        while(currentCommand->next != NULL)
            currentCommand = currentCommand->next;

        currentCommand->next = command;
        history->size++;
        history->line++;

        if(history->size > MAX_HISTORY)
        {
            currentCommand = history->head;
            history->head = currentCommand->next;
            free(currentCommand);
            history->size--;
        }
    }
    else
    {
        history->head = command;
        history->size = 1;
    }
}
