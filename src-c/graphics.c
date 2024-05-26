#include <stdio.h>
#include "graphics.h"
#include "defs.h"

static SDL_Window* window = NULL; // static used to establish scope and allow for persistence
static SDL_Renderer* renderer = NULL;
static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

bool initializeWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL...\n");
		return false;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	int fullScreenWidth = displayMode.w; // used to match window size to screen resolution
	int fullScreenHeight = displayMode.h;
	window = SDL_CreateWindow(
		NULL, // title
		SDL_WINDOWPOS_CENTERED, // x and y coordinates of window placement
		SDL_WINDOWPOS_CENTERED,
		fullScreenWidth,
		fullScreenHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window...\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0); // use default graphics driver
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer...\n");
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
    // allocate memory for the color buffer
    colorBuffer = (color_t*)malloc(sizeof(color_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    
	// create the SDL texture to display the color buffer
	colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
    return true;
}

void drawPixel(int x, int y, color_t color) {
    colorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

void drawRect(int x, int y, int width, int height, color_t color) {
    for (int i = x; i <= (x + width); i++) {
        for (int j = y; j <= (y + height); j++) {
            drawPixel(i, j, color);
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, color_t color) { // DDA line algorithm
	int deltaX = (x1 - x0);
	int deltaY = (y1 - y0);

	int longestSideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

	float xIncrement = deltaX / (float)longestSideLength;
	float yIncrement = deltaY / (float)longestSideLength;

	float currentX = x0;
	float currentY = y0;

	for (int i = 0; i < longestSideLength; i++) {
		drawPixel(round(currentX), round(currentY), color);
		currentX += xIncrement;
		currentY += yIncrement;
	}
}

void clearColorBuffer(color_t color) {
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
		colorBuffer[i] = color;
	}
}

void renderColorBuffer() {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        (int)(WINDOW_WIDTH * sizeof(color_t))
    );
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void destroyWindow() {
    free(colorBuffer);
    SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}