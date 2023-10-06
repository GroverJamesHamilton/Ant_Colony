#include "antWindow.h"
#include "restFunctions.h"
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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "Window renderer creation failure.\n";
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	return true;
}

void Window::RenderFrame() {
	SDL_UpdateWindowSurface(window);
}

void Window::Update() {
	SDL_FillRect(
		SDLWindowSurface,
		nullptr,
		SDL_MapRGB(SDLWindowSurface->format, 40, 40, 40)
	);
}

void Window::Draw() {

	if (renderer == nullptr) {
		std::cerr << "Window renderer creation failure.\n";
	}

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderDrawLine(renderer, 5, 5, 100, 120);
	SDL_RenderPresent(renderer);
}

void Window::pollEvents() {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			closed = true;
			break;
			//
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				int x, y;
				SDL_GetMouseState(&x, &y);
				cout << "Mouse click position: " << x << "," << y << endl;
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				DrawCircle(renderer, x, y, pointRadius);
				SDL_RenderPresent(renderer);
				break;
			case SDL_BUTTON_RIGHT:
				//SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", window);
				break;
			default:
				//SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", window);
				break;
			}
			break;
			//
		default:
			break;
		}
	}
}

