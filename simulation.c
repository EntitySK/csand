#include <stdio.h>
#include "simulation.h"
#include "util.h"

element** createArray(int width, int height) {
	element*  vals = (element* ) malloc(width * height * sizeof(element));
	element** rows = (element**) malloc(height * sizeof(element*));

	int i = 0;
	for (; i < height; ++i) {
		rows[i] = vals + i * width;
	}
	return rows;
}

destroyArray(element** arr) {
	free(*(arr));
	free(arr);
}

simulation* createSimulation(int width, int height) {
	simulation* sim = (simulation*) malloc(sizeof(simulation));
	sim->width = width;
	sim->height = height;
	sim->arr = createArray(width + 2, height + 2);

	return sim;
}

destroySimulation(simulation* sim) {
	destroyArray(sim->arr);
	free(sim);
}

paintElement(simulation* sim, element el, int x, int y, int radius) {
	int i = x - radius < 0 ? 0 : x - radius;
	for (; i <= x + radius && i <= sim->width; ++i) {
		int j = y - radius < 0 ? 0 : y - radius;
		for (; j <= y + radius && j <= sim->height; ++j) {
			if (inCircle(&x, &y, &i, &j, &radius))
				sim->arr[i + 1][j + 1] = el;
		}
	}
}

simulate(simulation* sim) {
	newRandom();
	int
		*rowShuffle = createShuffledSequence(1, sim->width,  sim->width),
		*colShuffle = createShuffledSequence(1, sim->height, sim->height);

	int i = 0;
	for(; i <= sim->width - 1; ++i) {
		int j = 0;
		for (; j <= sim->height - 1; ++j) {
			locale* loc = createLocale(sim->arr,  rowShuffle[i], colShuffle[j]);

			simulateElement(loc);

			destroyLocale(loc);
		}
	}

	free(rowShuffle);
	free(colShuffle);
}

addProperties(element* el, elementProperty prop) {
	*el |= prop;
}

removeProperties(element* el, elementProperty prop) {
	*el &= ~prop;
}

int hasProperties(element* el, elementProperty prop) {
	return (*el & prop) == prop;
}

set(element* el, element to) {
	*el = to;
}

swap(element* a, element* b) {
	element e1 = *a, e2 = *b;
	*a = e2;
	*b = e1;
}

simulateSpout(locale* loc) {
	element base = *loc->el;
	removeProperties(&base, PR_SPOUT);

	localeArray lar = {*loc};
	int i = 0, j = 0;
	for (; i < 3; ++i)
		for (; j < 3; ++j)
			if (i != 1 && j != 1 && !hasProperties(lar.arr[i][j], PR_EXISTS))
				set(lar.arr[i][j], base);
}

simulateFluid(locale* loc) {
	if (!hasProperties(loc->bot, PR_EXISTS))
		swap(loc->el, loc->bot);
	else {
		int
			leftFree  = !hasProperties(loc->left,  PR_EXISTS),
			rightFree = !hasProperties(loc->right, PR_EXISTS);
			if (leftFree && rightFree)
				srand() & 1 ? swap(loc->el, loc->left) : swap(loc->el, loc->right);
			else if (leftFree)
				swap(loc->el, loc->left);
			else if (rightFree)
				swap(loc->el, loc->right);
	}
}

simulateGas(locale* loc) {
	if ((!hasProperties(loc->top, PR_EXISTS) || hasProperties(loc->top, PR_FLUID) || hasProperties(loc->top, PR_GAS))) {
		swap(loc->el, loc->top);
		}
	else {
		int
			leftFree  = (!hasProperties(loc->left,  PR_EXISTS) || hasProperties(loc->left,  PR_FLUID) || hasProperties(loc->left,  PR_GAS)),
			rightFree = (!hasProperties(loc->right, PR_EXISTS) || hasProperties(loc->right, PR_FLUID) || hasProperties(loc->right, PR_GAS));
			if (leftFree && rightFree)
				srand() & 1 ? swap(loc->el, loc->left) : swap(loc->el, loc->right);
			else if (leftFree)
				swap(loc->el, loc->left);
			else if (rightFree)
				swap(loc->el, loc->right);
	}
}

simulateMomentary(locale* loc) {
	if (randomFloat() > 0.25f)
		set(loc->el, EL_AIR);
}

simulateHot(locale* loc) {
	localeArray lar = {*loc};
	int i = 0, j = 0;
	for (; i < 3; ++i)
		for (; j < 3; ++j)
			if (i != 1 && j != 1 && hasProperties(lar.arr[i][j], PR_FLAMMABLE))
				set(lar.arr[i][j], EL_FIRE);
}

simulatePlant(locale* loc) {
	localeArray lar = {*loc};
	int i = 0, j = 0;
	for (; i < 3; ++i)
		for (; j < 3; ++j)
			if (i != 1 && j != 1 && *lar.arr[i][j] == EL_WATER)
				set(lar.arr[i][j], EL_PLANT);
}

simulateVaporizes(locale* loc) {
	if (hasProperties(loc->bot, PR_HOT))
		set(loc->el, EL_STEAM);
}

simulateCondenses(locale* loc) {
	if (randomFloat() > 0.95)
		set(loc->el, EL_WATER);
}

simulateElement(locale* loc) {
	if (hasProperties(loc->el, PR_EXISTS)) {

		if (hasProperties(loc->el, PR_SPOUT))
			simulateSpout(loc);
		else {
			if (hasProperties(loc->el, PR_FLUID))
				simulateFluid(loc);
			if (hasProperties(loc->el, PR_GAS))
				simulateGas(loc);
			if (hasProperties(loc->el, PR_HOT))
				simulateHot(loc);
			if (hasProperties(loc->el, PR_MOMENTARY))
				simulateMomentary(loc);
			if (hasProperties(loc->el, PR_PLANT))
				simulatePlant(loc);
			if (hasProperties(loc->el, PR_VAPORIZES))
				simulateVaporizes(loc);
			if (hasProperties(loc->el, PR_CONDENSES))
				simulateCondenses(loc);
		}
	}
}

locale* createLocale(element** arr, int x, int y) {
	locale temp = (locale) {
		&arr[x - 1][y - 1], &arr[x   ][y - 1], &arr[x + 1][y - 1],
		&arr[x - 1][y    ], &arr[x   ][y    ], &arr[x + 1][y    ],
		&arr[x - 1][y + 1], &arr[x   ][y + 1], &arr[x + 1][y + 1],
	};

	locale* loc = (locale*) malloc(sizeof(locale));
	memcpy(loc, &temp, sizeof(locale));
	return loc;
}

destroyLocale(locale* loc) { free(loc); }

uint32_t elementBaseColor(element base) {
	switch(base) {
		case EL_AIR  : return 0x1A1A1A;
		case EL_WALL : return 0xAAAAAA;
		case EL_WATER: return 0x0000FF;
		case EL_STEAM: return 0xCCFFFF;
		case EL_SAND : return 0xFFFFCC;
		case EL_OIL  : return 0x996633;
		case EL_FIRE : return 0xFF0000;
		case EL_PLANT: return 0x33cc33;
		case EL_LAVA : return 0xCC0000;
		default: return 0xFF69B4;
	}
}

uint32_t elementColor(element* el) {
	element base = *el;
	removeProperties(&base, PR_SPOUT);

	uint32_t color = elementBaseColor(base);

	if (hasProperties(el, PR_SPOUT))
		brightenColor(&color, 2);

	return color;
}

uint32_t* sim2gfx(simulation* sim) {
	uint32_t* pixels = (uint32_t*) malloc(sim->width * sim->height * sizeof(uint32_t));

	int i = 0;
	for (; i <= sim->width; ++i) {
		int j = 0;
		for (; j <= sim->height; ++j) {
			int offset = (i * sim->width) + j;
			pixels[offset] = elementColor(&sim->arr[j + 1][i + 1]);
		}
	}
	return pixels;
}

/*
#include <stdio.h>
#include "simulation.h"
#include "util.h"

element** createArray(int width, int height) {
	element*  vals = (element* ) malloc(width * height * sizeof(element));
	element** rows = (element**) malloc(height * sizeof(element*));

	int i = 0;
	for (; i < height; ++i) {
		rows[i] = vals + i * width;
	}
	return rows;
}

destroyArray(element** arr) {
	free(*(arr));
	free(arr);
}

simulation* createSimulation(int width, int height) {
	simulation* sim = (simulation*) malloc(sizeof(simulation));
	sim->width = width;
	sim->height = height;
	sim->arr = createArray(width + 2, height + 2);

	return sim;
}

destroySimulation(simulation* sim) {
	destroyArray(sim->arr);
	free(sim);
}

paintElement(simulation* sim, element el, int x, int y, int radius) {
	int i = x - radius < 0 ? 0 : x - radius;
	for (; i <= x + radius && i <= sim->width; ++i) {
		int j = y - radius < 0 ? 0 : y - radius;
		for (; j <= y + radius && j <= sim->height; ++j) {
			if (inCircle(&x, &y, &i, &j, &radius))
				sim->arr[i + 1][j + 1] = el;
		}
	}
}

//TODO with randomization, do we need to create a new array every frame?
simulate(simulation* sim) {
	element** nextState = createArray(sim->width + 2, sim->height + 2);

	newRandom();
	int
		*rowShuffle = createShuffledSequence(1, sim->width,  sim->width),
		*colShuffle = createShuffledSequence(1, sim->height, sim->height);

	int i = 0;
	for(; i <= sim->width - 1; ++i) {
		int j = 0;
		for (; j <= sim->height - 1; ++j) {
			locale
				*loc     = createLocale(sim->arr,  rowShuffle[i], colShuffle[j]),
				*nextLoc = createLocale(nextState, rowShuffle[i], colShuffle[j]);

			simulateElement(loc, nextLoc);

			destroyLocale(loc);
			destroyLocale(nextLoc);
		}
	}

	free(rowShuffle);
	free(colShuffle);

	destroyArray(sim->arr);
	sim->arr = nextState;
}

addProperties(element* el, elementProperty prop) {
	*el |= prop;
}

removeProperties(element* el, elementProperty prop) {
	*el &= ~prop;
}

int hasProperties(element* el, elementProperty prop) {
	return (*el & prop) == prop;
}

set(element* el, element to) {
	*el = to;
}

emplace(element* el, element* to) {
	*to = *el;
}

swap(element* a, element* b) {
	element temp = *a;
	*b = *a;
	*a = temp;
}

int simulateSpout(locale* loc, locale* nextLoc) {
	element base = *loc->el;
	removeProperties(&base, PR_SPOUT);

	localeArray lar = {*loc}, nextLar = {*nextLoc};
	int i = 0, j = 0;
	for (; i < 3; ++i) {
		for (; j < 3; ++j) {
			if (i != 1 && j != 1 && !hasProperties(lar.arr[i][j], PR_EXISTS) && !hasProperties(nextLar.arr[i][j], PR_EXISTS))
				set(nextLar.arr[i][j], base);
		}
	}
	return 0;

	if (!hasProperties(loc->bot, PR_EXISTS))
		set(loc->bot, base);
	return 0;
}

int simulateFluid(locale* loc, locale* nextLoc) {
	if (!hasProperties(loc->bot, PR_EXISTS) && !hasProperties(nextLoc->bot, PR_EXISTS))
		emplace(loc->el, nextLoc->bot);
	else {
		int
			leftFree  = !hasProperties(loc->left,  PR_EXISTS) && !hasProperties(nextLoc->left,  PR_EXISTS),
			rightFree = !hasProperties(loc->right, PR_EXISTS) && !hasProperties(nextLoc->right, PR_EXISTS);
			if (leftFree && rightFree)
				srand() & 1 ? emplace(loc->el, nextLoc->left) : emplace(loc->el, nextLoc->right);
			else if (leftFree)
				emplace(loc->el, nextLoc->left);
			else if (rightFree)
				emplace(loc->el, nextLoc->right);
			else
				return 0;
	}
	return 1;
}

int simulateGas(locale* loc, locale* nextLoc) {
	if ((!hasProperties(loc->top,     PR_EXISTS) || hasProperties(loc->top,     PR_FLUID) || hasProperties(loc->top,     PR_GAS)) &&
		(!hasProperties(nextLoc->top, PR_EXISTS) || hasProperties(nextLoc->top, PR_FLUID) || hasProperties(nextLoc->top, PR_GAS))) {
		emplace(loc->el,  nextLoc->top);
		emplace(loc->top, nextLoc->el);
		}
	else {
		int
			leftFree  = (!hasProperties(loc->left,     PR_EXISTS) || hasProperties(loc->left,     PR_FLUID) || hasProperties(loc->left,     PR_GAS)) &&
						(!hasProperties(nextLoc->left, PR_EXISTS) || hasProperties(nextLoc->left, PR_FLUID) || hasProperties(nextLoc->left, PR_GAS)),
			rightFree = (!hasProperties(loc->right,     PR_EXISTS) || hasProperties(loc->right,     PR_FLUID) || hasProperties(loc->right,     PR_GAS)) &&
						(!hasProperties(nextLoc->right, PR_EXISTS) || hasProperties(nextLoc->right, PR_FLUID) || hasProperties(nextLoc->right, PR_GAS));
			if (leftFree && rightFree)
				srand() & 1 ? emplace(loc->el, nextLoc->left) : emplace(loc->el, nextLoc->right);
			else if (leftFree)
				emplace(loc->el, nextLoc->left);
			else if (rightFree)
				emplace(loc->el, nextLoc->right);
			else
				return 0;
	}
	return 1;
}

int simulateMomentary(locale* loc, locale* nextLoc) {
	if (randomFloat() > 0.25f)
		emplace(loc->el, nextLoc->el);
	return 1;
}

int simulateHot(locale* loc, locale* nextLoc) {
	localeArray lar = {*loc}, nextLar = {*nextLoc};
	int i = 0, j = 0;
	for (; i < 3; ++i) {
		for (; j < 3; ++j) {
			if (i != 1 && j != 1 && hasProperties(lar.arr[i][j], PR_FLAMMABLE) && hasProperties(nextLar.arr[i][j], PR_FLAMMABLE))
				set(nextLar.arr[i][j], EL_FIRE);
		}
	}
	return 0;
}

int simulatePlant(locale* loc, locale* nextLoc) {
	localeArray lar = {*loc}, nextLar = {*nextLoc};
	int i = 0, j = 0;
	for (; i < 3; ++i) {
		for (; j < 3; ++j) {
			if (i != 1 && j != 1 && *lar.arr[i][j] == EL_WATER && *nextLar.arr[i][j] == EL_WATER)
				set(nextLar.arr[i][j], EL_PLANT);
		}
	}
	return 0;
}

int simulateVaporizes(locale* loc, locale* nextLoc) {
	if (hasProperties(loc->bot, PR_HOT)) {
		set(nextLoc->el, EL_STEAM);
		return 1;
	}
	return 0;
}

int simulateCondenses(locale* loc, locale* nextLoc) {
	if (randomFloat() > 0.99) {
		set(nextLoc->el, EL_WATER);
		return 1;
	}
	return 0;
}

simulateElement(locale* loc, locale* nextLoc) {
	if (hasProperties(loc->el, PR_EXISTS)) {
		int moved = 0;

		if (hasProperties(loc->el, PR_SPOUT))
			moved |= simulateSpout(loc, nextLoc);
		else {
			if (hasProperties(loc->el, PR_FLUID))
				moved |= simulateFluid(loc, nextLoc);
			if (hasProperties(loc->el, PR_GAS))
				moved |= simulateGas(loc, nextLoc);
			if (hasProperties(loc->el, PR_HOT))
				moved |= simulateHot(loc, nextLoc);
			if (hasProperties(loc->el, PR_MOMENTARY))
				moved |= simulateMomentary(loc, nextLoc);
			if (hasProperties(loc->el, PR_PLANT))
				moved |= simulatePlant(loc, nextLoc);
			if (hasProperties(loc->el, PR_VAPORIZES))
				moved |= simulateVaporizes(loc, nextLoc);
			if (hasProperties(loc->el, PR_CONDENSES))
				moved |= simulateCondenses(loc, nextLoc);
		}

		if (!moved)
			emplace(loc->el, nextLoc->el);
	}
}

locale* createLocale(element** arr, int x, int y) {
	locale temp = (locale) {
		&arr[x - 1][y - 1], &arr[x   ][y - 1], &arr[x + 1][y - 1],
		&arr[x - 1][y    ], &arr[x   ][y    ], &arr[x + 1][y    ],
		&arr[x - 1][y + 1], &arr[x   ][y + 1], &arr[x + 1][y + 1],
	};

	locale* loc = (locale*) malloc(sizeof(locale));
	memcpy(loc, &temp, sizeof(locale));
	return loc;
}

destroyLocale(locale* loc) { free(loc); }

uint32_t elementBaseColor(element base) {
	switch(base) {
		case EL_AIR  : return 0x1A1A1A;
		case EL_WALL : return 0xAAAAAA;
		case EL_WATER: return 0x0000FF;
		case EL_STEAM: return 0xCCFFFF;
		case EL_SAND : return 0xFFFFCC;
		case EL_OIL  : return 0x996633;
		case EL_FIRE : return 0xFF0000;
		case EL_PLANT: return 0x33cc33;
		case EL_LAVA : return 0xCC0000;
		default: return 0xFF69B4;
	}
}

uint32_t elementColor(element* el) {
	element base = *el;
	removeProperties(&base, PR_SPOUT);

	uint32_t color = elementBaseColor(base);

	if (hasProperties(el, PR_SPOUT))
		brightenColor(&color, 2);

	return color;
}

uint32_t* sim2gfx(simulation* sim) {
	uint32_t* pixels = (uint32_t*) malloc(sim->width * sim->height * sizeof(uint32_t));

	int i = 0;
	for (; i <= sim->width; ++i) {
		int j = 0;
		for (; j <= sim->height; ++j) {
			int offset = (i * sim->width) + j;
			pixels[offset] = elementColor(&sim->arr[j + 1][i + 1]);
		}
	}
	return pixels;
}
*/
