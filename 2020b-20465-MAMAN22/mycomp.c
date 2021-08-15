#include "complex.h"
#include "common.h"

/*List of all user errors displayed*/
char *err[] = {
    "*WARNING: command %s does not exist\n",
    "*WARNING: invalid %s\n",
    "*WARNING: command too %s!!\n",
    "*WARNING: variable %s does not exist!\n",
    "*WARNING: missing parameter %s\n",
};

/*List of system messages rule*/
char *sys[] = {
    "Please type the function name and parameters according to the following format.\nUsage [function_name] [variable],[parameters:optionl]",
    "The available functions:\nread_comp           [complex number],[parameter],[parameter]\nprint_comp          [complex number]\nadd_comp            [complex number],[complex number]\nsub_comp            [complex number]\nmult_comp_real      [complex number],[scalar]\nmult_comp_img       [complex number],[scalar]\nmult_comp_comp      [complex number],[complex number]\nabs_comp            [complex number]\nstop                [none]",
    "WARNING: Program stoped, press enter key for exit"};

complex a, b, c, d, e, f;

/*Array for store the reference complex variabls (A-F)*/
struct
{
    char *name;
    complex *var;
} storage[] = {
    {"A", &a},
    {"B", &b},
    {"C", &c},
    {"D", &d},
    {"E", &e},
    {"F", &f},
    {"0", NULL}};

/*Array for store all complex function*/
struct
{
    char *name;
    void (*func)();
    int funcParam;
} cmd[] = {
    {"read_comp", read_comp, ARGS},
    {"print_comp", print_comp, NONE},
    {"add_comp", add_comp, VAR},
    {"sub_comp", sub_comp, VAR},
    {"mult_comp_real", mult_comp_real, SCALAR},
    {"mult_comp_img", mult_comp_img, SCALAR},
    {"mult_comp_comp", mult_comp_comp, VAR},
    {"abs_comp", abs_comp, NONE},
    {"stop", NULL, NONE},
    {"not_valid", NULL, NONE}};

int main()
{
    startUI();
    return EXIT;
}

void startUI()
{
    char command[MAX_COMMAND];
    /*define and set state for system*/
    int state = WAIT_FOR_CMD_NAME, paramState = NONE,
        funIndex = 0, varIndex = 0;
    int i, c = 0;
    char ch;
    char *cmdName = "", *varName = "", *rest = "";

    /*Print wellcome message*/
    printf("%s\n", LINE);
    printf("\t%s, %s\n", NAME, VER);
    printf("%s\n", LINE);

    printf("%s\n\n", sys[START]);
    printf("%s\n%s\n", sys[HELP], LINE);

    /*Get user commands*/
    FOREVER
    {
        clearCommandString(command);
        /*Scan all argument until user press enter button*/
        for (i = 0; c != ENTERBTN; i++)
        {
            c = getchar();
            if (c == ENTERBTN)
                break;
            ch = c;
            command[i] = ch;
            if (i > MAX_COMMAND)
            {
                handleErrorInput("long", CMD_LONG);
                state = ERROR;
                clearBuffer();
                break;
            }
        }

        /*Incase the user enter stop, stop running*/
        if (strcmp(command, "stop") == ZERO)
        {
            printf("%s", sys[STOP]);
            {
                int decision = getchar();
                if (decision == ENTERBTN)
                    break;
                else
                    state = ERROR;
            }
        }
        /*Add min-length command validation*/
        if (i < MIN_COMMAND)
        {
            handleErrorInput("short", CMD_LONG);
            state = ERROR;
        }
        c = 0;

        /*CHECK-COMMAND*/

        if (state == WAIT_FOR_CMD_NAME)
        {

            int commandLength, funcLength, varLength;
            commandLength = strlen(command);

            cmdName = strtok(command, " ");
            funcLength = strlen(cmdName);

            if (commandLength - funcLength >= 2)
            {
                varName = strtok(NULL, ",");
                varLength = strlen(varName);

                if (commandLength - funcLength - varLength >= 2)
                    rest = strtok(NULL, "");
            }
            else
            {
                state = ERROR;
                handleErrorInput("parameters", LOW_ARGS);
            }
        }

        /*FIND-FUNCITON*/
        if (state != ERROR)
        {
            /*through over all command list*/
            for (i = 0; cmd[i].func != NULL; i++)
                if (strcmp(cmdName, cmd[i].name) == ZERO)
                    break;

            if (cmd[i].func == NULL)
            {
                state = ERROR;
                handleErrorInput(cmdName, CMD_NOT_EXIST);
            }
            else
            {
                state = WAIT_FOR_VAR;
                funIndex = i;
            }
        }

        /*VAR-NAME*/

        /*Through over all variable list*/
        if (state == WAIT_FOR_VAR)
        {
            for (i = 0; storage[i].var != NULL; i++)
            {
                if (strcmp(varName, storage[i].name) == ZERO)
                    break;
            }
            if (storage[i].var == NULL)
            {
                state = WAIT_FOR_CMD_NAME;
                handleErrorInput(varName, VAR_NOT_EXIST);
            }
            else
            {
                paramState = stateAsCmd(funIndex);
                state = WAIT_FOR_PARAMS;
                varIndex = i;
            }
        }

        /*Scan variables from input*/
        if (state == WAIT_FOR_PARAMS)
        {
            switch (paramState)
            {
            case NONE:
            {
                /*call command function with current variable*/
                cmd[funIndex].func(storage[varIndex].var);
                break;
            }

            case ARGS:
            {
                int restLength = strlen(rest);
                double a, b;
                if (restLength < MIN_ARGS)
                {
                    state = ERROR;
                    handleErrorInput("parameters", LOW_ARGS);
                    break;
                }
                a = atof(strtok(rest, ","));
                b = atof(strtok(NULL, ","));
                cmd[funIndex].func(storage[varIndex].var, a, b);
                printf("Read to variable %s, two inputs: %.2f, %.2f\n", storage[varIndex].name, a, b);
                break;
            }

            case VAR:
            {
                int len;
                len = strlen(rest);
                if (len > 1)
                    handleErrorInput("number of arguments after variable", INVALID_ARGS);
                else
                {
                    for (i = 0; storage[i].var != NULL; i++)
                    {
                        if (strcmp(rest, storage[i].name) == ZERO)
                            break;
                    }
                    if (storage[i].var == NULL)
                        handleErrorInput(rest, VAR_NOT_EXIST);
                    else
                        cmd[funIndex].func(storage[varIndex].var, storage[i].var);
                }

                break;
            } 

            case SCALAR:
            {
                int len;
                len = strlen(rest);
                if (len > MAX_SCALAR)
                    handleErrorInput("number of arguments after variable", INVALID_ARGS);
                else
                {
                    char isDigitFlag = rest[0];
                    if (!isDigitFlag)
                    {
                        handleErrorInput("scalar argument, inster real number please", INVALID_ARGS);
                        break;
                    }
                    else
                    {
                        double num = atof(rest);
                        cmd[funIndex].func(storage[varIndex].var, num);
                    }
                }

                break;
            }
            }
        }
        varName = "";
        rest = "";
        state = WAIT_FOR_CMD_NAME;

    } /*END LOOP/FOREVER*/
}

/*The number of state according cmd name*/
int stateAsCmd(int indexOfCmd)
{
    return cmd[indexOfCmd].funcParam;
}

/*Handle error messages*/
void handleErrorInput(char *command, int msgIndex)
{
    printf("\n%s\n", LINE);
    printf(err[msgIndex], command);
    printf("%s\n\n", LINE);
}

/*Clear all char/string in buffer for new input*/
void clearBuffer()
{
    int c;
    c = getchar();
    while (c)
        if (c == ENTERBTN)
        {
            return;
        }
        else
            c = getchar();
}

/*Clear command string*/
void clearCommandString(char *command)
{
    int i;
    for (i = 0; i < MAX_COMMAND; i++)
    {
        command[i] = 0;
    }
}
