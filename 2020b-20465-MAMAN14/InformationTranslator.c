#include "InformationTranslator.h"
#include "General.h"

/* init InformationHolder by setting errorholder data and instruction */
InformationHolder* createInformationHolder() {
	InformationHolder* holder = (InformationHolder*)safeMalloc(sizeof(InformationHolder));
	holder->data = createGuidelineHolder();
	holder->instructions = createInstructions();
	holder->errorHolder = createErrorHolder();
	return holder;
}

void freeInformationHolder(InformationHolder* holder) {
	freeGuidelineHolder(holder->data);
	freeInstructions(holder->instructions);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}

/* translate infomation by insturction and data tranlates functions and return words list */
NodesList *translateInformation(InstructionsHolder *instructionsHolder, GuidelineHolder *guidelineHolder, NodesList *dataSymbols, NodesList *externSymbols)
{
	NodesList *words = createNodesList(sizeof(WordHolder));
	int wordAddress = MEMORY_ADDRESS_START;
	translateInstructions(&wordAddress, words, instructionsHolder, dataSymbols, externSymbols);
	translateData(&wordAddress, words, guidelineHolder, wordAddress);
	return words;
}

void translateInstructions(int *wordAddress, NodesList *words, InstructionsHolder *holder, NodesList *dataSymbols, NodesList *externSymbols)
{
	Node *instructionNode;
	Instruction *instruction;
	int instructionAddress;

	while ((instructionNode = (getNext(holder->instructions))))
	{
		instruction = (Instruction *)(instructionNode->value);
		instructionAddress = *wordAddress;
		addFirstWord(wordAddress, words, instruction, holder->registers);
		addArgumentsWords(instructionAddress, wordAddress, words, instruction->arguments, holder->symbols, dataSymbols, externSymbols, holder->registers);
	}
}

void translateData(int *wordAddress, NodesList *words, GuidelineHolder *holder, int counterToAdd)
{
	int i;
	DataWord *word;
	WordHolder *wordHolder = NULL;
	for (i = 0; i < holder->counter; i++)
	{
		word = (DataWord *)safeMalloc(sizeof(DataWord));
		word->value = holder->memory[i];
		wordHolder = createWordHolder(i + counterToAdd, (int *)word);
		addNode(words, NULL, wordHolder);
		if (DEBUG == 1)
		{
			printf("translateData: %u\t%x\n\n", wordHolder->address, wordHolder->value);
			printBinaryMachineCode(convertToDecimal(wordHolder->value));
		}
		free(wordHolder);
		free(word);
	}
}

void addFirstWord(int *wordAddress, NodesList *words, Instruction *instruction, NodesList *registers)
{
	FirstWord *firstWord;
	WordHolder *wordHolder;
	Node *registerNode;
	int destination = 0, source = 0, destinationRegister = 0, sourceRegister = 0;

	if (instruction->arguments->length >= 1)
	{
		destination = *((int *)(instruction->arguments->tail->value));
		registerNode = searchNode(registers, instruction->arguments->tail->name);
		if (registerNode != NULL) /* handle incase it is a destination register */
			destinationRegister = returnAsInt(registerNode->value);
	}

	if (instruction->arguments->length == 2)
	{
		source = *((int *)(instruction->arguments->head->value));
		registerNode = searchNode(registers, instruction->arguments->head->name);
		if (registerNode != NULL) /* handle incase it is a source register */ 
			sourceRegister = returnAsInt(registerNode->value);
	}
	firstWord = createFirstWord(destination, destinationRegister, source, sourceRegister, instruction->command->code, instruction->command->funct);
	wordHolder = createWordHolder(*wordAddress, (int *)firstWord);
	(*wordAddress)++;
	addNode(words, NULL, wordHolder);
	if (DEBUG == 1)
	{
		printf("addFirstWord: %u\t%x\n\n", wordHolder->address, wordHolder->value);
		printBinaryMachineCode(convertToDecimal(wordHolder->value));
	}
	free(wordHolder);
	free(firstWord);
}

/* add argumets to words list by creating word holder*/
void addArgumentsWords(int instructionAddress, int *wordAddress, NodesList *words, NodesList *arguments, NodesList *instructionSymbols, NodesList *dataSymbols, NodesList *externSymbols, NodesList *registers)
{
	Node *argumentNode;
	ArgumentWord *word;
	WordHolder *wordHolder;

	while ((argumentNode = getNext(arguments)))
	{
		if (searchNode(registers, argumentNode->name) == NULL)
		{
			word = argumentToWord(instructionAddress, *wordAddress, argumentNode, instructionSymbols, dataSymbols, externSymbols, registers);
			wordHolder = createWordHolder(*wordAddress, (int *)word);

			(*wordAddress)++;
			addNode(words, NULL, wordHolder);
			if (DEBUG == 1)
			{
				printf("addArgumentsWords: %u\t%x\n\n", wordHolder->address, wordHolder->value);
				printBinaryMachineCode(convertToDecimal(wordHolder->value));
			}
			free(wordHolder);
			free(word);
		}
	}
}
/* creating the word first incase hadle with funct that not exsit of equal to zero by setting than to zero*/
FirstWord *createFirstWord(int destination, int destinationRegister, int source, int sourceRegister, int opcode, int funct)
{
	FirstWord *word = (FirstWord *)safeMalloc(sizeof(FirstWord));
	word->encodingType = ABSOLUTE_ENCODING;
	word->funct = funct > 0 ? funct : 0;
	word->destination = destination;
	word->destinationRegister = destinationRegister;
	word->source = source;
	word->sourceRegister = sourceRegister;
	word->opcode = opcode;
	return word;
}

/* returning the argument word by checking the addresting type value (0-3) and  setting word vlaue and encoding type (A,R,E value)*/
ArgumentWord *argumentToWord(int instructionAddress, int wordAddress, Node *argumentNode, NodesList *instructionSymbols, NodesList *dataSymbols, NodesList *externSymbols, NodesList *registers)
{
	ArgumentWord *word;
	Node *symbol;
	int addressingType;
	char *parser;

	word = NULL;
	addressingType = returnAsInt(argumentNode->value); /* Convert the addressing type to int value */

	if (addressingType == IMMEDIATE_ADDRESSING_VALUE)
	{
		word = (ArgumentWord *)safeMalloc(sizeof(ArgumentWord));
		parser = argumentNode->name + 1; /* Skip the first char # that represents that it's an IMMEDIATE_ADDRESSING */
		word->value = atoi(parser);
		word->encodingType = ABSOLUTE_ENCODING;
	}

	else if (addressingType == DIRECT_ADDRESSING_VALUE)
	{
		word = (ArgumentWord *)safeMalloc(sizeof(ArgumentWord));
		if ((symbol = searchNode(instructionSymbols, argumentNode->name)))
		{
			word->value = returnAsInt(symbol->value);
			word->encodingType = ABSOLUTE_ENCODING;
		}
		else if ((symbol = searchNode(dataSymbols, argumentNode->name)))
		{
			word->value = returnAsInt(symbol->value);
			word->encodingType = RELOCATABLE_ENCODING;
		}
		else if ((symbol = searchNode(externSymbols, argumentNode->name)))
		{
			word->value = 0;
			word->encodingType = EXTERNAL_ENCODING;
			addNode((NodesList *)(symbol->value), NULL, &wordAddress);
		}
	}

	else if (addressingType == RELATIVE_ADDRESSING_VALUE)
	{
		word = (ArgumentWord *)safeMalloc(sizeof(ArgumentWord));
		parser = argumentNode->name + 1; /* Skip the first char & that represents that it's an RELATIVE_ADDRESSING */
		word->value = distanceBetweensAddress(parser, wordAddress, instructionSymbols, dataSymbols, externSymbols);
		word->encodingType = ABSOLUTE_ENCODING;
	}

	else if (addressingType == DIRECT_REGISTER_ADDRESSING_VALUE)
	{
		word = (ArgumentWord *)safeMalloc(sizeof(ArgumentWord));
		word->value = atoi(argumentNode->name);
		word->encodingType = ABSOLUTE_ENCODING;
	}

	if (word)
		return word;
	return NULL;
}

/* return the distance between the distance of a spesficic label to the current address (+1) */
int distanceBetweensAddress(char *label, int wordAddress, NodesList *instructionSymbols, NodesList *dataSymbols, NodesList *externSymbols)
{
	Node *symbol;
	int distance = -1;
	if ((symbol = searchNode(instructionSymbols, label)))
	{
		distance = returnAsInt(symbol->value);
	}
	else if ((symbol = searchNode(dataSymbols, label)))
	{
		distance = returnAsInt(symbol->value);
	}
	else if ((symbol = searchNode(externSymbols, label)))
	{
		distance = 0;
	}
	return (distance - wordAddress + 1);
}

WordHolder *createWordHolder(int address, int *wordValue)
{
	WordHolder *wordHolder = (WordHolder *)safeMalloc(sizeof(WordHolder));
	wordHolder->address = address;
	wordHolder->value = (*wordValue & BITS_TO_ZERO_START); /*0xFFFFFF*/
	return wordHolder;
}
