#ifndef CHART_H
#define CHART_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	BLACK,
	GREEN,
	RED,
	BLUE,
	WHITE
} color;

#define FILE_NOT_FOUND     1
#define MALLOC_ERROR       2
#define FREAD_ERROR        3
#define CANNOT_CREATE_FILE 4
#define FWRITE_ERROR       5
#define FILE_APPEND_ERROR  6
#define INPUT_FILE_TOO_BIG 7

#define FIRST_HEADER_SIZE   14
#define SECCOND_HEADER_SIZE 40

#define BITS_PER_PIXEL 24

#define CHART_ELEMENT_SIZE 4

#define HORIZONTAL 1
#define VERTICAL   2

#define REVERSE 1

#define SCALE_DATA 1

typedef struct {
	char* fileName;
	int fileSize;
	int width;
	int height;
} headerInfo;

typedef unsigned char* pixelsMap;
typedef unsigned char* chartData;

typedef struct {
	char* fileName;
	size_t fileSize;
	FILE* filePointer;
	pixelsMap map;
	chartData rawData;

	headerInfo headerInfo;
} chartInfo;

// BMP
int writeHeader(headerInfo);
int getFileSize(int, int, int);
pixelsMap allocatePixelsMap(headerInfo);
void putPixel(pixelsMap, int, int, headerInfo, color);
void putPixelRGB(pixelsMap, int, int, headerInfo, unsigned char, unsigned char, unsigned char);
void fillBackground(pixelsMap, headerInfo, color);
void fillBackgroundRGB(pixelsMap, headerInfo, unsigned char, unsigned char, unsigned char);
int writePixelsMap(headerInfo, pixelsMap);

// Chart
void verticalLine(pixelsMap, int, int, int, color, headerInfo);
void verticalLineRGB(pixelsMap, int, int, int, unsigned char, unsigned char, unsigned char, headerInfo);

void horizontalLine(pixelsMap, int, int, int, color, headerInfo);
void horizontalLineRGB(pixelsMap, int, int, int, unsigned char, unsigned char, unsigned char, headerInfo);

int makeChart(char*, chartInfo*, headerInfo, pixelsMap);
int bytesToInt(unsigned char*);
void intToBytes(int, unsigned char*);

void drawFlowChart(chartInfo, color);
void drawFlowChartRGB(chartInfo, unsigned char, unsigned char, unsigned char);

void drawDotChart(chartInfo, color);
void drawDotChartRGB(chartInfo, unsigned char, unsigned char, unsigned char);

void drawBarChart(chartInfo, int, int, int, color);
void drawBarChartRGB(chartInfo, int, int, int, unsigned char, unsigned char, unsigned char);

void addDivisionNet(chartInfo, int, int, int, color);
void addDivisionNetRGB(chartInfo, int, int, int, unsigned char, unsigned char, unsigned char);

void addDivisionLines(chartInfo, int, int, int, int, color);
void addDivisionLinesRGB(chartInfo, int, int, int, int, unsigned char, unsigned char, unsigned char);


#endif