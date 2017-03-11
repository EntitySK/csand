#include <stdio.h>
#include <SDL.h>
#include "graphics.h"

graphics* createGraphics(char* name, int width, int height) {
	graphics* gfx = (graphics*) malloc(sizeof(graphics));

	printf("Initializing SDL 2...");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		printf("SDL 2 initialized!\n");
		SDL_version compiled, linked;
		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		printf("Compiled version: %d.%d.%d\n",compiled.major, compiled.minor, compiled.patch);
		printf("Linked version: %d.%d.%d\n", linked.major, linked.minor, linked.patch);

		printf("Creating window...");
		gfx->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (gfx->window == NULL) {
			printf("window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			printf("window created!\n");

			gfx->renderer = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderPresent(gfx->renderer);
		}
	}
	return gfx;
}

destroyGraphics(graphics* gfx) {
	SDL_DestroyRenderer(gfx->renderer);
	SDL_DestroyWindow(gfx->window);
	SDL_Quit();
	free(gfx);
}

updateGraphics(graphics* gfx) {
	SDL_RenderPresent(gfx->renderer);
}

texture* createTexture(graphics* gfx, int access, int width, int height) {
	return SDL_CreateTexture(gfx->renderer, SDL_PIXELFORMAT_ARGB8888, access, width, height);
}

texture* createStaticTexture(graphics* gfx, int width, int height) {
	return createTexture(gfx, SDL_TEXTUREACCESS_STATIC, width, height);
}

texture* createStreamingTexture(graphics* gfx, int width, int height) {
	return createTexture(gfx, SDL_TEXTUREACCESS_STREAMING, width, height);
}

updateTexture(texture* tex, const void* pixels, int width) {
	SDL_UpdateTexture(tex, NULL, pixels, width * 4);
}

renderTexture(graphics* gfx, texture* tex, rect* loc) {
	SDL_RenderCopy(gfx->renderer, tex, NULL, loc);
}
