#include <stdio.h>
#include <SDL.h>
#include "graphics.h"
#include "simulation.h"
#include "button.h"

const int SCREEN_WIDTH  = 760;
const int SCREEN_HEIGHT = 720;

const int SIM_WIDTH  = 400;
const int SIM_HEIGHT = 400;

graphics*   gfx = NULL;
simulation* sim = NULL;
texture* simTexture = NULL;

rect* simLoc = NULL;
button* buttons;
int nButtons = 14;

main (int argc, char* args[]) {
	printSplash();

	init();
	initGui();

	while (1) {
		render();
		input();
		simulate(sim);
	}

	free(simLoc);
	free(buttons);

	destroySimulation(sim);
	destroyGraphics(gfx);

	return 0;
}

printSplash() {
	printf("=========================================\n");
	printf("Cand: the falling sand game written in C!\n");
	printf("starting.................................\n");
	printf("=========================================\n");
}

init() {
	gfx = createGraphics("Cand", SCREEN_WIDTH, SCREEN_HEIGHT);
	sim = createSimulation(SIM_WIDTH, SIM_HEIGHT);
	simTexture = createStreamingTexture(gfx, SIM_WIDTH, SIM_HEIGHT);
}

const element elementList[] = {
		EL_WALL,
		EL_WATER,
		EL_STEAM,
		EL_SAND,
		EL_OIL,
		EL_FIRE,
		EL_PLANT,
		EL_LAVA,
		EL_WATER_SPOUT,
		EL_STEAM_SPOUT,
		EL_SAND_SPOUT,
		EL_OIL_SPOUT,
		EL_FIRE_SPOUT,
		EL_LAVA_SPOUT
};

initGui() {
	simLoc = (rect*) malloc(sizeof(rect));
	rect simLocVal = {0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT};
	memcpy(simLoc, &simLocVal, sizeof(rect));

	buttons = (button*) calloc(nButtons, sizeof(button));
	int i = 0;
	for (; i < nButtons; ++i) {
		rect bounds = {720, i * 40, 40, 40};
		button b = {elementList[i], bounds};
		buttons[i] = b;
	}
}

render() {
	uint32_t* pixels = sim2gfx(sim);
	updateTexture(simTexture, pixels, SIM_WIDTH);
	free(pixels);

	renderTexture(gfx, simTexture, simLoc);

	int i = 0;
	for (; i < nButtons; ++i)
		renderButton(gfx, &buttons[i]);

	updateGraphics(gfx);
}

int buttonHeld = 0;
element chosenElement = EL_WALL;

int input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (buttonHeld || event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (rectContains(*simLoc, event.button.x, event.button.y)) {
					float
						x = event.button.x * SIM_WIDTH  / simLoc->width,
						y = event.button.y * SIM_HEIGHT / simLoc->height;
					paintElement(sim, chosenElement, x, y, 16);
				} else {
					int i = 0;
					for (; i < nButtons; ++i)
						if (overlapButton(&buttons[i], event.button.x, event.button.y)) {
							chosenElement = buttons[i].el;
							break;
						}
				}
			} else {
				if (rectContains(*simLoc, event.button.x, event.button.y)) {
					float
						x = event.button.x * SIM_WIDTH  / simLoc->width,
						y = event.button.y * SIM_HEIGHT / simLoc->height;
					paintElement(sim, EL_AIR, x, y, 16);
				}
			}
			buttonHeld = 1;
		}

		if (event.type == SDL_MOUSEBUTTONUP) {
			buttonHeld = 0;
		}
	}
}
