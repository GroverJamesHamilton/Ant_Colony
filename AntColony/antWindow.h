#pragma once
#include <string>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <utility>
#include <fstream>

using namespace std;

class Window
{
public:
	//All the screen functions to draw the node map and plot of the
	//minimum distance found in each iteration of the ant colony optimization
	Window(const string& title, int width, int height);		//Window constructor
	~Window();												//Window destructor
	void pollEvents();										//Handling what to do when using the mouse clicker
	inline bool isClosed() { return closed; };				//Is used to terminate the program in main if the window is closed								
	void DrawNode(int nodeNum, int radius, int x, int y);	//Draws a node that every ant should visit
	void highlight(int radius, int x, int y);				//Highlighting a node, a remnant from my previous program
	void unhighlight(int radius, int x, int y);				//Same as above
	vector<int> DrawEmptyPlot();							//Draws and empty plot with an x- and y-axis
	vector<double> loadParams();							//Loads the ant colony optimization parameters
	void connect2Nodes(vector<int> node1, vector<int> node2, int radius);//Draws a line between 2 nodes
	void connectAllNodes(vector<vector<int>> nodePos, vector<int> path, int radius, string drawMode);//Uses the help function above to draw all current connection
	void eraseMap();										//Erases the nodemap so it can be redrawn, all nodepositions are saved until the program is executed
	void redrawNodes(vector<vector<int>> nodePos, int radius);//Redraws nodes
	void dispParams();										//Display all ant parameters
	void changeParam();										//Change the desired parameter
	void drawPlot(vector<double> plotDistances, vector<int> minDistancesIndex);			//Plots the progression of the minimum distance for each iteration

private:
	bool init(); //Initialize window surface, font etc

private:
	//Window paramaters
	string title = "Antcolony";
	int width, height; //Determined in main
	SDL_Surface* SDLWindowSurface = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;
	bool closed = false;
	double plotRatio = 0.3; //The ratio of how much the window is occupied by the plot
	double dr = 0.04;		//Distance ratio to make sure the plot axes have some distance from the window edges
	bool plot = true;

	//Node parameters
	SDL_Surface* nodeNum;
	int nodeRad = 15; //Draw radius
	vector<vector<int>> nodePos;
	vector<int> tempPos = { 0,0,0 };
	vector<int> bestPath;
	int x, y;
	bool highlighted = false; //If any node is highlighted, i.e. different color
	int lastHighlightedNodeIndex = 999;
	int nearestNodeIndex;

	//Plot variables handler
	bool paramsLoaded = false;
	vector<double> optParams;
	int nrAnts, nrIterations;
	double evaporationRate, initUpperPheromone, initLowerPheromone, alpha, beta, pheromoneUpdateRate;

	//Used to classify the parameters when displaying
	vector<string> paramNames =
	{
		"Number of ants ",
		"Pheromone evaporation rate ",
		"Number of iterations ",
		"Initial pheromone level (Upper limit) ",
		"Initial pheromone level (Lower limit) ",
		"Alpha ",
		"Beta ",
		"Pheromone update rate "
	};
};