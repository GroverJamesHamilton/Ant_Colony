#pragma once
#include <string>
#include <SDL.h>
#include <vector>

using namespace std;

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);
bool inBounds(int xpos, int ypos, int radius, int windowHeight, int windowWidth);
bool collision(vector<vector<int>> positions, vector<int> currPos, int radius);
int getMaxRadius(int xPos, int yPos, vector<vector<int>> positions, int pRadiusint, int windowWidth, int windowHeight);
vector<vector<int>> combList(int N, int K);
bool lineCircleCollision(double x1, double y1, double x2, double y2, double xc, double yc, double circRadius);
bool lineCirclesCollision(int x1, int y1, int x2, int y2, vector<vector<int>> positions);
int getClosestNode(double xcur, double ycur, vector<vector<int>> nodeLocations);
bool nodeConnectionExists(vector<int> curNode, vector<vector<int>> nodes);
bool insideNode(double xcur, double ycur, vector<vector<int>> nodeLocations);
double distance(double x1, double y1, double x2, double y2);