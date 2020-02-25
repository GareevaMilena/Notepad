#pragma once
#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "view.h"

typedef struct structForText
{
	char** text;       //two-dimensional array with pointers to text
	int countOfStrings; //the number of lines in the text
	int maxSizeOfStrings; //size of the maximum line in the text
	char *buffer; //buffer - the buffer in which all the text lies
	int form;  //"with layout" mode or "without layout" mode
}structForText;

char *readOfFile(FILE *file, char *buffer);
int countOfStringsWithoutLayuot(char* buffer);
int countOfStringsWithLayuot(structForText *myText);
structForText *initMyPointersWithoutLayout(structForText* myText);
structForText *initPointersWithLayout(structForText *myText);
int maxSizeOfStringsWithoutLayout(structForText* myText);
structForText *initializingTextStructure(structForText *myText, char *nameOfFile, structForView *view, int form);
void clearText(structForText *myText);
structForText *openNewFile(HWND hwnd, structForText *myText, structForView *view);

#endif
