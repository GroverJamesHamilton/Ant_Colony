#include <iostream>
#include <SDL.h> 
#include "antWindow.h"
#include "restFunctions.h"

int main(int argc, char **argv)
{
	Window window("Antwindow", 800, 600);
	while (!window.isClosed()) {
		window.pollEvents();
	}
	return 0;
}