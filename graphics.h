#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <stdint.h>
#include "util.h"

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

typedef struct graphics {
	SDL_Window*   window;
	SDL_Renderer* renderer;
} graphics;

typedef SDL_Texture texture;

graphics* createGraphics(char* name, int width, int height);
destroyGraphics(graphics*);
updateGraphics(graphics* gfx);

texture* createStaticTexture(graphics* gfx, int width, int height);
texture* createStreamingTexture(graphics* gfx, int width, int height);
updateTexture(texture* tex, const void* pixels, int width);
renderTexture(graphics* gfx, texture* tex, rect* loc);

#endif // GRAPHICS_H_INCLUDED
