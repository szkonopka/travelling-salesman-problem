#pragma once

#include "Graph.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>

template< class RealType = double >
class uniform_real_distribution;

class TSPGenetic
{
private:
	struct node {
		int parent;
		int *sumNeighbours = new int[4];
		int length;
	};

	float mutationProb = 0.1f, crossoverProb = 0.85f, selectFromNewPopProb = 0.3f;
	float *fitnessScores;

	bool isTesting = false;

	std::random_device rd;
	Graph *inputGraph;
	int g_size, mutationOption;

	long bestDist;

	int *order, *bestPath;
	int **population;
public:
	TSPGenetic();
	TSPGenetic(Graph *inputGraph, int mutationOption, bool tests);
	~TSPGenetic();

	void setMutationProb(float mutationProb) {
		this->mutationProb = mutationProb;
	}

	void setCrossoverProb(float crossoverProb) {
		this->crossoverProb = crossoverProb;
	}

	long searchForSolution(int popSize, int crossType, int expectedSolution);
	int *greedyPath();
	
	long calcDistance(int *order);
	void calcFitness(int popSize);
	void normalizeFitness(int popSize);
	
	void shuffle(int *arr, int num);
	int iOfParent(node *erx, int parent);
	int indexOf(int *arr, int arrLength, int num);
	int getWorstIndex(int **pop, int popSize);
	int elementsAmount(int *arr, int arrLength);
	bool elemInArr(int *arr, int arrayLength, int elem);
	void displayArr(int *arr, int arrayLength);
	void getPath();

	void mutateOrder(int *arr);
	void swap(int *arr, int a, int b);
	void insert(int *arr, int a, int b);
	void invert(int *arr, int a, int b);

	int pickBestOne(int popSize);
	

	void genNewGeneration(int popSize, int crossType);

	int *basicCross(int *firstParent, int *secParent);
	int *crossPMX(int *firstParent, int *secParent);
	int *crossOX(int *firstParent, int *secParent);
	int *crossCX(int *firstParent, int *secParent);
	int *crossERX(int *firstParent, int *secParent);
};

