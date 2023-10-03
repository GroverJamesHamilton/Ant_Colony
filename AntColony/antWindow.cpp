#include "antWindow.h"

#include <iostream>

Window::Window(const string& title, int width, int height) :
	title(title), width(width), height(height)
{
	if (!init())
	{
		closed = true;
	}

}

Window::~Window() 
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cerr << "Failed to initialize SDL.\n";
		return 0;
	}

	window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN); //Not resizable

	if (window == nullptr) {
		std::cerr << "Window creation failure.\n";
	}

	return true;
}

void Window::pollEvents() {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			closed = true;
			break;
		default:
			break;
		}
	}
}