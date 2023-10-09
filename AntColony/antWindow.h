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

	void pollEvents();
	inline bool isClosed() { return closed; };
	void RenderFrame();
	void Update();
	void Draw();

private:
	bool init();

private:
	string title = "Antcolony";
	int width = 800;
	int height = 600;
	SDL_Surface* SDLWindowSurface = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool closed = false;
	int pointRadius = 10;
	vector<vector<int>> pointPositions;
	vector<int> tempPos = {0,0};
	int x, y;
	int obstRadius;
};