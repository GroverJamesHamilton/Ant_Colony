#include "antWindow.h"
#include "restFunctions.h"
#include "Algorithm.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <tuple>
#include <cmath>

//Window constructor
Window::Window(const string& title, int width, int height):
			   title(title), width(width), height(height) {

	if (!init()) {

		closed = true;
	}
}

//Window destructor
Window::~Window() {

	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
}

//Draws an empty plot, i.e. draws a white square and 2 orthogonal axes in the lower window area
vector<int> Window::DrawEmptyPlot() {

	const int plotHeight = static_cast<int>(plotRatio * height);
	const int plotY = static_cast<int>((1 - plotRatio) * height);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = plotY;
	rect.w = width;
	rect.h = plotHeight;
	//dr is used to put some distance between the plot axes and the window edges
	int origox = dr * width;
	int origoy = (1 - dr * plotRatio) * height;
	int plotw = width - 2 * origox;
	int ploth = plotHeight - 2 * (height - origoy);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//Draw vertical axis
	SDL_RenderDrawLine(renderer, origox, (1 + plotRatio * (dr - 1)) * height, origox, origoy);
	SDL_RenderDrawLine(renderer, origox - 1, (1 + plotRatio * (dr - 1)) * height, origox - 1, origoy);
	//Draw horizontal axis
	SDL_RenderDrawLine(renderer, origox, (1 - dr * plotRatio) * height, (1 - dr) * width, origoy);
	SDL_RenderDrawLine(renderer, origox, (1 - dr * plotRatio) * height + 1, (1 - dr) * width, origoy + 1);
	SDL_RenderPresent(renderer);
	//Values can be returned returned in order to plot the values within the designated axes
	vector<int> plotDims = { origox, origoy, plotw, ploth };

	return plotDims;
}

//Plots on the window (line-plot), intended to plot the current vector
//of the minimum distance per iteration
void Window::drawPlot(vector<double> dist, vector<int> minDistIdx) {

	int xRect, yRect, wRect, hRect, plotwidth;
	double scale;
	vector<int> drawrect = DrawEmptyPlot(); //Redraws the plot to be empty and obtains the coordinates to plot within
	xRect = drawrect.at(0);					//Plot origo-x
	yRect = drawrect.at(1);					//Plot origo-y
	wRect = drawrect.at(2);					//Plot length limit
	hRect = drawrect.at(3);					//Plot height limit

	plotwidth = wRect / dist.size();		//The pixel width between values is adapted based on vector size
	scale = hRect / maxDistance(dist);		//Scales the height of the plot
	//Plots all values within the designated plot space
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

	for (int j = 0; j < minDistIdx.size(); j++) {

		 DrawCircle(
			renderer,
			xRect + round(j * plotwidth),
			yRect + round(scale * minDistIdx.at(j)),
			5);

	}

	for (int i = 0; i < dist.size() - 1; i++) {

		SDL_RenderDrawLine(
			renderer,
			xRect + round(i * plotwidth),
			yRect - round(scale * dist.at(i)),
			xRect + round((i + 1) * plotwidth),
			yRect - round(scale * dist.at(i + 1)));
	}
	SDL_RenderPresent(renderer);
}

//Erases nodemap, i.e. draws a black square of the top area
void Window::eraseMap() {

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = static_cast<int>((1 - plotRatio) * height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}

//Load parameters from txt file
vector<double> Window::loadParams() {

	string line;
	vector<double> parameters;
	double parameter;
	ifstream params("params.txt");
	while (getline(params, line)) {

		parameter = stod(line);
		parameters.push_back(parameter);
	}

	return parameters;
}

//Displays paramaters from the text file
void Window::dispParams() {

	string param;
	ifstream params("params.txt");
	cout << "The optimization parameters: " << endl;
	for (int i = 0; i < paramNames.size(); i++) {

		getline(params, param);
		//Displays the parameter name declared in header file and value from txt file
		cout << i+1 << ":" << paramNames.at(i) << ": " << param << endl;
	}
}

//Change parameter by choice in the text file,
//this makes the same nodemap can be tested with different parameter values
void Window::changeParam() {

	string param;
	vector<string> parameters;
	ifstream params("params.txt");
	int changeNumber;
	while (getline(params, param)) {

		parameters.push_back(param);
	}
	while (1) {
		//Instruct the user what to type in
		string paramNumber, newValue;
		cout << "What parameter (1-8) do you want to change? (Check names above)" << endl;
		cout << "Type in anything else to not change anything." << endl;
		cin >> paramNumber;
		if (!isSingleDigit(paramNumber)) {

			cout << "No changes made. Right-click to change parameters again or commence testing your algorithm" << endl;
			break;
		}
		changeNumber = stoi(paramNumber);
		if (changeNumber >= 1 && changeNumber <= 8) {

			cout << "Change " << paramNames.at(changeNumber - 1) << " to: " << endl;
			cin >> newValue;
			//Makes sure only digits and decimals are used
			if (isDecimal(newValue) || isDigits(newValue) || isSingleDigit(newValue)) {

				parameters.at(changeNumber - 1) = newValue;
				ofstream params("params.txt", ofstream::out | ofstream::trunc);
				if (params.is_open()) {

					for (const auto& line : parameters) {

						params << line << "\n";
					}
					params.close();
					cout << paramNames.at(changeNumber - 1) << "is now changed to " << newValue << endl;
				}
				else {
					cerr << "Unable to open file" << endl;
				}
			}
			else {
				cout << "Not an int or double!" << endl;
				break;
			}
		}
		else {
			cout << "Not in range or a digit!" << endl;
			break;
		}
	}
}

//Initialize the window
bool Window::init() {

	//Initialize window surface
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialize SDL.\n";
		return 0;
	}
	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize TTF.\n";
		return 0;
	}
	//Load font
	font = TTF_OpenFont("Montserrat-Regular.ttf", 15);
	if (!font) {
		cout << "Failed to load font: " << TTF_GetError() << endl;
	}
	//Defines a cented window of a specific size
	window = SDL_CreateWindow(title.c_str(),
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  width,
							  height,
							  SDL_WINDOW_SHOWN); //Not resizable window

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
	DrawEmptyPlot(); //Draws empty plot under the nodemap

	return true;
}

//Draws a node to window with a number
void Window::DrawNode(int number, int radius, int x, int y) {

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	DrawCircle(renderer, x, y, radius);
	string nodeNumber = to_string(number);
	const char* num = nodeNumber.c_str();
	nodeNum = TTF_RenderText_Solid(font, num, { 0, 255, 0 });
	if (!nodeNum) {
		cout << "Failed to render text: \n";
	}
	else {
		//Draws the node number in the circle
		SDL_Rect numRect = { x - nodeNum->w / 2, y - nodeNum->h / 2, nodeNum->w, nodeNum->h };
		SDL_Texture* num_texture = SDL_CreateTextureFromSurface(renderer, nodeNum);
		SDL_RenderCopy(renderer, num_texture, NULL, &numRect);
	}
}

//Not used
void Window::highlight(int radius, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 255, 165, 0, SDL_ALPHA_OPAQUE);
	DrawCircle(renderer, x, y, radius);
	SDL_RenderPresent(renderer);
}
//Not used
void Window::unhighlight(int radius, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	DrawCircle(renderer, x, y, radius);
	SDL_RenderPresent(renderer);
}

//Function to connect 2 nodes, not from their centers but the rim of the drawn circles
void Window::connect2Nodes(vector<int> node1, vector<int> node2, int radius) {

	int x1, y1, x2, y2, X1, Y1, X2, Y2;
	x1 = node1.at(0);
	y1 = node1.at(1);
	x2 = node2.at(0);
	y2 = node2.at(1);
	double angle12 = getAngle(x1, y1, x2, y2); //Angle between point 1 and 2
	double angle21 = angle12 + M_PI;
	//Update new coordinates to draw from,
	//this makes the line go from the nodes circular surface rather than its center
	//makes the node number readable
	X1 = x1 + radius * cos(angle12);
	Y1 = y1 + radius * sin(angle12);
	X2 = x2 + radius * cos(angle21);
	Y2 = y2 + radius * sin(angle21);

	SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
}

//Connects all nodes based on either the final shortest path
//or the shortest path from the current iteration
void Window::connectAllNodes(vector<vector<int>> nodePos, vector<int> path, int radius, string drawMode) {

	//Sets the draw color based on whether the path is the final best, current or previous path
	if (drawMode == "Best") { SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE); }
	else if (drawMode == "Current") { SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); }
	else if (drawMode == "Previous") { SDL_SetRenderDrawColor(renderer, 20, 20, 0, SDL_ALPHA_OPAQUE); }
	else { SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); } //White for default
	vector<int> node1, node2;
	for (int i = 0; i < nodePos.size() - 1; i++) {

		//Nodes are connected to the their neighbors in the inout vector
		//For example is the path to display is {0 3 5 2 1 4},
		//first a line is drawn between node 0 and 3, then 3 and 5 etc
		node1 = nodePos.at(path.at(i));
		node2 = nodePos.at(path.at(i + 1));
		connect2Nodes(node1, node2, radius); //Uses the previous help function
	}
	//Draws a line between the last node for example 4 and 0
	node1 = nodePos.at(path.at(0));
	connect2Nodes(node1, node2, radius);
	SDL_RenderPresent(renderer);
}

//Redraws all nodes that has been placed previously
void Window::redrawNodes(vector<vector<int>> nodePos, int radius) {

	int nodex, nodey;
	for (int i = 0; i < nodePos.size(); i++) {

		nodex = nodePos.at(i).at(0);
		nodey = nodePos.at(i).at(1);
		DrawNode(i, radius, nodex, nodey);
		SDL_RenderPresent(renderer);
	}
}

void Window::saveNodes(vector<vector<int>> nodePos, string fileName)
{
	ifstream ifile(fileName);
	return ifile;

	if(ifile)
	{
		cout << "File exists" << endl;
	}

	for (size_t i = 0; i < nodePos.size(); i++) {
	}
}

//Handles the mouse click events
void Window::pollEvents() {

	SDL_Event event;
	vector<vector<int>> Lista;
	bool choosingStart = false;
	bool choosingGoal = false;
	if (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_QUIT:
			closed = true;
			break;
		case SDL_MOUSEBUTTONDOWN:

			switch (event.button.button) {

			case SDL_BUTTON_LEFT:

				SDL_GetMouseState(&x, &y); //Obtain click position
				//When left-clicking on window surface, this case acts depending on different circumstances
				if (inBounds(x, y, nodeRad, width, static_cast<int>((1 - plotRatio) * height))) {

					//In order to create or manipulate nodes,
					//the click has to in bounds of the surface,
					//and the drawn node's rim should not intersect with the edges
					//(Design choice, look's better this way).

					//Store positions.
					tempPos.at(0) = x;
					tempPos.at(1) = y;
					tempPos.at(2) = nodeRad;

					if (!collision(nodePos, tempPos, nodeRad) && nodePos.size() < 100) {

						//If no collision or inside existing node, place node.
						DrawNode(nodePos.size(), nodeRad, x, y);
						nodePos.push_back(tempPos); //Store node location in list.
						SDL_RenderPresent(renderer);
					}
					else if (insideNode(x, y, nodePos)) {

						//If inside existing node, we highlight it.
						nearestNodeIndex = getClosestNode(x, y, nodePos);
						int xCurNode = nodePos.at(nearestNodeIndex).at(0);
						int yCurNode = nodePos.at(nearestNodeIndex).at(1);
						int xPrevNode, yPrevNode;
						if (lastHighlightedNodeIndex != nearestNodeIndex && highlighted) {

							xPrevNode = nodePos.at(lastHighlightedNodeIndex).at(0);
							yPrevNode = nodePos.at(lastHighlightedNodeIndex).at(1);
							highlight(nodeRad, xCurNode, yCurNode);
							unhighlight(nodeRad, xPrevNode, yPrevNode);
							lastHighlightedNodeIndex = nearestNodeIndex;
						}
						else if (!highlighted) {

							//If no node is highlighted, highlight the current node we clicked inside.
							highlight(nodeRad, xCurNode, yCurNode);
							lastHighlightedNodeIndex = nearestNodeIndex; //Save index of the last highlighted node.
							highlighted = true;
						}
					}
				}
				else { cout << "Out of bounds! \n"; }
				break;
			case SDL_BUTTON_RIGHT:

				//Displays the parameters and lets you chose to change the current parameter
				//to fine tune the algorithm
				cout << endl;
				dispParams();
				changeParam();

				break;
			case SDL_BUTTON_MIDDLE:

					//Activates parameters before running algorithm
					optParams = loadParams();
					paramsLoaded = true;
					nrAnts = optParams.at(0);
					evaporationRate = optParams.at(1);
					nrIterations = optParams.at(2);
					initUpperPheromone = optParams.at(3);
					initLowerPheromone = optParams.at(4);
					alpha = optParams.at(5);
					beta = optParams.at(6);
					pheromoneUpdateRate = optParams.at(7);

				if (plot && nodePos.size() > 2) { //The program collapses if no nodes are placed

					//Generates the inital pheromone matrix
					vector<vector<double>> pheromones = genInitMat(nodePos.size(),
																   initUpperPheromone,
																   initLowerPheromone);
					vector<double> distances;
					vector<int> minDistancesIndex;
					double minDistance = pow(10, 6); //Sets the minimum distance to a very large baseline
					double curDistance;				 //The current minimum path length of the iteration is stored here
					vector<int> curAntPath, prevAntPath, bestAntPath;
					//Runs function with the generated initial pheromone matrix
					//and node positions
					for (int i = 0; i < nrIterations; i++) {

						//The function used return the updated pheromone matrix, minimum distance and best iteration path
						auto result = antOptOneIteration(nrAnts,
														 evaporationRate,
														 nodePos,
														 alpha,
														 beta,
														 pheromoneUpdateRate,
														 pheromones);
						curDistance = get<0>(result);
						curAntPath = get<1>(result);	  //The best ant path (for the current iteration)
						pheromones = get<2>(result);	  //Update pheromone matrix for next iteration
						distances.push_back(curDistance); //Save the minimum distance for each iteration

						if (curDistance < minDistance) {

							//If smallest path length so far has been found
							//save it to display and draw that path at the end
							minDistance = curDistance;
							bestAntPath = curAntPath;
							minDistancesIndex.push_back(i);
						}
						//Redraw map and plot with nodes without previous connections
						eraseMap();
						redrawNodes(nodePos, nodeRad);
						if (i > 0) {

							//Redraw the previous connection, but as a less visible color
							connectAllNodes(nodePos, prevAntPath, nodeRad, "Previous");
						}
						//Draw the current connection for the best path of the iteration
						connectAllNodes(nodePos, curAntPath, nodeRad, "Current");
						//Redraw plot of all minimum distances
						drawPlot(distances, minDistancesIndex);
						prevAntPath = curAntPath;
					}
					//Erases map again and fraws the best saved path
					cout << "The best distance: " << minDistance << endl;
					eraseMap();
					redrawNodes(nodePos, nodeRad);
					connectAllNodes(nodePos, bestAntPath, nodeRad, "Best");
				}

				break;
			default:
			break;
			}
			break;
		case SDL_KEYDOWN:

		switch (event.key.keysym.sym) {

		case SDLK_BACKSPACE:

		cout << "You pressed space" << endl;

		break;

		default:
		break;
	  }

			break;

		default:
		break;
		}
	}
}
