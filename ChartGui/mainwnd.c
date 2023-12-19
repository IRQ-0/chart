#include "mainwnd.h"

#define LARGE_BUFFER_SIZE 1024
#define SMALL_BUFFER_SIZE 32

#define EXIT return DefWindowProc(hWnd, msg, wParam, lParam);

#define CHART_APP_NAME "..\\main.exe"

extern HWND hMainWnd, hInputFilename, hOutputFilename, hWidth, hHeight, hChartBackground, hChartColor, hChartType, hBarCount, hBarSpace, hBarWidth, hNetType, hNetColor, hNetCount, hNetSpace, hNetWidth, hLineType, hGenerate, hShowAtEnd;
extern HINSTANCE hAppInstance;

HDC kon;
PAINTSTRUCT ps;

char itemStr[LARGE_BUFFER_SIZE];
int itemIndex;
char atoiBuffer[SMALL_BUFFER_SIZE];

// In / out files
char inputFile[LARGE_BUFFER_SIZE];
char outputFile[LARGE_BUFFER_SIZE];
	
// File size
int width;
int height;
	
// Chart params
char chartType[SMALL_BUFFER_SIZE];
char chartBackColor[SMALL_BUFFER_SIZE];
char chartColor[SMALL_BUFFER_SIZE];
	
// Only if chartType == Bar
int barCount;
int barSpace;
int barWidth;
	
// Division nets
char netType[SMALL_BUFFER_SIZE];
int netCount;
int netSpace;
int netWidth;
char netColor[SMALL_BUFFER_SIZE];
	
// Only if netType == Lines
char lineType[SMALL_BUFFER_SIZE];

// Show after creating
int showAfter;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	
	case WM_CREATE:
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE) {
            itemIndex = SendMessage(lParam, CB_GETCURSEL, 0, 0);
            SendMessage(lParam, CB_GETLBTEXT, itemIndex, itemStr);
            
            if (lParam == hChartType) {
            	strcpy(chartType, itemStr);
			} else if (lParam == hChartBackground) {
				strcpy(chartBackColor, itemStr);
			} else if (lParam == hChartColor) {
				strcpy(chartColor, itemStr);
			} else if (lParam == hNetType) {
				strcpy(netType, itemStr);
			} else if (lParam == hLineType) {
				strcpy(lineType, itemStr);
			} else if (lParam == hNetColor) {
				strcpy(netColor, itemStr);
			}
			
			if (strcmp(netType, "None") == 0) {
				EnableWindow(hNetColor, FALSE);
				EnableWindow(hNetSpace, FALSE);
				EnableWindow(hNetCount, FALSE);
				EnableWindow(hNetWidth, FALSE);
				EnableWindow(hLineType, FALSE);
			} else if (strcmp(netType, "Net") == 0) {
				EnableWindow(hNetColor, TRUE);
				EnableWindow(hNetSpace, TRUE);
				EnableWindow(hNetCount, TRUE);
				EnableWindow(hNetWidth, TRUE);
				EnableWindow(hLineType, FALSE);
			} else if (strcmp(netType, "Lines") == 0) {
				EnableWindow(hNetColor, TRUE);
				EnableWindow(hNetSpace, TRUE);
				EnableWindow(hNetCount, TRUE);
				EnableWindow(hNetWidth, TRUE);
				EnableWindow(hLineType, TRUE);
			}
			
			if (strcmp(chartType, "Bar") == 0) {
				EnableWindow(hBarCount, TRUE);
				EnableWindow(hBarSpace, TRUE);
				EnableWindow(hBarWidth, TRUE);
			} else {
				EnableWindow(hBarCount, FALSE);
				EnableWindow(hBarSpace, FALSE);
				EnableWindow(hBarWidth, FALSE);
			}
			
        } else if (lParam == hGenerate) {
        	memset(atoiBuffer, 0, SMALL_BUFFER_SIZE);
        	
        	getText(hInputFilename, inputFile);
        	getText(hOutputFilename, outputFile);
        	
        	getText(hWidth, atoiBuffer);
        	width = atoi(atoiBuffer);
        	if (width <= 0) {
        		MessageBox(hMainWnd, "Error in width value", WND_TITLE, MB_ICONERROR);
        		EXIT;
			}
			
			getText(hHeight, atoiBuffer);
        	height = atoi(atoiBuffer);
        	if (height <= 0) {
        		MessageBox(hMainWnd, "Error in height value", WND_TITLE, MB_ICONERROR);
        		EXIT;
			}
			
			if (strcmp(chartType, "") == 0) {
				MessageBox(hMainWnd, "Chart type not selected", WND_TITLE, MB_ICONERROR);
				EXIT;
			}
			
			if (strcmp(chartBackColor, "") == 0) {
				MessageBox(hMainWnd, "Chart background color not selected", WND_TITLE, MB_ICONERROR);
				EXIT;
			}
			
			if (strcmp(chartColor, "") == 0) {
				MessageBox(hMainWnd, "Chart color not selected", WND_TITLE, MB_ICONERROR);
				EXIT;
			}
			
			if (strcmp(netType, "") == 0) {
				MessageBox(hMainWnd, "Net type not selected", WND_TITLE, MB_ICONERROR);
				EXIT;
			}
			
			if (strcmp(chartType, "Bar") == 0) {
				getText(hBarCount, atoiBuffer);
        		barCount = atoi(atoiBuffer);
        		if (barCount <= 0) {
        			MessageBox(hMainWnd, "Error in bar count value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
				
				getText(hBarSpace, atoiBuffer);
        		barSpace = atoi(atoiBuffer);
        		if (barSpace <= 0) {
        			MessageBox(hMainWnd, "Error in bar space value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
				
				getText(hBarWidth, atoiBuffer);
        		barWidth = atoi(atoiBuffer);
        		if (barWidth <= 0) {
        			MessageBox(hMainWnd, "Error in bar width value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
			}
			
			if (strcmp(netType, "None") != 0) {
				if (strcmp(netColor, "") == 0) {
					MessageBox(hMainWnd, "Net color not selected", WND_TITLE, MB_ICONERROR);
					EXIT;
				}
			
				getText(hNetCount, atoiBuffer);
        		netCount = atoi(atoiBuffer);
        		if (netCount <= 0) {
        			MessageBox(hMainWnd, "Error in net count value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
				
				getText(hNetSpace, atoiBuffer);
        		netSpace = atoi(atoiBuffer);
        		if (netSpace <= 0) {
        			MessageBox(hMainWnd, "Error in net space value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
				
				getText(hNetWidth, atoiBuffer);
        		netWidth = atoi(atoiBuffer);
        		if (netWidth <= 0) {
        			MessageBox(hMainWnd, "Error in net width value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
				
				if (strcmp(netType, "Lines") == 0 && strcmp(lineType, "") == 0) {
					MessageBox(hMainWnd, "Error in lines type value", WND_TITLE, MB_ICONERROR);
	        		EXIT;
				}
			}
			
			char commandBuffer[LARGE_BUFFER_SIZE];
			char commandNetBuffer[LARGE_BUFFER_SIZE];
			
			// chart <filename in> <filename out> <width> <height> <background> <chart type> [options] <chart color> [dividion nets] [options] [division net color]
			if (strcmp(chartType, "Bar") == 0) {
				sprintf(commandBuffer, "%s %s %s %d %d %s %s %d %d %d %s",
									   CHART_APP_NAME,
									   inputFile,
									   outputFile,
									   width,
									   height,
									   chartBackColor,
									   chartType,
									   barCount,
									   barSpace,
									   barWidth,
									   chartColor);
			} else {
				sprintf(commandBuffer, "%s %s %s %d %d %s %s %s",
									   CHART_APP_NAME,
									   inputFile,
									   outputFile,
									   width,
									   height,
									   chartBackColor,
									   chartType,
									   chartColor);
			}
			
			if (strcmp(netType, "None") != 0) {
				if (strcmp(netType, "Lines") == 0) {
					sprintf(commandNetBuffer, " %s %d %d %d %s %s",
											  netType,
											  netCount,
											  netSpace,
											  netWidth,
											  lineType,
											  netColor);
					
				} else {
					sprintf(commandNetBuffer, " %s %d %d %d %s",
											  netType,
											  netCount,
											  netSpace,
											  netWidth,
											  netColor);
				}
				
				strcat(commandBuffer, commandNetBuffer);
			}
			
        	if (system(commandBuffer) != 0) {
        		MessageBox(hMainWnd, "Error while creating chart", WND_TITLE, MB_ICONERROR);
        		EXIT;
        		
			} else {
				if (showAfter) {
					sprintf(commandBuffer, "start %s", outputFile);
					
					if (system(commandBuffer) != 0) {
						MessageBox(hMainWnd, "Error while opening chart", WND_TITLE, MB_ICONERROR);
						EXIT;
					}
				}
			}
        	
		} else if (lParam == hShowAtEnd) {
			if (showAfter == 0) {
				SendMessage(hShowAtEnd, BM_SETCHECK, BST_CHECKED, 0);
				showAfter = 1;
			} else {
				SendMessage(hShowAtEnd, BM_SETCHECK, BST_UNCHECKED, 0);
				showAfter = 0;
			}
			
		}
        
		break;
		
	case WM_PAINT:
		kon = BeginPaint(hWnd, &ps);
		
		printText(10, 29, "Input file: ", kon);
		printText(10, 69, "Output file: ", kon);
		
		printText(10, 119, "Width: ", kon);
		printText(10, 159, "Height: ", kon);
		
		printText(10, 209, "Chart type: ", kon);
		printText(10, 249, "Chart back: ", kon);
		printText(10, 289, "Chart color: ", kon);
		
		printText(255, 209, "Count: ", kon);
		printText(255, 249, "Space: ", kon);
		printText(255, 289, "Width: ", kon);
		
		printText(10, 333, "Net type: ", kon);
		
		printText(10, 374, "Net count: ", kon);
		printText(10, 414, "Net space: ", kon);
		printText(10, 454, "Net width: ", kon);
		
		printText(10, 494, "Net color: ", kon);
		
		printText(255, 334, "Line type: ", kon);
		
		EndPaint(hWnd, &ps);
	break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}


inline void printText(int x, int y, char* text, HDC ps) {
	TextOut(ps, x, y, text, strlen(text));
}

inline void ref(void) {
	InvalidateRect(hMainWnd, 0, TRUE);
}

int getText(HWND handle, char* out) {
	int length = 0;
	length = (DWORD)GetWindowTextLengthA(handle);
	LPSTR str = (LPSTR)GlobalAlloc(GPTR, length);
	GetWindowText(handle, (LPSTR)str, length + 1);
	strcpy(out, str);
	
	return length;
}


