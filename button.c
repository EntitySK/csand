#include <SDL.h>
#include "button.h"

int overlapButton(button* b, int x, int y) {
	return rectContains(b->bounds, x, y);
}

uint32_t buttonColor(button* b) {
	return elementColor(&b->el);
}

renderButton(graphics* gfx, button* b) {
	rgbaColor color = {buttonColor(b)};

	SDL_SetRenderDrawColor(gfx->renderer,
							color.rgba[0],
							color.rgba[1],
							color.rgba[2],
							color.rgba[3]);
	SDL_RenderFillRect(gfx->renderer, &b->bounds);
}
