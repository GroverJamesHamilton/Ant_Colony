#include "restFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0) {
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0) {
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

bool inBounds(int xpos, int ypos, int radius, int windowWidth, int windowHeight) {

	if (xpos + radius < windowWidth && xpos - radius > 0 && ypos + radius < windowHeight && ypos - radius > 0) {
		return true;
}
	else
	{
		return false;
	}

}
bool collision(vector<vector<int>> positions, vector<int> currPos, int radius) {
	if (positions.size() == 0) { return false; }
	else
	{
		int xcurr = currPos.at(0);
		int ycurr = currPos.at(1);
		int xval, yval;
		int distance;
		int minDistance = 100000;
		bool allchecked = false;
	for (int i = 0; i < positions.size(); i++) {
		if (i == positions.size() - 1) { allchecked = true; }
		xval = positions.at(i).at(0);
		yval = positions.at(i).at(1);
		distance = sqrt(pow(xcurr - xval, 2) + pow(ycurr - yval, 2));

		if (distance < 2*radius) {
			return true;
			break;
		}
	}
	if (allchecked)
	{
		return false;
	}
	}
}