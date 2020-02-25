#pragma once
#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define IDM_FILEOPEN 1
#define IDM_EXIT 2
#define IDM_WITHLAYOUT 3
#define IDM_WITHOUTLAYOUT 4

void CreateMyMenu(HWND hwnd);
void printTextOnScreen(HDC hdc, structForText *myText, structForView *view);
void SetVerticalScroll(structForText *myText, structForView *view, long long SB_ID, WPARAM *wParam, HWND hwnd);
void SetHorizontalScroll(structForText *myText, structForView *view, long long SB_ID, WPARAM *wParam, HWND hwnd);
void SetCaret(structForText *myText, structForView *view);
#endif
