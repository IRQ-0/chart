#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "chart.h"

#define FLOW 1
#define DOT  2
#define BAR  3

int main(int argc, char** argv) {
	headerInfo header;

	if (argc < 8) {
		puts("usage: chart <filename in> <filename out> <width> <height> <background> <chart type> [options] <chart color> [dividion nets] [options] [division net color]");
		puts("\nAvailable backgrounds:\n - black \n - green \n - red \n - blue \n - white");
		puts("\n\nAvailable charts:\n - flow (no options needed) \n - dot (no options needed) \n - bar <bar count> <bar space> <bar width>");
		puts("\nAvailable chart colors:\n - black \n - green \n - red \n - blue \n - white");
		puts("\nDivision nets and lines:\n - net <net count> <net space> <net width> \n - lines <lines count> <lines space> <lines width> <lines type> type: horizontal / vertical");

		return 2;
	}

	char buf;
	for (int i = 3; i < argc; i++) {
		for (int j = 0; j < strlen(argv[i]); j++) {
			buf = *(argv[i] + j);
			*(argv[i] + j) = tolower(buf);
		}
	}

	if (atoi(argv[3]) <= 0) {
		puts("Invalid width");

		return 1;
	}

	if (atoi(argv[4]) <= 0) {
		puts("Invalid height");

		return 1;
	}

	header.fileName = argv[2];
	header.fileSize = getFileSize(atoi(argv[3]), atoi(argv[4]), BITS_PER_PIXEL);
	header.width = atoi(argv[3]);
	header.height = atoi(argv[4]);

	int err = writeHeader(header);

	if (err != 0) {
		printf("Header error: %d\n", err);

		return 1;
	}

	pixelsMap map;
	map = allocatePixelsMap(header);

	if (strcmp(argv[5], "black") == 0) {
		fillBackground(map, header, BLACK);

	} else if (strcmp(argv[5], "green") == 0) {
		fillBackground(map, header, GREEN);

	} else if (strcmp(argv[5], "red") == 0) {
		fillBackground(map, header, RED);

	} else if (strcmp(argv[5], "blue") == 0) {
		fillBackground(map, header, BLUE);

	} else if (strcmp(argv[5], "white") == 0) {
		fillBackground(map, header, WHITE);

	} else {
		puts("Unrecognized predfifined background");

		return 1;
	}

	chartInfo chr;

	err = makeChart(argv[1], &chr, header, map);

	if (err != 0) {
		printf("Chart error %d\n", err);

		return 1;
	}

	int chartType = 0, chartColor = 0, valueOffset = 0;

	if (strcmp(argv[6], "flow") == 0) {
		chartType = FLOW;
		valueOffset = 7;

	} else if (strcmp(argv[6], "dot") == 0) {
		chartType = DOT;
		valueOffset = 7;

	} else if (strcmp(argv[6], "bar") == 0) {
		if (argc == 7) {
			puts("Not enough parameters");

			return 2;
		}

		chartType = BAR;
		valueOffset = 10;

	} else {
		puts("Unrecognized chart type");

		return 1;
	}


	if (strcmp(argv[valueOffset], "black") == 0) {
		chartColor = BLACK;

	} else if (strcmp(argv[valueOffset], "green") == 0) {
		chartColor = GREEN;

	} else if (strcmp(argv[valueOffset], "red") == 0) {
		chartColor = RED;

	} else if (strcmp(argv[valueOffset], "blue") == 0) {
		chartColor = BLUE;

	} else if (strcmp(argv[valueOffset], "white") == 0) {
		chartColor = WHITE;

	} else {
		puts("Unrecognized predfifined chart color");

		return 1;
	}

	switch (chartType) {

	case FLOW:
		drawFlowChart(chr, chartColor);

		break;

	case DOT:
		drawDotChart(chr, chartColor);

		break;

	case BAR:
		if (atoi(argv[7]) <= 0 || atoi(argv[8]) <= 0 || atoi(argv[9]) <= 0) {
			puts("Invalid chart parameters");

			return 1;
		}

		drawBarChart(chr, atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), chartColor);

		break;
	}

	valueOffset++;

	printf("argc: %d  valueOffset: %d\n", argc, valueOffset);
	if (argc != valueOffset) {
		

		if (strcmp(argv[valueOffset], "net") == 0) {

			if (argc != (valueOffset + 5)) {
				puts("Not enough parameters");

				return 2;
			}

			if (atoi(argv[valueOffset + 1]) <= 0 || atoi(argv[valueOffset + 2]) <= 0 || atoi(argv[valueOffset + 3]) <= 0) {
				puts("Invalid net parameters");

				return 1;
			}

			int netColor = 0;

			if (strcmp(argv[valueOffset + 4], "black") == 0) {
				netColor = BLACK;

			} else if (strcmp(argv[valueOffset + 4], "green") == 0) {
				netColor = GREEN;

			} else if (strcmp(argv[valueOffset + 4], "red") == 0) {
				netColor = RED;

			} else if (strcmp(argv[valueOffset + 4], "blue") == 0) {
				netColor = BLUE;

			} else if (strcmp(argv[valueOffset + 4], "white") == 0) {
				netColor = WHITE;

			} else {
				puts("Unrecognized predfifined chart color");

				return 1;
			}

			addDivisionNet(chr, atoi(argv[valueOffset + 1]), atoi(argv[valueOffset + 2]), atoi(argv[valueOffset + 3]), netColor);

		} else if (strcmp(argv[valueOffset], "lines") == 0) {
			if (argc != (valueOffset + 6)) {
				puts("Not enough parameters");

				return 2;
			}

			if (atoi(argv[valueOffset + 1]) <= 0 || atoi(argv[valueOffset + 2]) <= 0 || atoi(argv[valueOffset + 3]) <= 0) {
				puts("Invalid net parameters");

				return 1;
			}

			int lineType = 0;

			if (strcmp(argv[valueOffset + 4], "horizontal") == 0) {
				lineType = HORIZONTAL;

			} else if (strcmp(argv[valueOffset + 4], "vertical") == 0) {
				lineType = VERTICAL;

			} else {
				puts("Unrecognized lines type");

				return 1;
			}

			int lineColor = 0;

			if (strcmp(argv[valueOffset + 5], "black") == 0) {
				lineColor = BLACK;

			} else if (strcmp(argv[valueOffset + 5], "green") == 0) {
				lineColor = GREEN;

			} else if (strcmp(argv[valueOffset + 5], "red") == 0) {
				lineColor = RED;

			} else if (strcmp(argv[valueOffset + 5], "blue") == 0) {
				lineColor = BLUE;

			} else if (strcmp(argv[valueOffset + 5], "white") == 0) {
				lineColor = WHITE;

			} else {
				puts("Unrecognized predfifined chart color");

				return 1;
			}

			addDivisionLines(chr, atoi(argv[valueOffset + 1]), atoi(argv[valueOffset + 2]), atoi(argv[valueOffset + 3]), lineType, lineColor);
		}
	}

	err = writePixelsMap(header, map);

	if (err != 0) {
		printf("Writing error: %d\n", err);

		return 1;
	}

	return 0;
}
