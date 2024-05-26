#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defs.h"
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

bool initializeWindow(void);
void drawPixel(int x, int y, color_t color);
void drawRect(int x, int y, int width, int height, color_t color);
void drawLine(int x0, int y0, int x1, int y1, color_t color);
void clearColorBuffer(color_t color);
void renderColorBuffer(void);
void destroyWindow(void);

#endif