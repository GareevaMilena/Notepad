#include <windows.h>
#include <stdio.h>

#include "text.h"
#include "view.h"
#include "menu.h"

/*    IN:
		myText - structure for text
		index - position in which to calculate the length
	  OUT:
        lineLen - line length
    Function computes length of the line placed on idx's position
    This is necessary when switching modes
*/

int lineLength(structForText *myText, int index)
{
    int lineLen = 0;
    if (index >= myText->countOfStrings - 1) {
        lineLen = strlen(myText->text[index]);
    } else {
        lineLen = myText->text[index+1] - myText->text[index];
        if (myText->text[index][lineLen-1] == '\n') {
            lineLen = max(lineLen - 2, 0);
        }
    }
    return lineLen;
}

/*    IN:
		hwnd - window handle
		view - structure for view
	  OUT:
		-
    Function returns window to initial position that was before scrolling horizontally
*/

void returnToThePreviousPosition(HWND hwnd, structForView *view)
{
    view->outputCharacterNumber = view->forCaret.x - view->caret.x;
//    view->caret.y = min(view->caret.y, view->numberOfCharactersOnScreen.y);
    view->outputLineNumber = view->forCaret.y - view->caret.y;
    SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
    SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
}

/*    IN:
		hwnd - window handle
	  OUT:
		-
	Function creates a menu (dynamic mode)
*/

void CreateMyMenu(HWND hwnd)
{
	HMENU hMenu = CreateMenu();
	HMENU hSubMenu = CreatePopupMenu();

	AppendMenu(hSubMenu, MF_STRING, IDM_FILEOPEN, "&Open");
	AppendMenu(hSubMenu, MF_STRING, IDM_EXIT, "&Exit");

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
	hSubMenu = CreatePopupMenu();

	AppendMenu(hSubMenu, MF_STRING, IDM_WITHLAYOUT, "&With Layout");
	AppendMenu(hSubMenu, MF_STRING, IDM_WITHOUTLAYOUT, "&Without Layout");

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&View");

	SetMenu(hwnd, hMenu);

}

/*    IN:
		hdc - device context instance
		myText - structure for text
		view - structure for view
	  OUT:
		-
	Function displays text
*/

void printTextOnScreen(HDC hdc, structForText *myText, structForView *view)
{
	int i = 0;
	int lineNumber = 0;
	int numberOfWindowLines = view->numberOfCharactersOnScreen.y + (myText->form == 1 && view->caret.y == view->numberOfCharactersOnScreen.y);
	for (i = view->outputLineNumber; i < myText->countOfStrings - 1; i++)
	{
	    int lineLen = lineLength(myText, i);
		if (lineLen - view->outputCharacterNumber >= view->numberOfCharactersOnScreen.x)
			TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, view->numberOfCharactersOnScreen.x);
		else {
            TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, lineLen - view->outputCharacterNumber);
		}
		lineNumber++;
		numberOfWindowLines--;
		if (numberOfWindowLines == 1)
		{
			i++;
			break;
		}
	}
	if (i != myText->countOfStrings - 1)
	{
		if (myText->text[i + 1] - myText->text[i] - view->outputCharacterNumber > view->numberOfCharactersOnScreen.x)
			TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, view->numberOfCharactersOnScreen.x);
		else
		{
			TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, myText->text[i + 1] - myText->text[i] - view->outputCharacterNumber);
		}
	}
	else
	{
		if (((int)strlen(myText->text[i]) - view->outputCharacterNumber) > view->numberOfCharactersOnScreen.x)
		{
			TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, view->numberOfCharactersOnScreen.x);
		}
		else
		{
			TextOut(hdc, 0, lineNumber * view->sizeOfChars.y, myText->text[i] + view->outputCharacterNumber, strlen(myText->text[i]) - view->outputCharacterNumber);
		}
	}
	return;
}

/*    IN:
		myText - structure for text
		view - structure for view
		SB_ID - command id
		wParam - parameter
		hwnd - window handle
	  OUT:
		-
	Function handles vertical scroll bar commands
*/

void SetVerticalScroll  (structForText *myText, structForView *view, long long SB_ID, WPARAM *wParam, HWND hwnd)
{
    static int prevThumbPosition = 0;
	switch (SB_ID)
	{
/*	case SB_TOP:
		view->caret.x = 0;
		view->forCaret.x = 0;
		view->outputCharacterNumber = 0;
		SetCaretPos( view->caret.x * view->sizeOfChars.x, view->caret.y * view->sizeOfChars.y);
		break;
	case SB_BOTTOM:
	    {
	        int lineLen = lineLength(myText, myText->countOfStrings-1);
            if (lineLen < view->numberOfCharactersOnScreen.x)
            {
                view->forCaret.x = lineLen;
                view->caret.x = view->forCaret.x;
            }
            else
            {
                view->outputCharacterNumber = lineLen - view->numberOfCharactersOnScreen.x;
                view->caret.x = view->numberOfCharactersOnScreen.x;
                view->forCaret.x = lineLen;
            }
	    }
        SetCaretPos( view->caret.x * view->sizeOfChars.x, view->caret.y * view->sizeOfChars.y);
		break;*/
	case SB_LINEUP:
        returnToThePreviousPosition(hwnd, view);
	    if (view->forCaret.y < view->outputLineNumber || view->forCaret.y > view->outputLineNumber + view->numberOfCharactersOnScreen.y)
        {
            if (view->outputLineNumber != 0)
                view->outputLineNumber--;
            view->caret.x = 0;
            view->caret.y = 0;
            view->forCaret.x = 0;
            view->forCaret.y = view->outputLineNumber;
            view->outputCharacterNumber = 0;
            SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
            break;
        }
        if (view->forCaret.y > 0)
        {
            view->forCaret.y--;
            view->caret.y--;
            if (view->outputLineNumber > 0 && view->caret.y < 0)
            {
                    view->outputLineNumber--;
                    view->caret.y = 0;
            }
            int lineLen = lineLength(myText, view->forCaret.y);
            if ( lineLen < view->forCaret.x)
            {
                if (view->outputCharacterNumber>0 && lineLen < view->forCaret.x - view->caret.x)
                {
                    view->outputCharacterNumber = max(0, lineLen - view->numberOfCharactersOnScreen.x);
                }
                view->forCaret.x = lineLen;
                view->caret.x = view->forCaret.x - view->outputCharacterNumber;
            }
        }
        SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
        if (myText->form == 0) {
            SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
        }
        break;
	case SB_LINEDOWN:
	    returnToThePreviousPosition(hwnd, view);
	    if (view->forCaret.y < view->outputLineNumber || view->forCaret.y > view->outputLineNumber + view->numberOfCharactersOnScreen.y)
        {
            view->outputLineNumber++;
            view->caret.x = 0;
            view->forCaret.x = 0;
            view->caret.y = view->numberOfCharactersOnScreen.y - 1;
            view->forCaret.y = view->outputLineNumber + view->numberOfCharactersOnScreen.y - 1;
            SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
            break;
        }
	    if (view->forCaret.y < myText->countOfStrings - 1)
        {
            view->forCaret.y++;
             if (view->forCaret.y + 1 > view->numberOfCharactersOnScreen.y && view->caret.y + 1 >= view->numberOfCharactersOnScreen.y)
             {
                view->outputLineNumber++;
             }
             if (view->caret.y < view->numberOfCharactersOnScreen.y - 1)
            {
                view->caret.y++;
            }
            int lineLen = lineLength(myText, view->forCaret.y);
            if (((view->forCaret.y + 1 != myText->countOfStrings)&&
                 lineLen < view->forCaret.x) ||
                ((view->forCaret.y == myText->countOfStrings - 1) &&
                 (lineLen < view->forCaret.x) ))
            {
                if (view->outputCharacterNumber>0 && lineLen < view->forCaret.x - view->caret.x)
                {
                    view->outputCharacterNumber = max(0, lineLen - view->numberOfCharactersOnScreen.x);
                }
                view->forCaret.x = lineLen;
                view->caret.x = view->forCaret.x - view->outputCharacterNumber;
            }
        }
        SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
        if (myText->form == 0) {
            SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
        }
		break;
	case SB_THUMBTRACK:
	    {
	        //printf("thum %d\n", (HIWORD(*wParam)));
	        //view->forCaret.x = view->caret.x = 0;
	    //    int prev = view->outputLineNumber;
	        int temp = (HIWORD(*wParam)) * (view->coefForvScroll + 1);
	        int mn, mx;
            GetScrollRange(hwnd, SB_VERT, &mn, &mx);
            if (mx == HIWORD(*wParam)) {
                temp = myText->countOfStrings - view->numberOfCharactersOnScreen.y;
            } else if (mn == HIWORD(*wParam)) {
                temp = 0;
            }
            view->outputLineNumber = temp;
         //   view->forCaret.y += view->outputLineNumber - prev;
            SetCaretPos(-1 * view->sizeOfChars.x, -1 * view->sizeOfChars.y);
	    }
		break;
	}
	return;
}

/*    IN:
		myText - structure for text
		view - structure for view
		SB_ID - command id
		wParam - parameter
		hwnd - window handle
	  OUT:
		-
	Function handles horizontal scroll bar commands
*/

void SetHorizontalScroll(structForText *myText, structForView *view, long long SB_ID, WPARAM *wParam, HWND hwnd)
{
    int i = 0, o = 0, flag = 0;
	switch (SB_ID)
	{
	case SB_THUMBTRACK:
//	    printf("hwtf %hu", HIWORD(*wParam));
		view->outputCharacterNumber = HIWORD(*wParam);
		if (view->outputCharacterNumber < myText->maxSizeOfStrings - 1)
        {
            SetCaretPos(-1 * view->sizeOfChars.x, -1* view->sizeOfChars.y);
        }
		break;
	case SB_LINELEFT:
        returnToThePreviousPosition(hwnd, view);
	    if (view->forCaret.x >= 0)
        {
            if (view->caret.x == 0 && view->forCaret.y > 0 && view->outputCharacterNumber == 0)
            {
                int lineLen = lineLength(myText, view->forCaret.y-1);
                if ( lineLen < view->numberOfCharactersOnScreen.x) {
                    view->forCaret.x = view->caret.x = lineLen;
                } else {
                    view->outputCharacterNumber = lineLen - view->numberOfCharactersOnScreen.x;
                    view->caret.x = view->numberOfCharactersOnScreen.x;
                    view->forCaret.x = lineLen;
                }
                if (view->forCaret.y == view->outputLineNumber)
                    view->outputLineNumber--;
                    if (view->caret.y> 0)
                        view->caret.y--;
                view->forCaret.y--;
            }
            else if (view->forCaret.x > 0)
            {
                view->forCaret.x--;
                view->caret.x--;
                if (view->outputCharacterNumber > 0 && view->caret.x < 0)
                {
                    view->outputCharacterNumber--;
                    view->caret.x = 0;
                }
            }
        }
        SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
		break;
    case SB_LINERIGHT:
        {
            returnToThePreviousPosition(hwnd, view);
            int lineLen = lineLength(myText, view->forCaret.y);
            if ((view->forCaret.y + 1 < myText->countOfStrings) &&
                (lineLen < view->numberOfCharactersOnScreen.x))
            {
                if (view->forCaret.x >= lineLen)
                {
                    if (view->caret.y + 1 >= view->numberOfCharactersOnScreen.y)
                        view->outputLineNumber++;
                    view->caret.x = 0;
                    if (view->caret.y + 1 < view->numberOfCharactersOnScreen.y)
                        view->caret.y++;
                    view->forCaret.x = 0;
                    view->forCaret.y++;
                    if (view->outputCharacterNumber>0)
                        view->outputCharacterNumber = 0;
                    SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
                    view->caret.x = min(view->numberOfCharactersOnScreen.x, view->caret.x);
                    view->outputCharacterNumber = view->forCaret.x - view->caret.x;
                    break;
                }
            }
            else if (view->forCaret.y + 1 < myText->countOfStrings &&
                     lineLen  == view->forCaret.x)
            {
                if (view->caret.y + 1 >= view->numberOfCharactersOnScreen.y)
                    view->outputLineNumber++;
                view->forCaret.x = 0;
                view->outputCharacterNumber = 0;
                view->caret.x = 0;
                if (view->caret.y + 1 < view->numberOfCharactersOnScreen.y)
                    view->caret.y++;
                view->forCaret.y++;
                SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
                view->caret.x = min(view->numberOfCharactersOnScreen.x, view->caret.x);
                view->outputCharacterNumber = view->forCaret.x - view->caret.x;
                break;
            }
            if((view->forCaret.y == myText->countOfStrings - 1) &&
                (view->forCaret.x == lineLen)) {
                break;
            }
            view->forCaret.x++;
            if (view->caret.x < view->numberOfCharactersOnScreen.x)
            {
                view->caret.x++;
            }
            else if (view->outputCharacterNumber + view->numberOfCharactersOnScreen.x < myText->maxSizeOfStrings &&
                view->caret.x == view->numberOfCharactersOnScreen.x)
            {
                view->outputCharacterNumber++;
            }
            SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
		break;
	}
	}
	return;
}
/*    IN:
		myText - structure for text
		view - structure for view
	  OUT:
		-
	Function sets the correct carriage position when switching from mode to mode
*/
void SetCaret(structForText *myText, structForView *view)
{
    int i = 0, flag = 0;
    int prevCaretY = view->caret.y, prevForCaretY = view->forCaret.y;
    for (i = 0; i < myText->countOfStrings - 1; i++)
    {
        if (myText->text[i+1] - view->pointerForCaret > 0)
        {
            view->forCaret.x = view->pointerForCaret - myText->text[i];
            view->forCaret.y = i;
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        view->forCaret.x = view->pointerForCaret - myText->text[i];
        view->forCaret.y = i;
    }
    if (view->forCaret.x <= view->numberOfCharactersOnScreen.x)
    {
        view->caret.x = view->forCaret.x;
        view->outputCharacterNumber = 0;
    }else{
        view->caret.x = view->forCaret.x % view->numberOfCharactersOnScreen.x;
        view->outputCharacterNumber = (view->forCaret.x / view->numberOfCharactersOnScreen.x) * view->numberOfCharactersOnScreen.x;
    }
    view->caret.y = min(prevCaretY, view->forCaret.y);
    view->outputLineNumber = view->forCaret.y - view->caret.y;
    return;
}
