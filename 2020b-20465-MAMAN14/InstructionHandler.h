#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "General.h"
#include "NodesList.h"
#include "CommandLine.h"

/* setting register name and value */
#define REG0 "r0"
#define REG0_VALUE 0

#define REG1 "r1"
#define REG1_VALUE 1

#define REG2 "r2"
#define REG2_VALUE 2

#define REG3 "r3"
#define REG3_VALUE 3

#define REG4 "r4"
#define REG4_VALUE 4

#define REG5 "r5"
#define REG5_VALUE 5

#define REG6 "r6"
#define REG6_VALUE 6

#define REG7 "r7"
#define REG7_VALUE 7


typedef struct {
	int code;
	int numberOfArguments;
	int funct;
	bool(*correctArgumentsTypes)(NodesList*);
} Command;

typedef struct {
	Command* command;
	NodesList* arguments; /* each node name is the argument it self, and the value of the argument is the addressing type value */
} Instruction;

typedef struct
{
	NodesList* symbols;
	NodesList* commands;
	NodesList* registers;
	NodesList* instructions;
	NodesList* symbolsUsedInArguments;
	int counter;
	ErrorHolder* errorHolder;
} InstructionsHolder;

Command* searchCommand(NodesList* commands, char* name);

InstructionsHolder* createInstructions();
Instruction* createInstruction(Command* command, NodesList* arguments);
void freeInstructions(InstructionsHolder* holder);
void freeInstructionsList(NodesList* instructions);

void handleInstruction(InstructionsHolder* holder, AssemblyLine* assemblyLine, char** lastArgument);
bool extractCommandParams(NodesList* arguments, AssemblyLine* assemblyLine, InstructionsHolder* holder);
int extractCommandReturnTimes(char* assemblyLine);
int getAddressingType(AssemblyLine* assemblyLine, char* argument, InstructionsHolder* holder);
char* getCommandName(char** linePtr);
void addInstructionSymbol(InstructionsHolder* holder, AssemblyLine* assemblyLine);
void addRegisters(NodesList* registers);
void addRegister(NodesList* registers, char* registerName, int registerValue);
void addCommands(NodesList* commands);
void addCommand(NodesList* commands, char* commandName, int code, int numberOfArguments, int funct, bool(*checkArguments)(NodesList*));
Command* createCommand(int code, int numberOfArguments, int funct, bool(*checkArguments)(NodesList*));

#endif 
