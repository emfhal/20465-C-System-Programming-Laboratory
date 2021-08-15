#include "General.h"

void* safeMalloc(size_t size) {
	void* toMalloc = malloc(size);
	if (!toMalloc) {
		printf("Coudn't allocate memory\n");
		exit(0);
	}
	return toMalloc;
}

/*Return value as int */
int returnAsInt(void* value)
{
	return *((int*)value);
}

char* getLabel(char** linePtr) {
	const char labelDelimiter = ':';
	return splitString(linePtr, labelDelimiter);
}

/*Removes the white spaces in a string */
char* trim(char *str)
{
	char *end;
	/* Trim leading space */
	while (*str != '\"' && isspace((unsigned char)*str)) str++;
	if (*str == 0)  /* All spaces */
		return str;
	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	/* Write new null terminator */
	*(end + 1) = 0;
	return str;
}

/*split string by delimiter char */
char* splitString(char** str, char delimiter) {
	char* innerString = *str;
	char *delimiterPtr = strchr(*str, delimiter);
	
	if (delimiterPtr) {
		int delimiterIndex = delimiterPtr - *str;
		/* delimiter end of the inner string */
		*(innerString + delimiterIndex) = 0; 
		*str += delimiterIndex + 1;
		return innerString;
	}
	return NULL;
}

bool startsWith(char* source, char* find) {
	return !strncmp(source, find, strlen(find));
}

bool isStringEmpty(char* str)
{
	return strlen(str) == 0;
}

/*Error holder lists section by adding and creating a message and line numbers */

ErrorHolder* createErrorHolder() {
	ErrorHolder* errorHolder = (ErrorHolder*)safeMalloc(sizeof(ErrorHolder));
	errorHolder->errors = createNodesList(sizeof(ErrorMessage));
	errorHolder->hasError = FALSE;
	return errorHolder;
}

void addError(ErrorHolder* errorHolder, char* error, int lineNumber) {
	ErrorMessage* errorMessage;
	errorMessage = createErrorMessage(error, lineNumber);
	errorHolder->hasError = TRUE;
	addNode(errorHolder->errors, NULL, errorMessage);
	free(errorMessage);
}

ErrorMessage* createErrorMessage(char* error, int lineNumber) {
	ErrorMessage* errorMessage = (ErrorMessage*)safeMalloc(sizeof(ErrorMessage));
	errorMessage->message = error;
	errorMessage->lineNumber = lineNumber;
	return errorMessage;
}

void freeErrorHolder(ErrorHolder* errorHolder) {
	freeNodesList(errorHolder->errors);
	free(errorHolder);
}


/*Output creator section - creating object, entiry and exsternal files only if there is (for internal debuging in General.h file change DEBUG value to 1) */

void createObjectFile(char* name, NodesList* words, int instructsCounter, int dataCounter)
{
	Node* wordNode;
	WordHolder* wordHolder;
	FILE* objectFile = createOutputFile(name, OBJECT_EXTENSION);

	fprintf(objectFile, "%7u %u\n", convertToDecimal(instructsCounter - MEMORY_ADDRESS_START), convertToDecimal(dataCounter));
	while ((wordNode = getNext(words)))
	{
		wordHolder = (WordHolder*)(wordNode->value);
		if (DEBUG == 1)
		{
			printf("%07u %06x\n", convertToDecimal(wordHolder->address), convertToDecimal(wordHolder->value));
			printBinaryMachineCode(convertToDecimal(wordHolder->value));
		}
		fprintf(objectFile, "%07u %06x\n", convertToDecimal(wordHolder->address), convertToDecimal(wordHolder->value));
	}
	fclose(objectFile);
}

void createEntriesFile(char* name, NodesList* entrySymbols)
{
	Node* symbolNode;
	if (entrySymbols->length > 0)
	{
		FILE* entriesFile = createOutputFile(name, ENTRIES_EXTENSION);
		while ((symbolNode = getNext(entrySymbols)))
		{
			fprintf(entriesFile, "%s %07u\n", symbolNode->name, convertToDecimal(returnAsInt(symbolNode->value)));
		}
		fclose(entriesFile);
	}
}

void createExtrenalsFile(char* name, NodesList* externSymbols)
{
	Node* symbolNode;
	NodesList* addressesList;
	Node* addressNode;

	if (externSymbols->length>0)
	{
		FILE* extrenalsFile = createOutputFile(name, EXTRENALS_EXTENSION);
		while ((symbolNode = getNext(externSymbols)))
		{
			addressesList = (NodesList*)(symbolNode->value);
			while ((addressNode = getNext(addressesList)))
			{
				fprintf(extrenalsFile, "%s %07u\n", symbolNode->name, convertToDecimal(returnAsInt(addressNode->value)));
			}
		}
		fclose(extrenalsFile);
	}
}

FILE* createOutputFile(char* name, char* fileExtension)
{
	FILE* file;
	char fileName[FILE_NAME_SIZE];
	sprintf(fileName, "%s.%s", name, fileExtension);
	file = fopen(fileName, "w");
	if (!file)
	{
		printf("Couldn't create a file\n");
		exit(0);
	}
	return file;
}

/* Converting int to decimal 10 recurtion method */
int convertToDecimal(int k)
{
	return (k == 0 || k == 1 ? k : ((k % 10) + 10 * convertToDecimal(k / 10)));
}

/* For internal debugging only */
void printBinaryMachineCode(int k)
{
int i;
printf("\n");
for (i = 23; i >= 0; i--)
{
	if (i == 2 || i == 7 || i == 10 || i == 12 || i == 15 || i == 17)
		printf("-");
	if ((k & (1 << i)) != 0)
	{
		printf("1");
	}
	else
	{
		printf("0");
	}
}
printf("\n");
}