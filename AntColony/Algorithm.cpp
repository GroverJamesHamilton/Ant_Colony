#include "Algorithm.h"
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <tuple>

//Display an ants node path from a vector i.e. 0 4 2 3 1
void dispPath(vector<int> path)
{
	for (int i = 0; i < path.size(); ++i)
	{
		cout << path.at(i) << " ";
	}
	cout << endl;
}

//Generates an empty trail node a random node in range [0,nodeNum-1]
//starting at a node in the given range, the rest of the vector is nodeNum-1 zeros
//For example: genTrail(5) -> 3 0 0 0 0 or 0 0 0 0 0
vector<int> genTrail(int nodeNum)
{
	//Generate a random number in range [0,nodeNum-1]
	vector<int> emptyTrail;
	random_device randDev;
	mt19937 gen(randDev());
	uniform_int_distribution<> unDist(0, nodeNum - 1);
	emptyTrail.push_back(unDist(gen));
	//Push back nodeNum-1 zeros for rest of vector
	for (int i = 1; i < nodeNum; i++)
	{
		emptyTrail.push_back(0);
	}
	//dispPath(emptyTrail);
	return emptyTrail;
}

//Generate several trails in a 2d-vector, one per ant in out implementation
vector<vector<int>> genTrails(int nodeNum, int nrAnts)
{
	vector<vector<int>> trails;
	for (int i = 0; i < nrAnts; i++)
	{
		trails.push_back(genTrail(nodeNum));
	}
	return trails;
}

//Determines if an ant has already visited node num
bool alreadyVisited(vector<int> visits, int num)
{
	bool visited = false;
	for (int i = 0; i < visits.size(); i++)
	{
		if (visits.at(i) == num)
		{
			visited = true;
		}
	}
	return visited;
}

//Returns the nodes the ant haven't travelled to
vector<int> restNodes(vector<int> visited, int nrVisits)
{
	vector<int> visits;
	vector<int> rest;
	//cout << "Visited: ";
	for (int i = 0; i < nrVisits; i++)
	{
		visits.push_back(visited.at(i));
		//cout << visited.at(i) << " ";
	}
	//cout << endl;
	for (int j = 0; j < visited.size(); j++)
	{
		if (!alreadyVisited(visits, j))
		{
			rest.push_back(j);
		}
	}
	//cout << "Rest: ";
	//dispPath(rest);
	return rest;
}

//Return the sum of vector
double sumVec(vector<double> vec)
{
	double sum = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		sum = sum + vec.at(i);
	}
	return sum;
}

//Determines the next node the ant should travel to
//based on probabilities and then selected
int nextNode(vector<int> trail, 
			 int nrVisits, 
			 vector<vector<double>> pheromones, 
			 vector<vector<int>> nodePos, 
			 double alpha, 
	         double beta)
{
	int curNode = trail.at(nrVisits - 1);
	int availNode;
	vector<int> notVisited = restNodes(trail, nrVisits);
		vector<double> tau, eta;
		double tmp, x1, y1, x2, y2;
		double sum = 0;
		double cumulProb = 0;
		int index;
		for (int i = 0; i < notVisited.size(); i++)
		{
			availNode = notVisited.at(i);
			tmp = pheromones[curNode][availNode];
			tau.push_back(pow(tmp, alpha));
			x1 = nodePos.at(curNode).at(0);
			y1 = nodePos.at(curNode).at(1);
			x2 = nodePos.at(availNode).at(0);
			y2 = nodePos.at(availNode).at(1);
			tmp = 1 / (distance(x1, y1, x2, y2));
			eta.push_back(pow(tmp, beta));
			sum = sum + tau.at(i) * eta.at(i);
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> distribution(0.0, 1.0);
		double rand = distribution(gen);
		//cout << "Random value: " << rand << endl;
		
		//The ant chooses next node based on heuristic + pheromone probability
		// as well a random factor to encourage exploration 
		for (int i = 0; i < notVisited.size(); i++)
		{
			tmp = tau.at(i) * eta.at(i) / sum;
			cumulProb = cumulProb + tmp;
			//cout << notVisited.at(i) << " : " << tmp << " " << cumulProb << endl;
			if (rand <= cumulProb)
			{
				index = notVisited.at(i);
				break;
			}
		}
		//cout << "We move to node: " << index << endl;
		return index;
	}

//Calculate the distance an ant travels based on the path it has chosen
double calcDist(vector<vector<int>> nodePos, vector<int> path)
{
	double x1, x2, y1, y2;
	double totalDist = 0;
	int node1, node2;
	for (int i = 0; i < path.size()-1; i++)
	{
		node1 = path.at(i);
		node2 = path.at(i + 1);
		x1 = nodePos.at(node1).at(0);
		y1 = nodePos.at(node1).at(1);
		x2 = nodePos.at(node2).at(0);
		y2 = nodePos.at(node2).at(1);
		totalDist = totalDist + (distance(x1, y1, x2, y2));
		//cout << "Distance between node " << node1 << " and node " << node2 << " : " << (distance(x1, y1, x2, y2)) << endl;
	}
	//Adding the distance from final node to the starting node
	node1 = path.at(0);
	x1 = nodePos.at(node1).at(0);
	y1 = nodePos.at(node1).at(1);
	totalDist = totalDist + (distance(x1, y1, x2, y2));
	//cout << "Distance between node " << node1 << " and node " << node2 << " : " << (distance(x1, y1, x2, y2)) << endl;
	return totalDist;
}

//Returns the pheromone dispersed by a single ant based on the path it takes
vector<vector<double>> localPheromoneUpdate(double pheromoneRate, vector<int> path, int matrixSize)
{
	vector<vector<double>> pheromoneDelta(matrixSize, vector<double>(matrixSize, 0));
	int node1, node2;
	path.push_back(path.at(0));
	for (int i = 0; i < path.size()-1; i++)
	{
		node1 = path.at(i);
		node2 = path.at(i + 1);
		pheromoneDelta.at(node1).at(node2) = pheromoneRate;
		pheromoneDelta.at(node2).at(node1) = pheromoneRate;
	}
	return pheromoneDelta;
}

//Display a matrix (help function for developing)
void dispMatrix(vector<vector<double>> mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat.size(); j++)
		{
			cout << mat.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

//Help function, displays int matrix
void dispIntMatrix(vector<vector<int>> mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat.size(); j++)
		{
			cout << mat.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

//Returns the maximum value from a double vector
double maxDistance(vector<double> distances)
{
	double max = 0;
	for (int i = 0; i < distances.size(); i++)
	{
		if (distances.at(i) > max)
		{
			max = distances.at(i);
		}
	}
	return max;
}

//Returns the minimum value from a double vector
pair<int, double> minDistance(vector<double> distances)
{
	double min = pow(10,9);
	int index;
	for (int i = 0; i < distances.size(); i++)
	{
		if (distances.at(i) < min)
		{
			min = distances.at(i);
			index = i;
		}
	}
	return make_pair(index, min);
}

//Function to multiplicate matrixes of same size and shape
vector<vector<double>> matrixMult(vector<vector<double>> A, double mult)
{
	for (size_t i = 0; i < A.size(); ++i)
	{
		for (size_t j = 0; j < A[i].size(); ++j)
		{
			A[i][j] = mult*A[i][j];
		}
	}
	return A;
}

//Function to add matrixes of same size and shape
vector<vector<double>> matrixAdd(vector<vector<double>> A, vector<vector<double>> B)
{
	if (A.size() == B.size() && A[0].size() == B[0].size())
	{
		// Perform element-wise addition and update matrix B
		for (size_t i = 0; i < A.size(); ++i)
		{
			for (size_t j = 0; j < A[i].size(); ++j)
			{
				B[i][j] += A[i][j];
			}
		}
	}
	else { cout << "Not same matrix sizes!" << endl; }
		return B;
}

//Gen initial pheromone matrix with each cell
//a random value between lower and upper limit
vector<vector<double>> genInitMat(int size, double upper, double lower)
{
	vector<vector<double>> initPheromones(size, vector<double>(size, 0));
	//Initialize random valie generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distribution(lower, upper);
	double rand = distribution(gen);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			rand = distribution(gen);
			initPheromones.at(i).at(j) = rand;
		}
	}

	return initPheromones;
}

//The full ACO algorithm, not used in the code
vector<int> antOpt(const int nrAnts, 
				   double evapRate, 
				   int iter, 
	               vector<vector<int>> nodePos, 
				   double upper,
				   double lower,
	               double alpha, 
	               double beta, 
	               double pheroFactor)
{
	vector<vector<int>> antPaths; //Ant paths generated
	vector<vector<double>> pheromones = genInitMat(nodePos.size(), upper, lower); //Initial pheromone matrix
	vector<vector<vector<double>>> pheromoneUpdates; //3D-vector with size nrAnts x nodePos.size() x nodePos.size() 
													 //Each layer is the pheromone scattered for the current iteration per ant 
	vector<double> pathDistances(nrAnts); //Distance per ant for the current iteration
	vector<double> tripsPerAnt(nrAnts);   //Nr trips each ant makes for the current iteration

	double minimum = 100000000; //A large value which will updated after the shortest path distance
	int next;
	double tripLength;
	int indexBest; //Index of the best ant path

	for (int k = 0; k < iter; k++)
	{
		antPaths = genTrails(nodePos.size(), nrAnts); //Place each ant on a random node and generate rest of trail empty
		//for example for 5 nodes in map and 3 ants: 
		// 3 0 0 0 0
		// 1 0 0 0 0
		// 0 0 0 0 0
		//Could be returned
		for (int i = 0; i < nrAnts; i++)
		{
			//Path decision making per ant based on pheromones, heuristics and randomness 
			for (int j = 1; j < nodePos.size(); j++)
			{
				//Fill in the next nodes based 
				next = nextNode(antPaths.at(i), j, pheromones, nodePos, alpha, beta);
				antPaths.at(i).at(j) = next;
			}
			//Calculate distances for each ant path 
			pathDistances.at(i) = calcDist(nodePos, antPaths.at(i));
			//The pheromones scattered by the ant for one trip 
			vector<vector<double>> pheroUpdate = localPheromoneUpdate(pheroFactor, antPaths.at(i), nodePos.size());
			pheromoneUpdates.push_back(pheroUpdate);
		}
		//Display the shortest path distance for this iteration
		pair<int, double> values = minDistance(pathDistances);
		indexBest = values.first;
		//std::cout << "Minimum path distance: " << values.second << endl;
		std::cout << values.second << endl;
		//If the shortest path so far, save distance
		if (values.second < minimum)
		{
			minimum = values.second;
		}
		if (k == 0)
		{
			//Sets the trip length based on the longest ant path for the first iteration
			tripLength = 10*maxDistance(pathDistances); 
		}
		//cout << "Nr trips per ant: " << endl;

		//Creates a delta matrix for how much pheromone is added for each ant and trips
		vector<vector<double>> deltaPhero(nodePos.size(), vector<double>(nodePos.size(), 0));
		for (int i = 0; i < nrAnts; i++)
		{
			//Pheromone per ant is added based on ths proportion
			//could 11.455.. trips 
			tripsPerAnt.at(i) = tripLength / pathDistances.at(i); 
			//Update the delta pheromones
			deltaPhero = matrixAdd(deltaPhero, matrixMult(pheromoneUpdates.at(i), tripsPerAnt.at(i)));
		}
		//Evaporate from the pheromone matrix and add the new pheromones from the current iteration
		//Ph(i) = Ph(i-1)*(1-evaprate) + deltaPh
		pheromones = matrixAdd(matrixMult(pheromones, (1 - evapRate)), deltaPhero);
	}
	cout << "Minimum path length: " << minimum << endl;
	return antPaths.at(indexBest);
}

//Same algorithm as above but just one iteration,
//i.e. the pheromone matrix given should be reused as an input when calling the fucntion again
//The motivation behind this is to plot the results online for each iteration 
tuple<double, vector<int>, vector<vector<double>>> antOptOneIteration(
	const int nrAnts,
	double evapRate,
	vector<vector<int>> nodePos,
	double alpha,
	double beta,
	double pheroFactor,
	vector<vector<double>> pheromones)
{
	vector<vector<int>> antPaths;
	//vector<vector<double>> pheromones = genInitMat(nodePos.size(), upper, lower);
	vector<vector<vector<double>>> pheromoneUpdates;
	vector<double> pathDistances(nrAnts);
	vector<double> tripsPerAnt(nrAnts);

	double minimum = 100000000;

	int next;
	double tripLength;
	int indexBest;

	antPaths = genTrails(nodePos.size(), nrAnts);
	for (int i = 0; i < nrAnts; i++)
	{
		for (int j = 1; j < nodePos.size(); j++)
		{
			next = nextNode(antPaths.at(i), j, pheromones, nodePos, alpha, beta);
			antPaths.at(i).at(j) = next;
		}
		pathDistances.at(i) = calcDist(nodePos, antPaths.at(i));
		vector<vector<double>> pheroUpdate = localPheromoneUpdate(pheroFactor, antPaths.at(i), nodePos.size());
		pheromoneUpdates.push_back(pheroUpdate);
	}

	pair<int, double> values = minDistance(pathDistances);
	indexBest = values.first;
	std::cout << values.second << endl;
	if (values.second < minimum)
	{
		minimum = values.second;
	}
	tripLength = 10 * maxDistance(pathDistances);
	vector<vector<double>> deltaPhero(nodePos.size(), vector<double>(nodePos.size(), 0));
	for (int i = 0; i < nrAnts; i++)
	{
		tripsPerAnt.at(i) = tripLength / pathDistances.at(i);
		deltaPhero = matrixAdd(deltaPhero, matrixMult(pheromoneUpdates.at(i), tripsPerAnt.at(i)));
	}
	pheromones = matrixAdd(matrixMult(pheromones, (1 - evapRate)), deltaPhero);

	return make_tuple(minimum, antPaths.at(indexBest), pheromones);
}





















