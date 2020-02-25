#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define _CRTDBG_MAP_ALLOC
#define MAXHIWORD 65535

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
//#include <crtdbg.h>
#include <limits.h>
//#pragma warning( disable:4996)
#include "text.h"
#include "view.h"
#include "menu.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");




int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	HWND hwnd;               /* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */



	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(hThisInstance, MAKEINTRESOURCE(230));
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	/* Создание окна */
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		_T("Code::Blocks Template Windows App"),       /* Title Text */ /* имя файла */
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		500,                 /* The programs width */
		375,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		//NULL                 /* No Window Creation data */
		lpszArgument
	);

	/* Make the window visible on the screen */
	/* Вывод окна на экран */
	ShowWindow(hwnd, nCmdShow);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		/* Препроцессорная обработка клавиатуры*/
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		/* Передаёт структуру "msg" обратно в Windows*/
		DispatchMessage(&messages);
	}

	/* The program return-value is 0 - The value that PostQuitMessage() gave */
//	_CrtDumpMemoryLeaks();
	return messages.wParam;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static structForView *view = NULL;
	static structForText *myText = NULL;
	CREATESTRUCT* pCS = NULL;
	RECT rect;
	GetWindowRect(hwnd, &rect);
	PAINTSTRUCT ps;
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
		CreateMyMenu(hwnd);
		pCS = (CREATESTRUCT*)lParam;
		myText = (structForText *)malloc(sizeof(structForText));
		if (myText == NULL)
		{
			printf("Not enough memory\n");
			SendMessage(hwnd, WM_DESTROY, 0, 0L);
			return 0;
		}
		myText = initializingTextStructure(myText, (char*)pCS->lpCreateParams, NULL, 0);
		if (myText == NULL)
			break;
		view = initializingViewStructure(hwnd, rect, view);
		if (view == NULL)
		{
			printf("Not enough memory\n");
			SendMessage(hwnd, WM_DESTROY, 0, 0L);
			return 0;
		}
		printf("Count of strings : %i\n", myText->countOfStrings);
		break;
	case WM_VSCROLL:
	    if (myText != NULL)
        {
            SetVerticalScroll(myText, view, LOWORD(wParam), &wParam, hwnd);
            SetScrollPos(hwnd, SB_VERT, view->outputLineNumber / (view->coefForvScroll + 1), TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
        }
		break;
	case WM_HSCROLL:
	    if (myText != NULL)
        {
            SetHorizontalScroll(myText, view, LOWORD(wParam), &wParam, hwnd);
            SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
        }
		break;
	case WM_KEYDOWN:
	    if (myText != NULL )
        {
            if ((view->numberOfCharactersOnScreen.y < view->caret.y || view->numberOfCharactersOnScreen.x < view->caret.x)) {
                view->caret.x = min(view->numberOfCharactersOnScreen.x, view->caret.x);
                view->outputCharacterNumber = view->forCaret.x - view->caret.x;
                view->caret.y = min(view->numberOfCharactersOnScreen.y, view->caret.y);
                view->outputLineNumber = view->forCaret.y - view->caret.y;
			}
            switch (wParam)
            {
            case VK_UP:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
                break;
            case VK_DOWN:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
                break;
            case VK_LEFT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0L);
                SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
                break;
            case VK_RIGHT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0L);
                SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
                break;
         /*   case VK_HOME:
                SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0L);
                SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
                break;
            case VK_END:
                SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0L);
                SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
                break; */
            case VK_PRIOR:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
                SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
                break;
            case VK_NEXT:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
                SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
                break;
            }
		InvalidateRect(hwnd, NULL, TRUE);
        }
		break;
	case WM_SIZE:
		view = initializingViewStructure(hwnd, rect, view);
		if (myText != NULL)
		{
			if (myText->form == 1)
			{
				view->pointerForCaret = myText->text[view->forCaret.y] + view->forCaret.x;
				free(myText->text);
				myText = initializingTextStructure(myText, NULL, view, myText->form);
				SetCaret(myText, view);
				SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
			}
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			view->coefForvScroll = myText->countOfStrings / MAXHIWORD;
			if (view->coefForvScroll > 0)
			{
				si.nMax = max(0, (myText->countOfStrings - view->numberOfCharactersOnScreen.y) / (1 + view->coefForvScroll));
			}
			else
			{
			    si.nMax = max(myText->countOfStrings - view->numberOfCharactersOnScreen.y, 0);
			}
			SetScrollRange(hwnd, SB_VERT, si.nMin, si.nMax, TRUE);
			si.nMax = myText->maxSizeOfStrings - 1;
			si.nPage = view->numberOfCharactersOnScreen.x;
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
			view->coefForvScroll = myText->countOfStrings / MAXHIWORD;

		}
		else
		{
			SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
			SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
		}
		SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
        SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);

		break;
	case WM_PAINT:
		if (myText != NULL)
		{
			hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rect);
			printTextOnScreen(hdc, myText, view);
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILEOPEN:
			myText = openNewFile(hwnd, myText, view);
			view->outputCharacterNumber = 0;
			view->outputLineNumber = 0;
			SendMessage(hwnd, WM_SIZE, 0, 0L);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case IDM_EXIT:
			SendMessage(hwnd, WM_DESTROY, 0, 0L);
			break;
		case IDM_WITHOUTLAYOUT:

			if (myText != NULL && myText->form == 1)
			{
				myText->form = 0;
				view->pointerForCaret = myText->text[view->forCaret.y] + view->forCaret.x;
				free(myText->text);
				myText = initializingTextStructure(myText, NULL, view, myText->form);
				if (view->caret.y == view->numberOfCharactersOnScreen.y) {
                    view->outputLineNumber++;
                    view->forCaret.y--;
                    view->caret.y--;
				}
				view->numberOfCharactersOnScreen.y--;
				SetCaret(myText, view);
                SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
				SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
				SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
				InvalidateRect(hwnd, NULL, TRUE);
				SendMessage(hwnd, WM_SIZE, 0, 0L);
			}
			break;

		case IDM_WITHLAYOUT:
			if ( myText!= NULL && myText->form == 0)
			{
			    SendMessage(hwnd, WM_KEYDOWN, VK_SPACE, 0L);
			    view->pointerForCaret = myText->text[view->forCaret.y] + view->forCaret.x;
				myText->form = 1;
				free(myText->text);
				myText = initializingTextStructure(myText, NULL, view, myText->form);
				view->outputCharacterNumber = 0;
                SetCaret(myText, view);
                SetCaretPos(view->caret.x * view->sizeOfChars.x, view->caret.y* view->sizeOfChars.y);
				SetScrollPos(hwnd, SB_VERT, view->outputLineNumber, TRUE);
				SetScrollPos(hwnd, SB_HORZ, view->outputCharacterNumber, TRUE);
				InvalidateRect(hwnd, NULL, TRUE);
				SendMessage(hwnd, WM_SIZE, 0, 0L);
			}
			break;
		}

		break;
    case WM_SETFOCUS:
        CreateCaret(hwnd, (HBITMAP)0, 2, 16);
        SetCaretPos(0, 0);
        ShowCaret(hwnd);
            break;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		if (myText != NULL) {
			clearText(myText);
		}
		if (view != NULL)
			free(view);
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
