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
	float MUTATE_PROB = 0.0005f;
	float CHOSE_PROB = 0.2f;
	bool CHOSE = true;
	std::random_device rd;
	Graph *inputGraph;
	int g_size;
	long bestDist;
	int *order, *bestPath;
	int **population;
	int mutationOption;
	float *fitnessScores;
public:
	TSPGenetic();
	TSPGenetic(Graph *inputGraph, int mutationOption);
	~TSPGenetic();

	long searchForSolution(int popSize, int crossType, int expectedSolution);
	int *greedyPath();
	
	long calcDistance(int *order);
	void calcFitness(int popSize);
	void normalizeFitness(int popSize);
	
	void shuffle(int *arr, int num);
	void swap(int *arr, int a, int b);
	void insert(int *arr, int a, int b);
	void invert(int *arr, int a, int b);
	int pickBestOne(int popSize);
	int indexOf(int *arr, int arrLength, int num);
	int getWorstIndex(int **pop, int popSize);

	void genNewGeneration(int popSize, int crossType);
	int *basicCross(int *firstParent, int *secParent);
	int *crossPMX(int *firstParent, int *secParent);
	int *crossOX(int *firstParent, int *secParent);
	int *crossCX(int *firstParent, int *secParent);
	int *crossERX(int *firstParent, int *secParent);
	int iOfParent(node *erx, int parent);
	int elementsAmount(int *arr, int arrLength);
	bool elemInArr(int *arr, int arrayLength, int elem);
	void displayArr(int *arr, int arrayLength);

	void mutateOrder(int *arr);
	void getPath();
};

