#include <windows.h>
#include <stdio.h>
#include "text.h"
#include "view.h"
#pragma warning( disable:4996)

/*    IN:
		buffer - the buffer in which all the text lies
	  OUT:
		countOfStrings - number of lines in the text
	Function counts the number of lines for the "without layout" mode
*/

int countOfStringsWithoutLayuot(char* buffer)
{
	int i = 0, countOfStrings = 1;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '\n')
			countOfStrings++;
		i++;
	}
	return countOfStrings;
}

/*    IN:
		myText - structure for text
	  OUT:
		myText - structure myText in which pointers are set to a buffer
	Function places pointers for the "without layout" mode
*/

structForText* initMyPointersWithoutLayout(structForText* myText)
{
	int i = 0, lineNumber = 0;
	myText->text = (char **)malloc(myText->countOfStrings * sizeof(char *));
	if (myText->text == NULL)
		return NULL;
	myText->text[lineNumber] = myText->buffer;
	lineNumber++;
	while (myText->buffer[i] != '\0')
	{
		if (myText->buffer[i] == '\n')
		{
			myText->text[lineNumber] = myText->buffer + i + 1;
			lineNumber++;
		}
		i++;
	}
	return myText;
}

/*    IN:
		myText - structure for text
	  OUT:
		maxSize - maximum line size in the text
	Function considers the maximum line size for the "without layout" mode
*/

int maxSizeOfStringsWithoutLayout(structForText* myText)
{
	int i = 0, maxSize = 0, lengthOfEachLine = 0;
	for (i = 0; i < myText->countOfStrings - 1; i++)
	{
		lengthOfEachLine = myText->text[i + 1] - myText->text[i] - 1;
		if (lengthOfEachLine > maxSize)
			maxSize = lengthOfEachLine;
	}
	lengthOfEachLine = strlen(myText->text[i]);
	if (lengthOfEachLine > maxSize)
		maxSize = lengthOfEachLine;
	return maxSize;
}

/*    IN:
		myText - structure for text
	  OUT:
		myText - structure myText in which pointers are set to a buffer
	Function places pointers for the "with layout" mode
*/

structForText *initPointersWithLayout(structForText *myText)
{
	int i = 0, lineNumber = 0, maxSize = 0;
	char *buffer = myText->buffer;
	myText->text = (char **)malloc(myText->countOfStrings * sizeof(char *));
	if (myText->text == NULL)
		return NULL;
	myText->text[lineNumber] = buffer;
	lineNumber++;
	while (buffer[i] != '\0')
	{
		if (maxSize == myText->maxSizeOfStrings)
		{
			myText->text[lineNumber] = buffer + i;
			lineNumber++;
			maxSize = 1;
		}
		else
			maxSize++;
		if (buffer[i] == '\n')
		{
			maxSize = 0;
			myText->text[lineNumber] = buffer + i + 1;
			lineNumber++;
		}
		i++;
	}
	return myText;
}

/*    IN:
		myText - structure for text
	  OUT:
		countOfStrings - number of lines in the text
	Function counts the number of lines for the "with layout" mode
*/

int countOfStringsWithLayuot(structForText *myText)
{
	int i = 0, countOfStrings = 1;
	int maxSize = 0;
	while (myText->buffer[i] != '\0')
	{
		if (maxSize == myText->maxSizeOfStrings)
		{
			countOfStrings++;
			maxSize = 1;
		}
		else
			maxSize++;
		if (myText->buffer[i] == '\n')
		{
			countOfStrings++;
			maxSize = 0;
		}
		i++;
	}
	return countOfStrings;
}

/*    IN:
		file - file from which you are reading data
	  OUT:
		buffer - the buffer in which all the text lies
	Function reads data from a file
*/

char *readOfFile(FILE *file, char *buffer)
{
	size_t sizeOfFile = 0, checkForFread = 0;

	fseek(file, 0, SEEK_END);
	sizeOfFile = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = (char *)calloc(sizeOfFile + 1, sizeof(char));
	if (buffer == NULL)
	{
		printf("Not enough memory");
		return NULL;
	}
	checkForFread = fread(buffer, sizeof(char), sizeOfFile, file);
	if (checkForFread != sizeOfFile && !feof(file))
	{
		printf("File is not read to the end");
		if (buffer != NULL)
                free(buffer);
		return NULL;
	}
	fclose(file);
	return buffer;
}

/*    IN:
		myText - structure for text
		file - file from which you are reading data
		nameOfFile - name of file from which you are reading data
		view - structure for view
		form - "with layout" mode or "without layout" mode
	  OUT:
		myText - initialized structure for text
	Function initializes the structure for storing text depending on the mode
*/

structForText * initializingTextStructure(structForText *myText, char *nameOfFile, structForView *view, int form)
{
	FILE *file = NULL;
	char* buffer = NULL;
	if (nameOfFile != NULL)
	{
		printf("%s\n", nameOfFile);
		file = fopen(nameOfFile, "rb");
		if (file == NULL)
		{
			printf("not open newFile!\n");
			return NULL;
		}
		myText->buffer = readOfFile(file, buffer);
	}
	if (myText->buffer == NULL)
	{
		myText->buffer = readOfFile(file, buffer);
	}
	if (form == 0)
	{
		myText->countOfStrings = countOfStringsWithoutLayuot(myText->buffer);
		myText = initMyPointersWithoutLayout(myText);
		myText->maxSizeOfStrings = maxSizeOfStringsWithoutLayout(myText);
		myText->form = 0;
	}
	if (form == 1)
	{
		int maxSizeOfSrcreen = view->numberOfCharactersOnScreen.x;
		myText->maxSizeOfStrings = maxSizeOfSrcreen;
		myText->countOfStrings = countOfStringsWithLayuot(myText);
		myText = initPointersWithLayout(myText);
		myText->form = 1;
	}
	if (file != NULL)
		fclose(file);
	return myText;
}

/*    IN:
		myText - structure for text
	  OUT:
		-
	Function frees the memory of the entire structure for text
*/

void clearText(structForText *myText)
{
	free(myText->buffer);
	myText->buffer = NULL;
	free(myText->text);
	free(myText);
}

/*    IN:
		hwnd - window handle
		myText - structure for text
		view - structure for view
	  OUT:
		myText - initialized structure for text
	Function opens a new file from the window and initializes structure myText
*/

structForText *openNewFile(HWND hwnd, structForText *myText, structForView *view)
{
	OPENFILENAME ofn;
	char fileName[200] = { 0 };
	int form = 0;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = 200;
	ofn.lpstrFilter = "All files\0*.*\0Text files\0*.txt\0";
	if (myText != NULL)
	{
		form = myText->form;
		clearText(myText);
	}
	else
		form = 0;
	if (GetOpenFileName(&ofn) == 0)
	{
		printf("new file not open\n");
		return NULL;
	}
	myText = (structForText *)malloc(sizeof(structForText));
	if (myText == NULL)
	{
		printf("Not enough memory");
		return NULL;
	}
	myText = initializingTextStructure(myText, ofn.lpstrFile, view, form);
	view->caret.x = 0;
	view->caret.y = 0;
	view->forCaret.x = 0;
	view->forCaret.y = 0;
	return myText;
}
