#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "General.h"

#define IMMEDIATE_ADDRESSING_START '#' /*Must to start with # char*/
#define IMMEDIATE_ADDRESSING_VALUE 0

#define DIRECT_ADDRESSING_VALUE 1

#define RELATIVE_ADDRESSING_START '&' /*Must to start with & char*/
#define RELATIVE_ADDRESSING_VALUE 2

#define DIRECT_REGISTER_ADDRESSING_VALUE 3

/* Order by arguments commands amount and set -1 incase there is no funct value at all*/
/* 2 arguments commands */
#define MOV_STR "mov"
#define MOV_CODE 0
#define MOV_ARGS 2
#define MOV_FUNCT -1

#define CMP_STR "cmp"
#define CMP_CODE 1
#define CMP_ARGS 2
#define CMP_FUNCT -1

#define ADD_STR "add"
#define ADD_CODE 2
#define ADD_ARGS 2
#define ADD_FUNCT 1

#define SUB_STR "sub"
#define SUB_CODE 2
#define SUB_ARGS 2
#define SUB_FUNCT 2

#define LEA_STR "lea"
#define LEA_CODE 4
#define LEA_ARGS 2
#define LEA_FUNCT -1

/* 1 arguments commands */

#define CLR_STR "clr"
#define CLR_CODE 5
#define CLR_ARGS 1
#define CLR_FUNCT 1


#define NOT_STR "not"
#define NOT_CODE 5
#define NOT_ARGS 1
#define NOT_FUNCT 2

#define INC_STR "inc"
#define INC_CODE 5
#define INC_ARGS 1
#define INC_FUNCT 3

#define DEC_STR "dec"
#define DEC_CODE 5
#define DEC_ARGS 1
#define DEC_FUNCT 4

#define JMP_STR "jmp"
#define JMP_CODE 9
#define JMP_ARGS 1
#define JMP_FUNCT 1

#define BNE_STR "bne"
#define BNE_CODE 9
#define BNE_ARGS 1
#define BNE_FUNCT 2

#define JSR_STR "jsr"
#define JSR_CODE 9
#define JSR_ARGS 1
#define JSR_FUNCT 3

#define RED_STR "red"
#define RED_CODE 12
#define RED_ARGS 1
#define RED_FUNCT -1

#define PRN_STR "prn"
#define PRN_CODE 13
#define PRN_ARGS 1
#define PRN_FUNCT -1

/* 0 arguments commands */

#define RTS_STR "rts"
#define RTS_CODE 14
#define RTS_ARGS 0
#define RTS_FUNCT -1

#define STOP_STR "stop"
#define STOP_CODE 15
#define STOP_ARGS 0
#define STOP_FUNCT -1

/* Create AssemblyLine by setting the labelname of the beginig of the line the number and orignal */

typedef struct _assemblyLine {
	char* originalLinePtr;
	char* labelName;
	char* line;
	int lineNumber;	
} AssemblyLine;

AssemblyLine* createAssemblyLine(char* line, int lineNumber);

/* For each command we have a function that checking the types of the arguments  (in the last 2 there is no arguments at all so they will return true automaticly) */

bool checkMovArgs(NodesList* arguments);
bool checkCmpArgs(NodesList* arguments);
bool checkAddArgs(NodesList* arguments);
bool checkSubArgs(NodesList* arguments);
bool checkNotArgs(NodesList* arguments);
bool checkClrArgs(NodesList* arguments);
bool checkLeaArgs(NodesList* arguments);
bool checkIncArgs(NodesList* arguments);
bool checkDecArgs(NodesList* arguments);
bool checkJmpArgs(NodesList* arguments);
bool checkBneArgs(NodesList* arguments);
bool checkJsrArgs(NodesList* arguments);
bool checkRedArgs(NodesList* arguments);
bool checkPrnArgs(NodesList* arguments);
bool checkRtsArgs(NodesList* arguments);
bool checkStopArgs(NodesList* arguments);


#endif
