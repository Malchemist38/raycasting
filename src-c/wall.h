#ifndef WALL_H
#define WALL_H

#include <math.h>
#include "defs.h"
#include "ray.h"
#include "player.h"
#include "textures.h"
#include "graphics.h"

void changeColorIntensity(color_t* color, float factor);
void renderWallProjection(void);

#endif