#include <windows.h>
#include <stdlib.h>
#include "mainwnd.h"

HWND hMainWnd, hInputFilename, hOutputFilename, hWidth, hHeight, hChartBackground, hChartColor, hChartType, hBarCount, hBarSpace, hBarWidth, hNetType, hNetColor, hNetCount, hNetSpace, hNetWidth, hLineType, hGenerate, hShowAtEnd;

HINSTANCE hAppInstance;

void addColors(HWND);

BOOL InitInstance(HINSTANCE hInst) {
	hAppInstance = hInst;

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wc.lpszClassName = "MainWndClass";
	wc.lpszMenuName = NULL;

	if (!RegisterClassEx(&wc)) {
		DWORD err = GetLastError();
		return FALSE;
	}
	
	hMainWnd = CreateWindowEx(0, "MainWndClass", "Chart creator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 600, NULL, NULL, hInst, NULL);
	
	if (!hMainWnd) {
		return FALSE;
	}
	
	hInputFilename = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 25, 220, 30, hMainWnd, NULL, hAppInstance, NULL);
	hOutputFilename = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 65, 220, 30, hMainWnd, NULL, hAppInstance, NULL);
	
	hWidth = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 115, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	hHeight = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 155, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	
	hChartType = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 90, 205, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	hChartBackground = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 90, 245, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	hChartColor = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 90, 285, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	
	addColors(hChartBackground);
	addColors(hChartColor);
	
	SendMessage(hChartType, CB_ADDSTRING, 0, (LPARAM) "Flow");
	SendMessage(hChartType, CB_ADDSTRING, 0, (LPARAM) "Dot");
	SendMessage(hChartType, CB_ADDSTRING, 0, (LPARAM) "Bar");
	
	hBarCount = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 205, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	hBarSpace = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 245, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	hBarWidth = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 285, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	
	EnableWindow(hBarCount, FALSE);
	EnableWindow(hBarSpace, FALSE);
	EnableWindow(hBarWidth, FALSE);
	
	hNetType = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 90, 330, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	
	SendMessage(hNetType, CB_ADDSTRING, 0, (LPARAM) "None");
	SendMessage(hNetType, CB_ADDSTRING, 0, (LPARAM) "Net");
	SendMessage(hNetType, CB_ADDSTRING, 0, (LPARAM) "Lines");
	
	hNetCount = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 370, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	hNetSpace = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 410, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	hNetWidth = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 450, 70, 30, hMainWnd, NULL, hAppInstance, NULL);
	
	hNetColor = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 90, 490, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	
	addColors(hNetColor);
	
	hLineType = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 330, 330, 150, 200, hMainWnd, NULL, hAppInstance, NULL);
	
	SendMessage(hLineType, CB_ADDSTRING, 0, (LPARAM) "Horizontal");
	SendMessage(hLineType, CB_ADDSTRING, 0, (LPARAM) "Vertical");
	
	hGenerate = CreateWindowEx(0, "BUTTON", "GENERATE", WS_CHILD | WS_VISIBLE, 255, 490, 200, 40, hMainWnd, NULL, hAppInstance, NULL);
	
	EnableWindow(hNetColor, FALSE);
	EnableWindow(hNetSpace, FALSE);
	EnableWindow(hNetCount, FALSE);
	EnableWindow(hNetWidth, FALSE);
	EnableWindow(hLineType, FALSE);
	
	hShowAtEnd = CreateWindowEx(0, "BUTTON", "Show after creating", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 495, 490, 150, 40, hMainWnd, NULL, hAppInstance, NULL);
	
	
	
	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd) {
	MSG Msg;

	if (!InitInstance(hInstance)) {
		return 1;
	}

	while(GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

void addColors(HWND handle) {	
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM) "Black");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM) "Green");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM) "Red");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM) "Blue");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM) "White");
}
