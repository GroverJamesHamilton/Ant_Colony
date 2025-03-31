#include <iostream>
#include <SDL.h> 
#include "antWindow.h"
#include "restFunctions.h"

constexpr int SCREEN_WIDTH = 1800;
constexpr int SCREEN_HEIGHT = 950;

int main(int argc, char **argv)
{

	cout << "Welcome to my Ant Colony Optimization project!" << endl;
	cout << "To test the algorithm, place nodes on the black area with the left mouse button 3-100" << endl;
	cout << "To change parameters, use the right mouse button on the window and set your inputs in the terminal" << endl;
	cout << "Quit changing parameters by typing in a non-digit" << endl;
	cout << "When ready, click your middle mouse button on the window to test your nodes and parameters" << endl;

	Window window("Antwindow", SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!window.isClosed()) 
	{
		window.pollEvents();
	}
	return 0;
}


