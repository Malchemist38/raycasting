#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include <stdint.h> // for fixed size integer data types
#include <stdbool.h>
#include "textures.h" // prototype images copyright by id software
#include "defs.h"
#include "graphics.h"
#include "map.h"
#include "ray.h"
#include "player.h"
#include "wall.h"
#include "sprite.h"

bool isGameRunning = false;
int ticksLastFrame = 0;

void setup() {
	// have uPNG library decode all PNG files and loads the wallTextures array
	loadTextures();
}

void processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT: {
			isGameRunning = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isGameRunning = false;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				player.walkDirection = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				player.walkDirection = -1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				player.turnDirection = 1;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				player.turnDirection = -1;
			}
			break;
		}
		case SDL_KEYUP: {
			if (event.key.keysym.sym == SDLK_UP) {
				player.walkDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				player.walkDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				player.turnDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				player.turnDirection = 0;
			}
			break;
		}
	}
}

void update() {
	// offset enought time to reach target frame time length (fps)
	int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

	// delay execution if we are running too fast
	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
		SDL_Delay(timeToWait);
	}
	// number of milliseconds in current frame minus number of milliseconds in the last frame converted to seconds
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks(); // update for the next frame

	// update game objects as a function of deltaTime
	movePlayer(deltaTime);
	castAllRays();
}

void render() {
	clearColorBuffer(0xFF000000);
	renderWallProjection();
	renderSpriteProjection();

	// render the minimap objects
	renderMapGrid();
	renderMapRays();
	renderMapSprites();
	renderMapPlayer();

	renderColorBuffer();
}

void releaseResources() {
	freeTextures();
	destroyWindow();
}

int main(int argc, char* argv[]) {
	isGameRunning = initializeWindow();
	setup();
	while (isGameRunning) {
		processInput();
		update();
		render();
	}

	releaseResources();
	return EXIT_SUCCESS;
}