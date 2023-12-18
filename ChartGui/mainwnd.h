#ifndef _MAIN_WND_H
#define _MAIN_WND_H

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#define WND_TITLE "Chart creator"

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ref(void);
int getText(HWND, char*);

#endif

