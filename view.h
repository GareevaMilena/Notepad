#pragma once
#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

typedef struct{
	int x;
	int y;
}positionOnScreen;

typedef struct structForView{
    positionOnScreen screenSize;  //screen sizes
	positionOnScreen sizeOfChars; //letter size
    positionOnScreen numberOfCharactersOnScreen; //number of characters that can fit on the screen
	positionOnScreen caret;  //cursor position relative to screen
	positionOnScreen forCaret; //cursor position relative to text
	int outputCharacterNumber; //character number from which to print
	int outputLineNumber; //line number with which to print
	float coefForvScroll; // display coefficient for vertical scroll
	char *forLayuot; //pointer to the first word when changing mode
	char *pointerForCaret; //pointer to remember the cursor position when switching modes
} structForView;

structForView *initializingViewStructure(HWND hwnd, RECT rect, structForView *view);
structForView *monospaceFont(HWND hwnd, structForView *view);
structForView *screenSize( RECT rect, structForView *view);

#endif
#pragma once
