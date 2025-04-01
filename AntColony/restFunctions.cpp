#include "restFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cctype>

//Figure out whether a string is a decimal, not my original code
//From Stack Overflow
bool isDecimal(const string& str) {

	bool hasDecimalPoint = false;
	for (char c : str){

		if (c == '.' && !hasDecimalPoint) {

			hasDecimalPoint = true;
		}
		else if (!isdigit(c)) {

			return false;
		}
	}
	return hasDecimalPoint;
}

//Figure out whether a string is a collection of only digits, not my original code
bool isDigits(const std::string& str) {

	return all_of(str.begin(), str.end(), ::isdigit);
}

//Figure out whether a string is a single digit, not my original code
bool isSingleDigit(const std::string& str) {

	return str.length() == 1 && std::isdigit(str[0]);
}

//Draw circle in SDL, not my original code
void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {

	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y) {

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

//Checks if a position is in bounds of a circle of known radius
bool inBounds(int xpos, int ypos, int radius, int windowWidth, int windowHeight) {

	if (xpos + radius < windowWidth && xpos - radius > 0 && ypos + radius < windowHeight && ypos - radius > 0) {

		return true;
	}
	else {

		return false;
	}
}

//Checks if the node you want to place collides with another
bool collision(vector<vector<int>> positions, vector<int> currPos, int radius) {
	if (positions.size() == 0) { return false; }
	else {

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
	if (allchecked) {

		return false;
		}
	}
}

//Return the center of the closest node based on the mouse-clicked x- and y-coordinates
int getClosestNode(double xcur, double ycur, vector<vector<int>> nodeLocations) {
	double distance, xnode, ynode;
	double minDistance = 10000000;
	int minDistIndex;
	for (int i = 0; i < nodeLocations.size(); ++i) {

		xnode = nodeLocations.at(i).at(0);
		ynode = nodeLocations.at(i).at(1);
		distance = sqrt(pow((xcur - xnode), 2) + pow((ycur - ynode), 2));

		if (distance < minDistance) {
			minDistance = distance;
			minDistIndex = i;
		}

	}
	return minDistIndex;
}

//Checks if the mouse click is inside an existing node
bool insideNode(double xcur, double ycur, vector<vector<int>> nodeLocations) 
{
	double xnode, ynode, distance;
	double rad = nodeLocations.at(0).at(2);
	bool inside = false;
	for (int i = 0; i < nodeLocations.size(); ++i) {

		xnode = nodeLocations.at(i).at(0);
		ynode = nodeLocations.at(i).at(1);
		distance = sqrt(pow((xnode - xcur), 2) + pow((ynode - ycur), 2));
		if (distance < rad) {

			inside = true;
			break;
		}
	}
	return inside;
}

//Euclidian distance
double distance(double x1, double y1, double x2, double y2) {

	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

//Get angle in radians
double getAngle(int x1, int y1, int x2, int y2) {

	int deltaY = y2 - y1;
	int deltaX = x2 - x1;
	return atan2(deltaY, deltaX);
}