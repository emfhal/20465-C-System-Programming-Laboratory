#include "AssemblyCompiler.h"

void compileAssembly(char* name, NodesList* lines) {
	InformationHolder* holder = createInformationHolder();
	firstRun(holder, lines);
	/* after our first run we checking the symbols in the arguments adding the Insturction Counter to the Data Counter and set enrties values */
	checkSymbolsUsedInArguments(holder);
	addICToDataCounter(holder->data->symbols, holder->instructions->counter);
	setEntriesValues(holder);

	/* next we will need to check if there was any error incase there was we will print them and if not we will run in the second time and create the outputs files */
	if (holder->errorHolder->hasError || holder->instructions->errorHolder->hasError || holder->data->errorHolder->hasError)
		printErrors(name, holder);
	else
		secondRun(name, holder, lines);
	
	freeInformationHolder(holder);
}

void checkSymbolsUsedInArguments(InformationHolder* holder) {
	Node* symbolNode;
	int* lineNum;
	while ((symbolNode = getNext(holder->instructions->symbolsUsedInArguments)))
	{
		/* seach symbols if they exsit*/
		Node* holderSymbol = searchNode(holder->data->symbols, symbolNode->name);
		Node* externSymbol = searchNode(holder->data->externSymbols, symbolNode->name);
		Node* insturctionsSymbol = searchNode(holder->instructions->symbols, symbolNode->name);
		
		/*if symbol exist continue if not return error*/
		if (holderSymbol || externSymbol || insturctionsSymbol)
			continue;
			
		lineNum = (int*)(symbolNode->value);
		addError(holder->errorHolder, "The label that was used in the arguments was invalid", *lineNum);
	}
}

void addICToDataCounter(NodesList* dataSymbols, int counterToAdd)
{
	Node* symbol;

	for (symbol = getNext(dataSymbols); symbol != NULL ; symbol = getNext(dataSymbols))
	{
		int* address = (int*)symbol->value;
		*address += counterToAdd;
	}
}

void setEntriesValues(InformationHolder* holder)
{
	Node* entryNode;
	Node* symbol;
	
	while ((entryNode = getNext(holder->data->enterySymbols)))
	{
		if ((symbol = searchNode(holder->instructions->symbols, entryNode->name)))
		{
			*((int*)(entryNode->value)) = *((int*)(symbol->value));
		}
		else if ((symbol = searchNode(holder->data->symbols, entryNode->name)))
		{
			*((int*)(entryNode->value)) = *((int*)(symbol->value));
		}
		else
		{
			addError(holder->errorHolder, "The entry wasn't defined", *((int*)(entryNode->value)));
		}
	}
}

void firstRun(InformationHolder* holder, NodesList* lines) {
		char* lastArgument = NULL;
	Node* node;
	AssemblyLine* assemblyLine;
	
	while ((node = getNext(lines))) {
		assemblyLine = (AssemblyLine*)(node->value);
		/* comment or space line doesn't need to handle at all */
		if (isStringEmpty(assemblyLine->line) || assemblyLine->line[0] == COMMENT_LINE_START)
			continue; 
		/* handle if is Guideline line start */
		if (assemblyLine->line[0] == GUIDELINE_LINE_START)
		{
			handleGuideline(holder->data, assemblyLine);
			lastArgument = NULL;
		}
		else{
			handleInstruction(holder->instructions, assemblyLine, &lastArgument);
		}	
	}
	if (NULL != lastArgument){
		free(lastArgument);}
}

void secondRun(char* name, InformationHolder* holder, NodesList* lines) {
	NodesList* words = translateInformation(holder->instructions, holder->data, holder->data->symbols, holder->data->externSymbols);
	createEntriesFile(name, holder->data->enterySymbols);
	createExtrenalsFile(name, holder->data->externSymbols);
	createObjectFile(name, words, holder->instructions->counter, holder->data->counter);
	freeNodesList(words);
}

void printErrors(char* name, InformationHolder* holder) {
	Node* errorNode;
	printf("Errors in the file %s.as:\n\n", name);
	while ((errorNode = getNext(holder->errorHolder->errors)))
		printError(errorNode);
	while ((errorNode = getNext(holder->data->errorHolder->errors)))
		printError(errorNode);
	while ((errorNode = getNext(holder->instructions->errorHolder->errors)))
		printError(errorNode);
	printf("\n\n");
}

void printError(Node* errorNode) {
	ErrorMessage* errorMessage = (ErrorMessage*)(errorNode->value);
	printf("Line number %d - %s\n", errorMessage->lineNumber, errorMessage->message);
}
