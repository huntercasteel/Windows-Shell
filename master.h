#ifndef MASTER_H
#define MASTER_H

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_DIGITS 3 // defines the max amount of digits permitted in an escape sequence
#define MAX_HISTORY 20 // defines the max amount of commands stored in history
#define MAX_STRING 100 // defines the max amount of characters permitted for receiving a string from stdin and a file
#define RUN_COMMANDS ".mshrc" // defines the filename of run command scripts for the shell

#endif