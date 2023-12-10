#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chart.h"

int main(int argc, char** argv) {
	headerInfo header;
	header.fileName = "asdf.bmp";
	header.fileSize = getFileSize(1000, 1000, BITS_PER_PIXEL);
	header.width = 1000;
	header.height = 1000;

	printf("%d\n", writeHeader(header));

	pixelsMap map;
	map = allocatePixelsMap(header);

	fillBackground(map, header, BLACK);

	chartInfo chr;

	printf("%d\n", makeChart("out.a", &chr, header, map));

	//addDivisionNetRGB(chr, 50, 20, 1, 25, 255, 20);

	//drawBarChart(chr, 8, 20, 10, BLACK);
	drawFlowChartRGB(chr, 25, 255, 20);
	//addDivisionNet(chr, 2, 500, 1, BLACK);

	//addDivisionLines(chr, 5, 100, 5, VERTICAL, BLACK);
	//addDivisionLines(chr, 5, 100, 5, HORIZONTAL, BLACK);

//	verticalLine(map, 0, 500, 500, BLACK, header);

	printf("%d\n", writePixelsMap(header, map));

	return 0;
}
