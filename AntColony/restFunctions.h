#pragma once
#include <string>
#include <SDL.h>
#include <vector>

using namespace std;

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);
bool inBounds(int xpos, int ypos, int radius, int windowHeight, int windowWidth);
bool collision(vector<vector<int>> positions, vector<int> currPos, int radius);