#include "CommandLine.h"

/* here will will create by line number and line char pointer an AssemblyLine*/
AssemblyLine* createAssemblyLine(char* line, int lineNumber) {
	AssemblyLine* aLine = (AssemblyLine*)safeMalloc(sizeof(AssemblyLine));
	char* newLine = (char*)safeMalloc(sizeof(char) * (strlen(line) + 1));
	memcpy(newLine, line, sizeof(char)* (strlen(line) + 1));
	aLine->line = newLine;
	aLine->originalLinePtr = newLine;
	aLine->lineNumber = lineNumber;
	return aLine;
}

/* here we will cheack for every command type the addressing value by the source and destinatio arguments if exsit (incase is stop or rts command return true for zero arg)*/
bool checkMovArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkCmpArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument ||  DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(IMMEDIATE_ADDRESSING_VALUE == destinationArgument || DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkAddArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkSubArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkLeaArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkClrArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkNotArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkIncArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkDecArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}


bool checkJmpArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || RELATIVE_ADDRESSING_VALUE == destinationArgument);
}

bool checkBneArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || RELATIVE_ADDRESSING_VALUE == destinationArgument);
}

bool checkJsrArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || RELATIVE_ADDRESSING_VALUE == destinationArgument);
}

bool checkRedArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkPrnArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == destinationArgument || DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkRtsArgs(NodesList* arguments) {
	return TRUE;
}

bool checkStopArgs(NodesList* arguments) {
	return TRUE;
}

