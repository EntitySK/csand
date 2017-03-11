#include <time.h>
#include <stdlib.h>
#include "util.h"

int rectContains(rect r, int x, int y) {
	return x >= r.x && x <= r.x + r.width && y >= r.y && y <= r.y + r.height;
}

int rectContains2(int rx, int ry, int width, int height, int x, int y) {
	rect r = {rx, ry, width, height};
	return rectContains(r, x, y);
}

newRandom() {
	time_t t;
	srand((unsigned) time(&t));
}

float randomFloat() {
	return rand()/(float)(RAND_MAX/1);
}

int inCircle(int* centerX, int* centerY, int* x, int* y, int* radius) {
	int distX = *x - *centerX, distY = *y - *centerY;
	int distSquared = (distX * distX) + (distY * distY);
	int rSquared = *radius * *radius;
	return distSquared <= rSquared;
}

void shuffle(int *shuffled, int length, int times) {
    int i,j,k;
    int aux;

    i = j = k = 0;
    for(; i < times; i++) {
        do {
            j=rand() % length;
            k=rand() % length;
        } while (j == k);

        aux = shuffled[j];
        shuffled[j] = shuffled[k];
        shuffled[k] = aux;
    }
}

int* createShuffledSequence(int start, int end, int times) {
	int i = 0, len = end - start + 1;

	int* seq = (int*) calloc(len, sizeof(int));

	for(; i < len; ++i)
		seq[i] = start + i;

	shuffle(seq, len, times);

	return seq;
}

brightenColor(uint32_t* color, uint8_t brightness) {
	rgbaColor col = {*color};
	col.rgba[0] *= brightness;
	col.rgba[1] *= brightness;
	col.rgba[2] *= brightness;

	*color = col.color;
}
