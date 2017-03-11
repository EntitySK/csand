#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <stdint.h>

typedef enum {
	PR_EXISTS    = 1,
	PR_SPOUT     = 2,
	PR_FLUID     = 4,
	PR_GAS       = 8,
	PR_MOMENTARY = 16,
	PR_FLAMMABLE = 32,
	PR_HOT       = 64,
	PR_PLANT     = 128,
	PR_VAPORIZES = 256,
	PR_CONDENSES = 512,
} elementProperty;

typedef enum {
	EL_AIR         = 0,
	EL_WALL        = PR_EXISTS,
	EL_WATER       = PR_EXISTS | PR_FLUID | PR_VAPORIZES,
	EL_STEAM       = PR_EXISTS | PR_GAS   | PR_CONDENSES,
	EL_SAND        = PR_EXISTS | PR_FLUID,
	EL_OIL         = PR_EXISTS | PR_FLUID | PR_FLAMMABLE,
	EL_FIRE        = PR_EXISTS | PR_HOT   | PR_MOMENTARY,
	EL_PLANT       = PR_EXISTS | PR_PLANT | PR_FLAMMABLE,
	EL_LAVA        = PR_EXISTS | PR_FLUID | PR_HOT,
	EL_WATER_SPOUT = EL_WATER | PR_SPOUT,
	EL_STEAM_SPOUT = EL_STEAM | PR_SPOUT,
	EL_SAND_SPOUT  = EL_SAND  | PR_SPOUT,
	EL_OIL_SPOUT   = EL_OIL   | PR_SPOUT,
	EL_FIRE_SPOUT  = EL_FIRE  | PR_SPOUT,
	EL_LAVA_SPOUT  = EL_LAVA  | PR_SPOUT,
	} element;

typedef struct simulation {
	int width, height;
	element** arr;
} simulation;

typedef struct locale {
	element
	*top_left, *top, *top_right,
	*left    , *el , *right,
	*bot_left, *bot, *bot_right;
} locale;

typedef union {
	locale    loc;
	element** arr[3][3];
} localeArray;

locale* createLocale(element** sim, int x, int y);
destroyLocale(locale* loc);

simulation* createSimulation(int width, int height);
destroySimulation(simulation*);
paintElement(simulation* sim, element el, int x, int y, int radius);
simulate(simulation*);
uint32_t* sim2gfx(simulation* sim);

#endif // SIMULATION_H_INCLUDED
