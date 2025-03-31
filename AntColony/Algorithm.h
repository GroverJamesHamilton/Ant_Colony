#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <utility>
#include "restFunctions.h"

using namespace std;

vector<vector<int>> genTrails(int nodeNum, int antNum);
vector<int> genTrail(int nodeNum);
vector<int> restNodes(vector<int> visited, int nrVisits);
int nextNode(vector<int> trail, int nrVisits, vector<vector<double>> pheromones, vector<vector<int>> nodePos, double alpha, double beta);
bool alreadyVisited(vector<int> visits, int num);
double calcDist(vector<vector<int>> nodePos, vector<int> path);
vector<vector<double>> localPheromoneUpdate(double pheromoneRate, vector<int> path, int matrixSize);
void dispMatrix(vector<vector<double>> mat);
void dispIntMatrix(vector<vector<int>> mat);
double maxDistance(vector<double> distances);
pair<int, double> minDistance(vector<double> distances);
vector<vector<double>> matrixMult(vector<vector<double>> A, double mult);
vector<vector<double>> matrixAdd(vector<vector<double>> A, vector<vector<double>> B);
void dispPath(vector<int> path);
vector<vector<double>> genInitMat(int size, double upper, double lower);

vector<int> antOpt(const int nrAnts, 
				   double evapRate, 
				   int iter, 
				   vector<vector<int>> nodePos, 
				   double upper,
				   double lower,
	               double alpha, 
	               double beta, 
	               double pheroFactor);

tuple<double, vector<int>, vector<vector<double>>> antOptOneIteration(
	const int nrAnts,
	double evapRate,
	vector<vector<int>> nodePos,
	double alpha,
	double beta,
	double pheroFactor,
	vector<vector<double>> pheromoneMatrix);

