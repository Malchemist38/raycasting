#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

bool mapHasWallAt(float x, float y);
bool isInsideMap(float x, float y);
int getMapAt(int i, int j);
void renderMapGrid(void);

#endif