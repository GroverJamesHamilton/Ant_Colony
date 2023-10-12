#pragma once
#include <string>
#include <SDL.h>
#include <vector>

using namespace std;

class Window
{
public:

	Window(const string& title, int width, int height);
	~Window();
	void resetNode();
	void pollEvents();
	inline bool isClosed() { return closed; };
	void RenderFrame();
	void Update();
	void Draw();

private:
	bool init();

private:
	string title = "Antcolony";
	int width = 1000;
	int height = 800;
	SDL_Surface* SDLWindowSurface = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool closed = false;
	int nodeRad = 15;
	vector<vector<int>> nodePos;
	vector<vector<int>> obstPositions;
	vector<int> nodeConnection = { 0,0 };
	vector<vector<int>> nodeConnections;
	vector<int> tempPos = { 0,0,0 };
	vector<int> tempObst = { 0,0,0 };
	double connectionWeights[100][100];
	int x, y;
	int obstRadius;
	bool highlighted = false;
	int lastHighlightedNodeIndex = 999;
	int nearestNodeIndex;
};