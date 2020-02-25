#include <windows.h>
#include <stdio.h>
#include<math.h>

#include "text.h"
#include "view.h"

/*    IN:
        hwnd - window handle
        view - structure for view
      OUT:
        view - initialized structure for view
    Function sets a monospace font
*/

structForView *monospaceFont(HWND hwnd, structForView *view)
{
	TEXTMETRIC tm;
	HDC hdc;

	hdc = GetDC(hwnd);

	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

	GetTextMetrics(hdc, &tm);
	view->sizeOfChars.x = tm.tmMaxCharWidth;
	view->sizeOfChars.y = tm.tmHeight+ tm.tmExternalLeading;

	ReleaseDC(hwnd, hdc);

	return view;
}

/*    IN:
        rect - structure that defines a rectangle
        view - structure for view
      OUT:
        view - initialized structure for view
    Function sets window sizes
*/

structForView *screenSize( RECT rect, structForView *view)
{
    view->screenSize.x = rect.right - rect.left - 2 * GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXVSCROLL) - 2 * GetSystemMetrics(SM_CXBORDER);
    view->screenSize.y = rect.bottom - rect.top - 2 * GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYHSCROLL) - 2 * GetSystemMetrics(SM_CYBORDER) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYCAPTION);
    view->numberOfCharactersOnScreen.x = floor((float)view->screenSize.x / (float)view->sizeOfChars.x);
    view->numberOfCharactersOnScreen.y = ceil((float)view->screenSize.y/ (float)view->sizeOfChars.y );
    return view;
}

/*    IN:
        hwnd - window handle
        rect - structure that defines a rectangle
        oldView - old structure for view
      OUT:
        view - new initialized structure for view
    Function initializes the window structure
*/

structForView *initializingViewStructure(HWND hwnd, RECT rect, structForView *view)
{
	if (view == NULL)
	{
		view = (structForView *)malloc(sizeof(structForView));
		view->outputCharacterNumber = 0;
		view->outputLineNumber = 0;
		view->caret.x = 0;
        view->caret.y = 0;
        view->forCaret.x = 0;
        view->forCaret.y = 0;
	}
    view = monospaceFont(hwnd, view);
    view = screenSize(rect, view);
    return view;
}
