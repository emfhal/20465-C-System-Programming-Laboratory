#include "InstructionHandler.h"

void handleInstruction(InstructionsHolder* holder, AssemblyLine* assemblyLine, char** lastArgument) {
	int instructionLength;
	int zeroArguments;
	char* commandName;
	bool success;
	Command* command;
	NodesList* arguments;
	
	zeroArguments = FALSE;
	instructionLength = 1;
	success = TRUE;

	if(assemblyLine->labelName) addInstructionSymbol(holder, assemblyLine);
	commandName = getCommandName(&(assemblyLine->line));

	/* Command without arguments */
	if (!commandName) {
		commandName = assemblyLine->line; 
		zeroArguments = TRUE;
	}
	command = searchCommand(holder->commands, commandName);

	if (NULL == command) {
		addError(holder->errorHolder, "Command name is invalid", assemblyLine->lineNumber);
		return;
	}
	arguments = createNodesList(sizeof(int));
	
	/*extract parameters*/
	if (!zeroArguments)
	{
		success = extractCommandParams(arguments, assemblyLine, holder);
		if (!success)
		{
			*lastArgument = NULL;
			return;
		}
	}
		
	if (arguments->length != command->numberOfArguments) {
		addError(holder->errorHolder, "Number of command arguments was wrong", assemblyLine->lineNumber);
		*lastArgument = NULL;
		return;
	}

	if (!command->correctArgumentsTypes(arguments)) {
		addError(holder->errorHolder, "Wrong type of arguments", assemblyLine->lineNumber);
		*lastArgument = NULL;
		return;
	}

	/*downgrade by one the counter incase the desitionation is register*/
	if (arguments->length >= 1) {
		if (searchNode(holder->registers, arguments->tail->name) != NULL) instructionLength-=1;
	}
 
 	/*downgrade by one the counter incase the source is register*/
	if (arguments->length == 2) {
		if (searchNode(holder->registers, arguments->head->name) != NULL) instructionLength-=1;
	}

	addNode(holder->instructions, NULL, createInstruction(command, arguments));
	holder->counter += arguments->length + instructionLength;
}

Command* searchCommand(NodesList* commands, char* name)
{
	/*
	Search command and relocate memory for him cause of change the ReturnTimes in Instruction
	*/
	Command* toReturn = safeMalloc(sizeof(Command));
	Node* currentNode = commands->head;

	while (currentNode)
	{
		if (startsWith(name, currentNode->name))
		{
			memcpy(toReturn, currentNode->value, sizeof(Command));
			return toReturn;
		}	
		currentNode = currentNode->next;
	}
	return NULL;
}

Instruction* createInstruction(Command* command, NodesList* arguments) {
	Instruction* instruction = (Instruction*)safeMalloc(sizeof(Instruction));
	instruction->command = command;
	instruction->arguments = arguments;
	return instruction;
}

bool extractCommandParams(NodesList* arguments, AssemblyLine* assemblyLine, InstructionsHolder* holder) {
	const char* delimiters = ",";
	char* token;
	char* argument;
	int addressingType;
	int counter=0;

	/* walk through the tokens */
	for (token = strtok(assemblyLine->line, delimiters); token != NULL; token = strtok(NULL, delimiters))
	{
		argument = trim(token);
		addressingType = getAddressingType(assemblyLine, argument, holder);
		addNode(arguments, argument, &addressingType);
		counter+=1;
	}
	if(counter) 
	   return TRUE;
	return FALSE;
}

int getAddressingType(AssemblyLine* assemblyLine, char* argument, InstructionsHolder* holder) {
	char* arg = trim(assemblyLine->originalLinePtr);
	
	if (argument[0] == IMMEDIATE_ADDRESSING_START)
		return IMMEDIATE_ADDRESSING_VALUE; /* =0 */
		
    /*According to the instructions there must be a JUMP JSR BNE command for realative adressing*/
	else if (argument[0] == RELATIVE_ADDRESSING_START && (strcmp(arg,BNE_STR)==0 || strcmp(arg,JSR_STR)==0 || strcmp(arg, JMP_STR)==0)) /* =2 */
		return RELATIVE_ADDRESSING_VALUE;

	else if (searchNode(holder->registers, argument))
		return DIRECT_REGISTER_ADDRESSING_VALUE; /* =3 */
	
	else {
		addNode(holder->symbolsUsedInArguments, argument, &(assemblyLine->lineNumber));
		return DIRECT_ADDRESSING_VALUE; /* =1 */
	}
}

void addInstructionSymbol(InstructionsHolder* holder, AssemblyLine* assemblyLine) {
	if (assemblyLine->labelName) {
		if (searchNode(holder->symbols, assemblyLine->labelName)) /*Return error incase the label was already declared before*/
			addError(holder->errorHolder, "This label was already declared", assemblyLine->lineNumber);
		else if(searchNode(holder->registers, assemblyLine->labelName) || searchNode(holder->commands, assemblyLine->labelName) 
		|| strlen(assemblyLine->labelName) > MAX_LEN_LEBAL || !isalpha(assemblyLine->labelName[0])) 
		/*Return error incase the label is illegal (if is a register  or commands name or the length more than 32 or the first char is not a Alphabeit)*/
			addError(holder->errorHolder, "This label name is illegal", assemblyLine->lineNumber);
		else{
			addNode(holder->symbols, assemblyLine->labelName, &holder->counter);
		}
	}
}

char* getCommandName(char** linePtr) {
	const char commandDelimiter = ' ';
	return splitString(linePtr, commandDelimiter); /*The command name if exsit will come first in the line (after label as well)*/
}

InstructionsHolder* createInstructions() {
	InstructionsHolder* holder = (InstructionsHolder*)safeMalloc(sizeof(InstructionsHolder));
	
	/*create a new NodesList*/
	holder->commands = createNodesList(sizeof(Command));
	holder->symbols = createNodesList(sizeof(int));
	holder->symbolsUsedInArguments = createNodesList(sizeof(int));
	holder->instructions = createNodesList(sizeof(Instruction));
	holder->registers = createNodesList(sizeof(char*));
	
	/*adding values by starting the counter at MEMORY_ADDRESS_START*/
	holder->counter = MEMORY_ADDRESS_START;
	addRegisters(holder->registers);
	addCommands(holder->commands);
	holder->errorHolder = createErrorHolder();
	
	return holder;
}

void freeInstructions(InstructionsHolder* holder) {
	freeNodesList(holder->symbols);
	freeNodesList(holder->commands);
	freeNodesList(holder->registers);
	freeNodesList(holder->symbolsUsedInArguments);
	freeInstructionsList(holder->instructions);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}

void freeInstructionsList(NodesList* instructions) {
	Node* node;
	Instruction* instruction;
	while ( (node = getNext(instructions)) ) {
		instruction = (Instruction*)(node->value);
		freeNodesList(instruction->arguments);
	}
	freeNodesList(instructions);
}
 /* adding registers and commands functions section */

void addRegisters(NodesList* registers) {
	addRegister(registers, REG0, REG0_VALUE);
	addRegister(registers, REG1, REG1_VALUE);
	addRegister(registers, REG2, REG2_VALUE);
	addRegister(registers, REG3, REG3_VALUE);
	addRegister(registers, REG4, REG4_VALUE);
	addRegister(registers, REG5, REG5_VALUE);
	addRegister(registers, REG6, REG6_VALUE);
	addRegister(registers, REG7, REG7_VALUE);
}

void addRegister(NodesList* registers, char* registerName, int registerValue)
{
	addNode(registers, registerName, &registerValue);
}

void addCommands(NodesList* commands) {
	addCommand(commands, MOV_STR, MOV_CODE, MOV_ARGS, MOV_FUNCT, checkMovArgs);
	addCommand(commands, CMP_STR, CMP_CODE, CMP_ARGS, CMP_FUNCT, checkCmpArgs);
	addCommand(commands, ADD_STR, ADD_CODE, ADD_ARGS, ADD_FUNCT, checkAddArgs);
	addCommand(commands, SUB_STR, SUB_CODE, SUB_ARGS, SUB_FUNCT, checkSubArgs);
	addCommand(commands, LEA_STR, LEA_CODE, LEA_ARGS, LEA_FUNCT, checkLeaArgs);

	addCommand(commands, CLR_STR, CLR_CODE, CLR_ARGS, CLR_FUNCT, checkClrArgs);
	addCommand(commands, NOT_STR, NOT_CODE, NOT_ARGS, NOT_FUNCT, checkNotArgs);
	addCommand(commands, INC_STR, INC_CODE, INC_ARGS, INC_FUNCT, checkIncArgs);
	addCommand(commands, DEC_STR, DEC_CODE, DEC_ARGS, DEC_FUNCT, checkDecArgs);

	addCommand(commands, JMP_STR, JMP_CODE, JMP_ARGS, JMP_FUNCT, checkJmpArgs);
	addCommand(commands, BNE_STR, BNE_CODE, BNE_ARGS, BNE_FUNCT, checkBneArgs);
	addCommand(commands, JSR_STR, JSR_CODE, JSR_ARGS, JSR_FUNCT, checkJsrArgs);

	addCommand(commands, RED_STR, RED_CODE, RED_ARGS, RED_FUNCT, checkRedArgs);
	addCommand(commands, PRN_STR, PRN_CODE, PRN_ARGS, PRN_FUNCT, checkPrnArgs);

	addCommand(commands, RTS_STR, RTS_CODE, RTS_ARGS, RTS_FUNCT, checkRtsArgs);
	addCommand(commands, STOP_STR, STOP_CODE, STOP_ARGS, STOP_FUNCT, checkStopArgs);
}

/* adding and creating new commands by setting the opcode the number of arguments and funct value and seting the correct argument type to the command */


void addCommand(NodesList* commands, char* commandName, int code, int numberOfArguments, int funct, bool(*correctArgumentsTypes)(NodesList*)) {
	Command* command = createCommand(code, numberOfArguments, funct, correctArgumentsTypes);
	addNode(commands, commandName, command);
	free(command); /* When adding a node, it copies the bytes, so we can free this command */
}

Command* createCommand(int code, int numberOfArguments, int funct, bool(*correctArgumentsTypes)(NodesList*)) {
	Command* command = (Command*)safeMalloc(sizeof(Command));
	command->code = code;
	command->numberOfArguments = numberOfArguments;
	command->funct = funct;
	command->correctArgumentsTypes = correctArgumentsTypes;
	return command;
}
