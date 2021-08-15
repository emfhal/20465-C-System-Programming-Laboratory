#ifndef INFORMATION_TRANSLATOR_H
#define INFORMATION_TRANSLATOR_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "InstructionHandler.h"
#include "GuidelineHandler.h"
#include "NodesList.h"
#include "General.h"

typedef struct
{
	InstructionsHolder* instructions;
	GuidelineHolder* data;
	ErrorHolder* errorHolder;
} InformationHolder;

InformationHolder * createInformationHolder();
void freeInformationHolder(InformationHolder* holder);

NodesList* translateInformation(InstructionsHolder* instructionsHolder, GuidelineHolder* guidelineHolder, NodesList* dataSymbols, NodesList* externSymbols);

void translateInstructions(int* wordAddress, NodesList* words, InstructionsHolder* holder, NodesList* dataSymbols, NodesList* externSymbols);

void translateData(int* wordAddress, NodesList* words, GuidelineHolder* holder, int counterToAdd);

FirstWord* createFirstWord(int destination,int destinationRegister, int source, int sourceRegister, int opcode, int funct);

ArgumentWord* argumentToWord(int instructionAddress, int wordAddress, Node* argumentNode, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers);

int distanceBetweensAddress(char* label, int wordAddress, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols);

WordHolder* createWordHolder(int address, int* wordValue);

void addFirstWord(int* wordAddress, NodesList* words, Instruction* instruction, NodesList* registers);

void addArgumentsWords(int instructionAddress, int* wordAddress, NodesList* words, NodesList* arguments, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers);
#endif
