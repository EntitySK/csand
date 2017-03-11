#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdint.h>

typedef struct rect {
	int x, y, width, height;
} rect;

int rectContains(rect r, int x, int y);
int rectContain2(int rx, int ry, int width, int height, int x, int y);

newRandom();
float randomFloat();

int inCircle(int* centerX, int* centerY, int* x, int* y, int* radius);

void shuffle(int *shuffled, int length, int times);
int* createShuffledSequence(int start, int end, int times);

typedef union {
	uint32_t color;
	uint8_t rgba[4];
} rgbaColor;

brightenColor(uint32_t* color, uint8_t brightness);

#endif // UTIL_H_INCLUDED
