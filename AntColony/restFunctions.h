#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


using namespace std;

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);
bool inBounds(int xpos, int ypos, int radius, int windowHeight, int windowWidth);
bool collision(vector<vector<int>> positions, vector<int> currPos, int radius);
int getClosestNode(double xcur, double ycur, vector<vector<int>> nodeLocations);
bool insideNode(double xcur, double ycur, vector<vector<int>> nodeLocations);
double distance(double x1, double y1, double x2, double y2);
double getAngle(int x1, int y1, int x2, int y2);
bool isDigits(const std::string& str);
bool isSingleDigit(const std::string& str);
bool isDecimal(const std::string& str);
