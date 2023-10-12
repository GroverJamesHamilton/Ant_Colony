#include "restFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

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

bool inBounds(int xpos, int ypos, int radius, int windowWidth, int windowHeight)
{
	if (xpos + radius < windowWidth && xpos - radius > 0 && ypos + radius < windowHeight && ypos - radius > 0) 
	{
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

int getMaxRadius(int xPos, int yPos, vector<vector<int>> positions, int pRadius, int windowWidth, int windowHeight) {
	if (positions.size() == 0) { return 0; }
	else {
		int xval, yval;
		int distance = 0;
		int minDistance = 100000;
		for (int i = 0; i < positions.size(); i++) {
			xval = positions.at(i).at(0);
			yval = positions.at(i).at(1);
			distance = sqrt(pow(xPos - xval, 2) + pow(yPos - yval, 2));
			if (distance < minDistance) { minDistance = distance; }
		}
		if (minDistance > 3*pRadius) {
			return minDistance - 1.5 * pRadius;
		}
		else { return 0; }
	}
}

vector<vector<int>> combList(int N, int K)
{
	std::string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's
	vector<vector<int>> allComb;
	// print integers and permute bitmask
	do {
		vector <int> tmp;
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) tmp.push_back(i);
		}
		allComb.push_back(tmp);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
	return allComb;
}

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

bool insideNode(double xcur, double ycur, vector<vector<int>> nodeLocations) 
{
	double xnode, ynode, distance;
	double rad = nodeLocations.at(0).at(2);
	bool inside = false;
	for (int i = 0; i < nodeLocations.size(); ++i) 
	{
		xnode = nodeLocations.at(i).at(0);
		ynode = nodeLocations.at(i).at(1);
		distance = sqrt(pow((xnode - xcur), 2) + pow((ynode - ycur), 2));
		if (distance < rad)
		{
			inside = true;
			break;
		}
	}
	return inside;
}

bool nodeConnectionExists(vector<int> curNode, vector<vector<int>> nodes) {
	bool exists = false;
	if (nodes.size() > 0) 
	{
		for (int i = 0; i < nodes.size(); ++i)
		{
			if (curNode == nodes.at(i))
			{
				exists = true;
			}
		}
	}
	else { exists = false; }
	return exists;
}

double distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}






bool lineCircleCollision(double x1, double y1, double x2, double y2, double xc, double yc, double circRadius) {
	bool collision;
	if (x1 != x2) {

		double k = (y2 - y1) / (x2 - x1);
		double m = y1 - k * x1;
		double dist = abs(k*xc - yc + m) / sqrt(pow(k,2) + 1);

		if (dist < circRadius) {
			collision = true;
		}
		else { collision = false; }
	}
	else { collision = false; }
	
	return collision;
}

bool lineCirclesCollision(int x1, int y1, int x2, int y2, vector<vector<int>> positions) {
	int xc, yc, radc;
	bool collision = false;
	for (int i = 0; i < positions.size(); ++i) {
		xc = positions.at(i).at(0);
		yc = positions.at(i).at(1);
		radc = positions.at(i).at(2);
		collision = lineCircleCollision(x1, y1, x2, y2, xc, yc, radc);
		if (collision) {
			return true;
			break;
		}
	}
	return false;
}