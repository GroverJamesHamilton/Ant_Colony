#pragma once
#include <string>
#include <SDL.h>

using namespace std;

class Window
{
public:

	Window(const string& title, int width, int height);
	~Window();

	void pollEvents();
	inline bool isClosed() { return closed; };

private:
	bool init();

private:
	string title = "Antcolony";
	int width = 800;
	int height = 600;

	bool closed = false;

	SDL_Window* window = nullptr;
};