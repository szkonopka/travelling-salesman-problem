#define _CRT_SECURE_NO_WARNINGS

#include "Graph.h"
#include <iostream>
#include "TSPGenetic.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>



void testGenetic(int popSize, int cross, std::string fileName, std::string instanceName, int instanceSize, int mutation, int expectedSolution) {
	Graph *g = new Graph(instanceSize);
	
	//instanceName = instanceName + ".txt";
	//g->readFromFileAsym(instanceName);
	
	if (!g->readDataParser("data/" + instanceName, 0)) {
		exit(0);
	}

	TSPGenetic *tsp;
	int sumD = 0, solutionD = 0, countOpt = 0;
	fileName = "results/" + fileName + ".txt";

	std::fstream file;
	file.open(fileName, std::ios::out);

	if (file.good()) {
		for (int i = 0; i < 10; i++) {
			
			tsp = new TSPGenetic(g, mutation, true);
			std::cout << i << ". solution: \n";
			solutionD = tsp->searchForSolution(popSize, cross, expectedSolution);
			printf("%d\n", solutionD);
			tsp->getPath();
			sumD += solutionD;
			if (solutionD == expectedSolution)
				countOpt++;
		}
		file << "Average length: " << sumD / 10;
		file << "Opt: " << countOpt << "/" << 10;
		delete tsp, g;
	}
	file.close();
	
}

void genetic(int popSize, int cross, std::string fileName, std::string instanceName, int instanceSize, int mutation, int expectedSolution) {
	Graph *g = new Graph(instanceSize);
	if (!g->readDataParser("data/" + instanceName, 0)) {
		exit(0);
	}
	int solutionD = 0;	
	TSPGenetic *tsp = new TSPGenetic(g, mutation, false);
	solutionD = tsp->searchForSolution(popSize, cross, expectedSolution);
	tsp->getPath();
	printf("%d\n", solutionD);
	delete tsp, g;
}

void geneticMenu(int POPULATION_SIZE, std::string instance, int instanceSize, int option) {
	int expectedSolution;
	short cross, mutation;

	printf("Select crossover:\n");
	printf("DEFAULT - basic crossover\n2. PMX crossover\n3. OX crossover\n4. CX crossover\n5. ERX crossover\n");
	std::cin >> cross;

	printf("Select mutation:\n");
	printf("1. Insert\n2. Swap\n3. Invert\n");
	std::cin >> mutation;

	printf("Type expected solution, if there you don't know it then just type 0.\n");
	std::cin >> expectedSolution;

	std::string crossName = instance;

	if (cross == 2) crossName += "pmx";
	else if (cross == 3) crossName += "ox";
	else if (cross == 4) crossName += "cx";
	else if (cross = 5) crossName += "erx";
	else crossName += "basicx";

	if (mutation == 1) crossName += "insert";
	else if (mutation == 2) crossName += "swap";
	else if (mutation == 3) crossName += "invert";

	switch (option) {
	case 1:
		testGenetic(POPULATION_SIZE, cross, crossName, instance, instanceSize, mutation, expectedSolution);
		break;
	case 2:
		genetic(POPULATION_SIZE, cross, crossName, instance, instanceSize, mutation, expectedSolution);
		break;
	}
}

void graphParameters(std::string &instance, int &instanceSize) {
	printf("Type filename of the instance.\n");
	std::cin >> instance;

	printf("Type instance size.\n");
	std::cin >> instanceSize;
}

int main(int *argc, char **argv) {
	std::string instance;
	int instanceSize, option;
	int POPULATION_SIZE = 1000;

	while (true) {
		printf("\n0 - CHOOSE THE GRAPH\n1 - TEST\n2 - SINGLE RUN\nDEFAULT - EXIT\n");
		std::cin >> option;
		switch (option) {
		case 0:
			graphParameters(instance, instanceSize);
			break;
		case 1:
			geneticMenu(POPULATION_SIZE, instance, instanceSize, option);
			break;
		case 2:
			geneticMenu(POPULATION_SIZE, instance, instanceSize, option);
			break;
		default:
			return 0;
			break;
		}
	}
	return 0;
}
