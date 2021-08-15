#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/*Setting constants*/
#define EXIT 0
#define NAME "My complex calculator"
#define VER "MMN22"
#define LINE "-----"
#define ZERO 0
#define COMMA ","

/*Setting enums*/
/*Errors and System Messages*/
enum err
{
    CMD_NOT_EXIST,
    INVALID_ARGS,
    CMD_LONG,
    VAR_NOT_EXIST,
    LOW_ARGS
};
enum sys
{
    START,
    HELP,
    STOP
}; 

enum inputValidation
{
    INVALID,
    VALID
};
enum returnFunction
{
    FALSE,
    TRUE
};
enum inputState
{
    WAIT_FOR_CMD_NAME,
    WAIT_FOR_VAR,
    WAIT_FOR_PARAMS,
    ERROR
};
enum inputParams
{
    VAR,
    ARGS,
    SCALAR,
    NONE
};
enum spaceFlags
{
    BEFORE,
    AFTER
};


/*  In&output manager  */
void startUI();
void handleErrorInput(char *command, int msgIndex);
void clearBuffer();
int stateAsCmd(int indexOfCmd);
void clearCommandString(char *command);

/* Simple UI */
#define FOREVER for (;;)
#define MAX_SCALAR 4
#define SPACEBTN 32
#define ENTERBTN 10
#define MAX_SPACE_BEFORE 1
#define MAX_SPACE_AFTER 2
#define MIN_ARGS 3
#define MAX_COMMAND 30
#define MIN_COMMAND 4

#endif
