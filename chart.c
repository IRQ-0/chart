#include "chart.h"
#include "colors.h"

// BMP

int writeHeader(headerInfo header) {
	FILE *f = fopen(header.fileName, "wb");
	if (!f) {
		return CANNOT_CREATE_FILE;
	}

	// Write "BM" signature
	fputc('B', f);
	fputc('M', f);

	// Write file size
	if (fwrite(&header.fileSize, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write 4 x 0x00 (2 x 2 reserved bytes)
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);

	// Write starting point for pixels
	int pixelsStarting = FIRST_HEADER_SIZE + SECCOND_HEADER_SIZE;
	if (fwrite(&pixelsStarting, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write seccond header size
	int seccondHeaderSize = SECCOND_HEADER_SIZE;
	if (fwrite(&seccondHeaderSize, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write width
	if (fwrite(&header.width, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write heigth
	if (fwrite(&header.height, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write color planes number (must be 1)
	fputc(0x01, f);
	fputc(0x00, f);

	// Write bits per pixel value
	int bitsPerPixel = BITS_PER_PIXEL;
	if (fwrite(&bitsPerPixel, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Compression (2 x 0x00 byte)
	fputc(0x00, f);
	fputc(0x00, f);

	// Write raw imige size
	int rawImigeSizeSize = (header.width * header.height) * (BITS_PER_PIXEL / 8);
	if (fwrite(&rawImigeSizeSize, 4, 1, f) != 1) {
		return FWRITE_ERROR;
	}

	// Write imige size per meter (used only in printing) (8 x 0x00 byte)
	for (int i = 0; i < 8; i++) {
		fputc(0x00, f);
	}

	// Write colors in palette (4 x 0x00 byte)
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);

	// Write important colors number (4 x 0x00 byte)
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);

	fclose(f);

	return 0;
}

int getFileSize(int width, int height, int bits) {
	return (((width * height) * bits) + (FIRST_HEADER_SIZE + SECCOND_HEADER_SIZE) + 2);
}

unsigned char* allocatePixelsMap(headerInfo info) {
	return (unsigned char*) malloc((info.width * info.height) * (BITS_PER_PIXEL / 8));
}

void putPixel(unsigned char* map, int x, int y, headerInfo info, color color) {
	int startPoint = (((info.width * y) * (BITS_PER_PIXEL / 8)) + (x * (BITS_PER_PIXEL / 8)));
	//printf("%d\n", startPoint);

	switch (color) {
		case BLACK:
			memcpy((map + startPoint), BLACK_RGB, (BITS_PER_PIXEL / 8));
		break;

		case WHITE:
			memcpy((map + startPoint), WHITE_RGB, (BITS_PER_PIXEL / 8));
		break;

		case GREEN:
			memcpy((map + startPoint), GREEN_RGB, (BITS_PER_PIXEL / 8));
		break;

		case BLUE:
			memcpy((map + startPoint), BLUE_RGB, (BITS_PER_PIXEL / 8));
		break;

		case RED:
			memcpy((map + startPoint), RED_RGB, (BITS_PER_PIXEL / 8));
		break;

	default:
		__asm__("nop");
	}
}

void putPixelRGB(pixelsMap map, int x, int y, headerInfo info, unsigned char r, unsigned char g, unsigned char b) {
	int startPoint = (((info.width * y) * (BITS_PER_PIXEL / 8)) + (x * (BITS_PER_PIXEL / 8)));

	// Note that actual order in BMP file is NOT R,G,B, it's B,G,R
	unsigned char bgr[3];
	bgr[0] = b;
	bgr[1] = g;
	bgr[2] = r;

	memcpy((map + startPoint), bgr, (BITS_PER_PIXEL / 8));
}

void fillBackground(pixelsMap map, headerInfo header, color color) {
	switch (color) {
		case BLACK:
			for (int i = 0; i < (header.width * header.height); i++) {
				memcpy((map + (i * (BITS_PER_PIXEL / 8))), BLACK_RGB, (BITS_PER_PIXEL / 8));
			}
		break;

		case WHITE:
			for (int i = 0; i < (header.width * header.height); i++) {
				memcpy((map + (i * (BITS_PER_PIXEL / 8))), WHITE_RGB, (BITS_PER_PIXEL / 8));
			}
		break;

		case GREEN:
			for (int i = 0; i < (header.width * header.height); i++) {
				memcpy((map + (i * (BITS_PER_PIXEL / 8))), GREEN_RGB, (BITS_PER_PIXEL / 8));
			}
		break;

		case BLUE:
			for (int i = 0; i < (header.width * header.height); i++) {
				memcpy((map + (i * (BITS_PER_PIXEL / 8))), BLUE_RGB, (BITS_PER_PIXEL / 8));
			}
		break;

		case RED:
			for (int i = 0; i < (header.width * header.height); i++) {
				memcpy((map + (i * (BITS_PER_PIXEL / 8))), RED_RGB, (BITS_PER_PIXEL / 8));
			}
		break;

	default:
		__asm__("nop");
	}	
}

void fillBackgroundRGB(pixelsMap map, headerInfo header, unsigned char r, unsigned char g, unsigned char b) {
	// Note that actual order in BMP file is NOT R,G,B, it's B,G,R
	unsigned char bgr[3];
	bgr[0] = b;
	bgr[1] = g;
	bgr[2] = r;

	for (int i = 0; i < (header.width * header.height); i++) {
		memcpy((map + (i * (BITS_PER_PIXEL / 8))), bgr, (BITS_PER_PIXEL / 8));
	}
}

int writePixelsMap(headerInfo header, pixelsMap map) {
	FILE *f = fopen(header.fileName, "a");
	if (!f) {
		return FILE_APPEND_ERROR;
	}

	size_t size = ((header.width * header.height) * (BITS_PER_PIXEL / 8));

	#ifdef REVERSE
		int j = 0;
		
		unsigned char* reverse = (unsigned char*) malloc(size);

		for (int i = ((header.width * header.height) - 1); i >= 0; i--) {
			memcpy((reverse + (j++ * (BITS_PER_PIXEL / 8))), (map + (i * (BITS_PER_PIXEL / 8))), (BITS_PER_PIXEL / 8));
		}

		memcpy(map, reverse, size);
		memset(reverse, 0x00, size);

		int lines = (size / (header.width * (BITS_PER_PIXEL / 8)));
		unsigned char* buffer = malloc((header.width * (BITS_PER_PIXEL / 8)));
		unsigned char* revBuffer = malloc((header.width * (BITS_PER_PIXEL / 8)));

		
		for (int i = 0; i < lines; i++) {
			memcpy(buffer, (map + (i * (header.width * (BITS_PER_PIXEL / 8)))), (header.width * (BITS_PER_PIXEL / 8)));

			j = 0;

			for (int k = (header.width - 1); k >= 0; k--) {
				memcpy((revBuffer + (j++ * (BITS_PER_PIXEL / 8))), (buffer + (k * (BITS_PER_PIXEL / 8))), (BITS_PER_PIXEL / 8));
			}

			memcpy((reverse + (i * (header.width * (BITS_PER_PIXEL / 8)))), revBuffer, (header.width * (BITS_PER_PIXEL / 8)));
		}

		memcpy(map, reverse, size);

		free(reverse);
		free(buffer);
		free(revBuffer);

	#endif

	if (fwrite(map, 1, size, f) != size) {
		return FWRITE_ERROR;
	}

	fclose(f);

	return 0;
}

// Chart

int bytesToInt(unsigned char* bytes) {
	int x;
	memcpy(&x, bytes, CHART_ELEMENT_SIZE);
	return x;
}

void intToBytes(int num, unsigned char* bytes) {
	memcpy(bytes, &num, CHART_ELEMENT_SIZE);
}

int makeChart(char* filename, chartInfo* chart, headerInfo info, pixelsMap map) {
	chart->map = map;
	chart->fileName = filename;
	chart->headerInfo = info;

	FILE *f = fopen(filename, "rb");
	if (!f) {
		return FILE_NOT_FOUND;
	}

	chart->filePointer = f;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (size > (info.width * CHART_ELEMENT_SIZE)) {
		return INPUT_FILE_TOO_BIG;
	}

	chart->fileSize = size;

	chartData out = (chartData) malloc(size);
	if (!out) {
		return MALLOC_ERROR;
	}

	if (fread(out, 1, size, f) != (size)) {
		return FREAD_ERROR;
	}

	#ifdef SCALE_DATA

		int max = 0;

		for (int i = 0; i < size; i += CHART_ELEMENT_SIZE) {
			if (bytesToInt(out + i) > max) {
				max = bytesToInt(out + i);
			}
		}

		chartData outFinal = (chartData) malloc(size);
		if (!outFinal) {
			return MALLOC_ERROR;
		}

		for (int i = 0; i < size; i += CHART_ELEMENT_SIZE) {
			intToBytes(((info.height * bytesToInt(out + i)) / max), (outFinal + i));
		}

		chart->rawData = outFinal;
		free(out);

	#else

		chart->rawData = out;

	#endif

	fclose(f);

	return 0;
}

void verticalLine(pixelsMap map, int y0, int y1, int x, color col, headerInfo info) {
	for (int i = 0; i < (y1 - y0); i++) {
		putPixel(map, x, (y0 + i), info, col);
	}
}

void verticalLineRGB(pixelsMap map, int y0, int y1, int x, unsigned char r, unsigned char g, unsigned char b, headerInfo info) {
	for (int i = 0; i < (y1 - y0); i++) {
		putPixelRGB(map, x, (y0 + i), info, r, g, b);
	}
}

void horizontalLine(pixelsMap map, int x0, int x1, int y, color col, headerInfo info) {
	for (int i = 0; i < (x1 - x0); i++) {
		putPixel(map, (x0 + i), y, info, col);
	}
}

void horizontalLineRGB(pixelsMap map, int x0, int x1, int y, unsigned char r, unsigned char g, unsigned char b, headerInfo info) {
	for (int i = 0; i < (x1 - x0); i++) {
		putPixelRGB(map, (x0 + i), y, info, r, g, b);
	}
}

void drawFlowChart(chartInfo chrt, color col) {
	unsigned char buffer[CHART_ELEMENT_SIZE];

	for (int i = 0; i < (chrt.fileSize / CHART_ELEMENT_SIZE); i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);
		verticalLine(chrt.map, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo.height, i, col, chrt.headerInfo);
	}
}

void drawFlowChartRGB(chartInfo chrt, unsigned char r, unsigned char g, unsigned char b) {
	unsigned char buffer[CHART_ELEMENT_SIZE];

	for (int i = 0; i < (chrt.fileSize / CHART_ELEMENT_SIZE); i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);
		verticalLineRGB(chrt.map, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo.height, i, r, g, b, chrt.headerInfo);
	}
}

void drawDotChart(chartInfo chrt, color col) {
	unsigned char buffer[CHART_ELEMENT_SIZE];

	for (int i = 0; i < (chrt.fileSize / CHART_ELEMENT_SIZE); i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);
		putPixel(chrt.map, i, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo, col);
	}
}

void drawDotChartRGB(chartInfo chrt, unsigned char r, unsigned char g, unsigned char b) {
	unsigned char buffer[CHART_ELEMENT_SIZE];

	for (int i = 0; i < (chrt.fileSize / CHART_ELEMENT_SIZE); i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);
		putPixelRGB(chrt.map, i, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo, r, g, b);
	}
}

void drawBarChart(chartInfo chrt, int bars, int barSpace, int barWidth, color col) {
	unsigned char buffer[CHART_ELEMENT_SIZE];
	int counter = 0;

	for (int i = 0; i < bars; i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);

		for (int j = 0; j < barWidth; j++) {
			verticalLine(chrt.map, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo.height, (counter + j), col, chrt.headerInfo);
		}

		counter += barSpace;
	}
}

void drawBarChartRGB(chartInfo chrt, int bars, int barSpace, int barWidth, unsigned char r, unsigned char g, unsigned char b) {
	unsigned char buffer[CHART_ELEMENT_SIZE];
	int counter = 0;

	for (int i = 0; i < bars; i++) {
		memcpy(buffer, (chrt.rawData + (i * CHART_ELEMENT_SIZE)), CHART_ELEMENT_SIZE);

		for (int j = 0; j < barWidth; j++) {
			verticalLineRGB(chrt.map, (chrt.headerInfo.height - bytesToInt(buffer)), chrt.headerInfo.height, (counter + j), r, g, b, chrt.headerInfo);
		}

		counter += barSpace;
	}
}

void addDivisionNet(chartInfo chrt, int lines, int space, int width, color col) {
	int counter = 0;

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < width; j++) {
			verticalLine(chrt.map, 0, chrt.headerInfo.width, (counter + j), col, chrt.headerInfo);
			horizontalLine(chrt.map, 0, chrt.headerInfo.height, (counter + j), col, chrt.headerInfo);
		}

		counter += space;
	}

}

void addDivisionNetRGB(chartInfo chrt, int lines, int space, int width, unsigned char r, unsigned char g, unsigned char b) {
	int counter = 0;

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < width; j++) {
			verticalLineRGB(chrt.map, 0, chrt.headerInfo.width, (counter + j), r, g, b, chrt.headerInfo);
			horizontalLineRGB(chrt.map, 0, chrt.headerInfo.height, (counter + j), r, g, b, chrt.headerInfo);
		}

		counter += space;
	}
}

void addDivisionLines(chartInfo chrt, int lines, int space, int width, int type, color col) {
	int counter = 0;

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < width; j++) {
			if (type == HORIZONTAL) {
				horizontalLine(chrt.map, 0, chrt.headerInfo.height, (counter + j), col, chrt.headerInfo);
			} else if (type == VERTICAL) {
				verticalLine(chrt.map, 0, chrt.headerInfo.width, (counter + j), col, chrt.headerInfo);	
			} else {
				__asm__("nop");
			}
		}

		counter += space;
	}
}

void addDivisionLinesRGB(chartInfo chrt, int lines, int space, int width, int type, unsigned char r, unsigned char g, unsigned char b) {
	int counter = 0;

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < width; j++) {
			if (type == HORIZONTAL) {
				horizontalLineRGB(chrt.map, 0, chrt.headerInfo.height, (counter + j), r, g, b, chrt.headerInfo);
			} else if (type == VERTICAL) {
				verticalLineRGB(chrt.map, 0, chrt.headerInfo.width, (counter + j), r, g, b, chrt.headerInfo);	
			} else {
				__asm__("nop");
			}
		}

		counter += space;
	}	
}
