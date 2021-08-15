#include "Assembler.h"

int main(int argc, char* argv[])
{	
	char* fileName;
	int i;
	char assemblyFileName[FILE_NAME_SIZE];
	NodesList* lines = NULL;
	for (i = 1; i < argc; i++) { /* The command can include multilpte .as files*/
		fileName = argv[i];
		sprintf(assemblyFileName, "%s.as", fileName); /* The input extestion file must to be a as file*/
		lines = linesToAssemblyList(assemblyFileName); 
		compileAssembly(fileName, lines);
		freeLinesString(lines);
		freeNodesList(lines); /* Free nodes list after complie the aseembly lines*/
	}	return 0;
}

NodesList* linesToAssemblyList(char* fileName) {
	char currentLine[MAX_LINE_LEN];
	AssemblyLine* assemblyLine;
	int lineNumber = 0;
	NodesList* linesList = createNodesList(sizeof(AssemblyLine));
	FILE* file = fopen(fileName, "r");

	if (NULL == file) /* If the input file could not open*/
	{
		printf("Error - Couldnt open the file %s\n", fileName);
		exit(0);
	}

	while (fgets(currentLine, MAX_LINE_LEN, file)){
		assemblyLine = createAssemblyLine(currentLine, ++lineNumber); /* Add 1 to line number and create assembly line*/
		assemblyLine->line = trim(assemblyLine->line);
		assemblyLine->labelName = getLabel(&(assemblyLine->line));
	
		/* After getting the label name, we need to trim in the last time before adding the line to the list */
		assemblyLine->line = trim(assemblyLine->line);
		addNode(linesList, NULL, assemblyLine);
	
		/* After adding to list list we can free this line from the memory */
		free(assemblyLine); 
	}
	fclose(file);
	return linesList;
}

void freeLinesString(NodesList* lines) {
	Node* node;
	while ((node = getNext(lines))) {
		free(((AssemblyLine*)(node->value))->originalLinePtr);
	}
}
