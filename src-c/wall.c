#include "wall.h"

// create a dimming/shadow effect
void changeColorIntensity(color_t* color, float factor) {
	color_t a = (*color & 0xFF000000); // bitwise masking
	color_t r = (*color & 0x00FF0000) * factor;
	color_t g = (*color & 0x0000FF00) * factor;
	color_t b = (*color & 0x000000FF) * factor;

	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);	
}

void renderWallProjection() {
	for (int x = 0; x < NUM_RAYS; x++) {
		// calculate perpendicular distance to avoid fish-eye distortion
		float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle); // calculate perpendicular distance to fix fishbowl/fisheye effect
		
		// height of the 3D projected wall
		float wallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
		
		// top y value of the wall (highest pixel)
		int wallTopY = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
		wallTopY = wallTopY < 0 ? 0 : wallTopY; // set the top of the screen as the lower limit for wall height
		
		// bottom y value of the wall (lowest pixel)
		int wallBottomY = (WINDOW_HEIGHT / 2) + (wallHeight / 2); // center around the middle of the screen
		wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY; // set the bottom of the screen as the upper limit
		
		// set the color of the ceiling
		for (int y = 0; y < wallTopY; y++) {
			drawPixel(x, y, 0xFF444444);
		}

		// draw the textured wall
		int textureOffsetX;
		if (rays[x].wasHitVertical) {
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		}
		else {
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		}

		// get the texture ID number from the map content
		int texNum = rays[x].texture - 1;

		// query the texture width and height from the upng
		int textureWidth = upng_get_width(textures[texNum]);
		int textureHeight = upng_get_height(textures[texNum]);
		
		// render the wall from wallTopY to wallBottomY
		for (int y = wallTopY; y < wallBottomY; y++) {
			int distanceFromTop = y + (wallHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)textureHeight / wallHeight);
			
			// set the color of the wall based on the color from the texture that we have in memory
			color_t* wallTextureBuffer = (color_t*) upng_get_buffer(textures[texNum]);
			color_t texelColor = wallTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX]; // texel = texture pixel
			
			// make the pixel color darker if the ray hit was vertical
			if (rays[x].wasHitVertical) {
				changeColorIntensity(&texelColor, 0.7);
			}
			drawPixel(x, y, texelColor);
		}

		// set the color of the floor
		for (int y = wallBottomY; y < WINDOW_HEIGHT; y++) {
			drawPixel(x, y, 0xFF888888);
		}
	}
}