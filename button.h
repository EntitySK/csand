#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "simulation.h"
#include "util.h"
#include "graphics.h"

typedef struct button {
	element el;
	rect bounds;
} button;

int overlapButton(button* b, int x, int y);

renderButton(graphics* gfx, button* b);

#endif // BUTTON_H_INCLUDED
