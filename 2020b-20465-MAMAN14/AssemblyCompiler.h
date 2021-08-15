#ifndef ASSEMBLY_COMPILER_H
#define ASSEMBLY_COMPILER_H

#include <stdio.h>

#include "General.h"
#include "NodesList.h"
#include "InformationTranslator.h"

#define COMMENT_LINE_START ';'
#define GUIDELINE_LINE_START '.'

void compileAssembly(char* name, NodesList* lines);
void firstRun(InformationHolder* holder, NodesList*);
void secondRun(char* name, InformationHolder* holder, NodesList*);

/* Checks if the symbols that were used in the arguments exist */
void checkSymbolsUsedInArguments(InformationHolder* holder);

/* adds the number of instructions to each address of the data symbols */
void addICToDataCounter(NodesList* dataSymbols, int counterToAdd);

/* Sets for each entry the address where it was defined */
void setEntriesValues(InformationHolder* holder);

/* printing error incase there is insthead of creating output files */
void printErrors(char* name, InformationHolder* holder);

void printError(Node* errorNode);

#endif
