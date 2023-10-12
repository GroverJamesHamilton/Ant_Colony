#include "antWindow.h"
#include "restFunctions.h"
#include <iostream>
#include <algorithm>

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
	vector<vector<int>> Lista;

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

				SDL_GetMouseState(&x, &y);
				if (inBounds(x, y, nodeRad, width, height))
				{
					tempPos.at(0) = x;
					tempPos.at(1) = y;
					tempPos.at(2) = nodeRad;

					if (!collision(nodePos, tempPos, nodeRad) && nodePos.size() < 100)
					{
						nodePos.push_back(tempPos);
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
						DrawCircle(renderer, x, y, nodeRad);
						SDL_RenderPresent(renderer);
					}
					else if (insideNode(x, y, nodePos)) 
					{
						nearestNodeIndex = getClosestNode(x, y, nodePos);
						int xCurNode = nodePos.at(nearestNodeIndex).at(0);
						int yCurNode = nodePos.at(nearestNodeIndex).at(1);
						int xPrevNode, yPrevNode;
						if (lastHighlightedNodeIndex == nearestNodeIndex && highlighted)
						{
							SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
							DrawCircle(renderer, xCurNode, yCurNode, nodeRad);
							SDL_RenderPresent(renderer);
							lastHighlightedNodeIndex = 999;
							highlighted = false;
						}
						else if (!highlighted)
						{
							SDL_SetRenderDrawColor(renderer, 255, 165, 0, SDL_ALPHA_OPAQUE);
							DrawCircle(renderer, xCurNode, yCurNode, nodeRad);
							SDL_RenderPresent(renderer);
							lastHighlightedNodeIndex = nearestNodeIndex;
							highlighted = true;
						}
						else if (lastHighlightedNodeIndex != nearestNodeIndex && highlighted) 
						{
							xPrevNode = nodePos.at(lastHighlightedNodeIndex).at(0);
							yPrevNode = nodePos.at(lastHighlightedNodeIndex).at(1);
							SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
							DrawCircle(renderer, xPrevNode, yPrevNode, nodeRad);

							nodeConnection.at(0) = min(lastHighlightedNodeIndex, nearestNodeIndex);
							nodeConnection.at(1) = max(lastHighlightedNodeIndex, nearestNodeIndex);
							if (!nodeConnectionExists(nodeConnection, nodeConnections))
							{
								SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
								SDL_RenderDrawLine(renderer, xPrevNode, yPrevNode, xCurNode, yCurNode);
								connectionWeights[nodeConnection.at(0)][nodeConnection.at(1)] = distance(xPrevNode, yPrevNode, xCurNode, yCurNode);
								nodeConnections.push_back(nodeConnection);
							}
							else
							{
								double input;
								cout << "Override connection weight (Distance): \n";
								cin >> input;
								connectionWeights[nodeConnection.at(0)][nodeConnection.at(1)] = input;
							}
							SDL_RenderPresent(renderer);
							lastHighlightedNodeIndex = 999;
							highlighted = false;
						}
					}
				}
				else { cout << "Out of bounds! \n"; }

				break;
			case SDL_BUTTON_RIGHT:/*
				if (nodePos.size() > 1) 
				{
					SDL_GetMouseState(&x, &y);
					int index;
					index = getClosestNode(x, y, nodePos);
					if (index == chosenNode) 
					{
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
						DrawCircle(renderer, nodePos.at(index).at(0), nodePos.at(index).at(1), nodeRad);
						chosenNode = 1000000;
					}
					else 
					{
						if (chosenNode == 1000000) 
						{
							chosenNode = index;
							SDL_SetRenderDrawColor(renderer, 255, 165, 0, SDL_ALPHA_OPAQUE);
							DrawCircle(renderer, nodePos.at(index).at(0), nodePos.at(index).at(1), nodeRad);
						}
						else 
						{
							vector<int> curNode;
							curNode.push_back(min(nodePos.at(index).at(0), nodePos.at(index).at(1)));
							curNode.push_back(max(nodePos.at(index).at(0), nodePos.at(index).at(1)));
							if (!nodeConnectionExists(curNode, nodeConnections))
							{
								nodeConnections.push_back(curNode);
								SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
								SDL_RenderDrawLine(renderer, nodePos.at(index).at(0), nodePos.at(index).at(1), nodePos.at(chosenNode).at(0), nodePos.at(chosenNode).at(1));
								SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
								DrawCircle(renderer, nodePos.at(chosenNode).at(0), nodePos.at(chosenNode).at(1), nodeRad);
							}
							chosenNode = 1000000;
						}
					}
					SDL_RenderPresent(renderer);
				}*/
				break;
			case SDL_BUTTON_MIDDLE:

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

//Under SDL_BUTTON_MIDDLE
				/*
				if (pointPositions.size() > 1 && !linesDrawn) {
					int x1, y1, x2, y2;
					Lista = combList(pointPositions.size(), 2);
					for (int i = 0; i < Lista.size(); i++) {
						x1 = pointPositions.at(Lista.at(i).at(0)).at(0);
						y1 = pointPositions.at(Lista.at(i).at(0)).at(1);
						x2 = pointPositions.at(Lista.at(i).at(1)).at(0);
						y2 = pointPositions.at(Lista.at(i).at(1)).at(1);

						if (!lineCirclesCollision(x1, y1, x2, y2, obstPositions)) {
							SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
						}
					}
					SDL_RenderPresent(renderer);
				}*/

				//Under SDL_BUTTON_RIGHT
				/*
				SDL_GetMouseState(&x, &y);
				obstRadius = getMaxRadius(x, y, pointPositions, pointRadius, width, height);
				if (obstRadius != 0) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					DrawCircle(renderer, x, y, obstRadius);
					SDL_RenderPresent(renderer);
					tempObst.at(0) = x;
					tempObst.at(1) = y;
					tempObst.at(2) = obstRadius;
					obstPositions.push_back(tempObst);
				}
				*/