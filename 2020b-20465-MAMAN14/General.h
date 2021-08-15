#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "NodesList.h"

/* Setting DEBUG value to 1 for internal debugging */ 
#define DEBUG 0

/* Setting exstion name of the output files */ 
#define OBJECT_EXTENSION "ob"
#define ENTRIES_EXTENSION "ent"
#define EXTRENALS_EXTENSION "ext"
/* Setting memory adress start max line lenth, max len lebel and file name size */ 
#define MEMORY_ADDRESS_START 100
#define MAX_LINE_LEN 80
#define MAX_LEN_LEBAL 32
#define FILE_NAME_SIZE 50

/* Setting size of each word total 24 bits */ 
#define ENCODING_BITS_SIZE 3
#define FUNCT_BITS_SIZE 5
#define DESTINATION_REGISTER_BITS_SIZE 3
#define DESTINATION_BITS_SIZE 2
#define SOURCE_REGISTER_BITS_SIZE 3
#define SOURCE_BITS_SIZE 2
#define OPCODE_BITS_SIZE 6

/* A,R,E encoding values (A: 100=4,R: 010=2,E: 001=1)*/ 
#define ABSOLUTE_ENCODING 4
#define RELOCATABLE_ENCODING 2
#define EXTERNAL_ENCODING 1

/* for 24 bytes (2^24)-1 = FFFFFF*/
#define BITS_TO_ZERO_START 0xFFFFFF

/* ALL_BITS_SIZE = 24 */ 
#define ALL_BITS_SIZE (ENCODING_BITS_SIZE+FUNCT_BITS_SIZE+DESTINATION_REGISTER_BITS_SIZE+DESTINATION_BITS_SIZE+SOURCE_REGISTER_BITS_SIZE+SOURCE_BITS_SIZE+OPCODE_BITS_SIZE)

typedef enum { FALSE, TRUE } bool;

typedef struct{
	unsigned int encodingType : ENCODING_BITS_SIZE;
	unsigned int funct : FUNCT_BITS_SIZE;
	unsigned int destinationRegister : DESTINATION_REGISTER_BITS_SIZE;
	unsigned int destination : DESTINATION_BITS_SIZE;
	unsigned int sourceRegister : SOURCE_REGISTER_BITS_SIZE;
	unsigned int source : SOURCE_BITS_SIZE;
	unsigned int opcode : OPCODE_BITS_SIZE;
} FirstWord;

typedef struct {
	unsigned int encodingType : ENCODING_BITS_SIZE;
	int value : (ALL_BITS_SIZE - ENCODING_BITS_SIZE);
} ArgumentWord;

typedef struct {
	int value : ALL_BITS_SIZE;
} DataWord;

typedef struct {
	int address;
	int value;
} WordHolder;

typedef struct
{
	char* message;
	int lineNumber;
} ErrorMessage;

typedef struct
{
	NodesList* errors;
	bool hasError;
} ErrorHolder;

ErrorHolder* createErrorHolder();
void addError(ErrorHolder* errorHolder, char* error, int lineNumber);
ErrorMessage* createErrorMessage(char* error, int lineNumber);
void freeErrorHolder(ErrorHolder* errorHolder);

void* safeMalloc(size_t size);
int returnAsInt(void* value);
char* getLabel(char** linePtr);

/* Removes the white spaces in a string */
char* trim(char *str);

/* Seperates the string with first appearance of the delimiter */
char* splitString(char** str, char delimiter);

bool startsWith(char* source, char* find);

bool isStringEmpty(char* str);

void createObjectFile(char* name, NodesList* words, int instructsCounter, int dataCounter);

void createEntriesFile(char* name, NodesList* entrySymbols);

void createExtrenalsFile(char* name, NodesList* externSymbols);

FILE* createOutputFile(char* name, char* fileExtension);

int convertToDecimal(int k);

void printBinaryMachineCode(int k);

#endif
