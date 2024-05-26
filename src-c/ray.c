#include "ray.h"
#include "player.h"
#include "utils.h"
#include <float.h> // for FLT_MAX constant

ray_t rays[NUM_RAYS];

void castRay(float rayAngle, int stripID) {
	normalizeAngle(&rayAngle);
	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = rayAngle < (0.5 * PI) || rayAngle > (1.5 * PI);
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	///////////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION CODE //
	///////////////////////////////////////////

	bool foundHorzWallHit = false;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallTexture = 0;

	// find the y-coordinate of the closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// find the x-coordinate of the closest horizontal grid intersection
	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// calculate the increment of the xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	// increment xstep and ystep until we find a wall
	while (isInsideMap(nextHorzTouchX, nextHorzTouchY)) {
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
		
		if (mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY; // record current x and y positions
			foundHorzWallHit = true;
			horzWallTexture = getMapAt((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			break;
		}
		else {
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}

	///////////////////////////////////////////
	//  VERTICAL RAY-GRID INTERSECTION CODE  //
	///////////////////////////////////////////

	bool foundVertWallHit = false;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallTexture = 0;

	// find the x-coordinate of the closest horizontal grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// find the y-coordinate of the closest horizontal grid intersection
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	// calculate the increment of the xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	// increment xstep and ystep until we find a wall
	while (isInsideMap(nextVertTouchX, nextVertTouchY)) {
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (mapHasWallAt(xToCheck, yToCheck)) {
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY; // record current x and y positions
			foundVertWallHit = true;
			vertWallTexture = getMapAt((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			break;
		}
		else {
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}
	
	// calculate horizontal and vertical hit distances and choose the smallest value
	float horzHitDistance = foundHorzWallHit
		? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
		: FLT_MAX;
	float vertHitDistance = foundVertWallHit
		? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
		: FLT_MAX;
	if (vertHitDistance < horzHitDistance) {
		rays[stripID].distance = vertHitDistance;
		rays[stripID].wallHitX = vertWallHitX;
		rays[stripID].wallHitY = vertWallHitY;
		rays[stripID].texture = vertWallTexture;
		rays[stripID].wasHitVertical = true;
	}
	else {
		rays[stripID].distance = horzHitDistance;
		rays[stripID].wallHitX = horzWallHitX;
		rays[stripID].wallHitY = horzWallHitY;
		rays[stripID].texture = horzWallTexture;
		rays[stripID].wasHitVertical = false;
	}
	rays[stripID].rayAngle = rayAngle;
}

void castAllRays() {
	// iterate through all rays
	for (int col = 0; col < NUM_RAYS; col++) {
		float rayAngle = player.rotationAngle + atan((col - NUM_RAYS / 2) / DIST_PROJ_PLANE);
		castRay(rayAngle, col);
	}
}

void renderMapRays() {
	for (int i = 0; i < NUM_RAYS; i++) {
		drawLine(
			player.x * MINIMAP_SCALE_FACTOR,
			player.y * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitX * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitY * MINIMAP_SCALE_FACTOR,
            0xFF0000FF
        );
	}
}