#define _CRT_SECURE_NO_WARNINGS

#include "Graph.h"
#include <iostream>
#include "TSPGenetic.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>

int expectedSolution = 1272;

void testGenetic(int popSize, int cross, std::string fileName, std::string instanceName, int instanceSize, int mutation) {
	Graph *g = new Graph(instanceSize);
	
	//instanceName = instanceName + ".txt";
	//g->readFromFileAsym(instanceName);
	
	if (!g->readData("data/" + instanceName, 0)) {
		exit(0);
	}

	TSPGenetic *tsp;
	int sumD = 0, solutionD = 0, countOpt = 0;
	fileName = "results/" + fileName + ".txt";

	std::fstream file;
	file.open(fileName, std::ios::out);

	if (file.good()) {
		for (int i = 0; i < 10; i++) {
			
			tsp = new TSPGenetic(g, mutation);
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

void genetic(int popSize, int cross, std::string fileName, std::string instanceName, int instanceSize, int mutation) {
	Graph *g = new Graph(instanceSize);
	if (!g->readData("data/" + instanceName, 0)) {
		exit(0);
	}
	int solutionD = 0;	
	TSPGenetic *tsp = new TSPGenetic(g, mutation);
	solutionD = tsp->searchForSolution(popSize, cross, expectedSolution);
	tsp->getPath();
	delete tsp, g;
}

void geneticMenu(int POPULATION_SIZE, std::string instance, int instanceSize, int option) {
	short cross, mutation;
	printf("Select crossover:\n");
	printf("DEFAULT - basic crossover\n2. PMX crossover\n3. OX crossover\n4. CX crossover\n5. ERX crossover\n");
	scanf("%d", &cross);

	printf("Select mutation:\n");
	printf("1. INSERT\n2.SWAP\n3.INVERT\n");
	std::cin >> mutation;

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
		testGenetic(POPULATION_SIZE, cross, crossName, instance, instanceSize, mutation);
		break;
	case 2:
		genetic(POPULATION_SIZE, cross, crossName, instance, instanceSize, mutation);
		break;
	}
	
	
}

void showFiles() {
	std::vector<std::string> names;
	std::string path = "data/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile((LPCWSTR)path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back("ee");
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	for each(std::string e in names) {
		std::cout << e << "\n" << std::endl;
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
	const int POPULATION_SIZE = 1000;

	while (true) {
		printf("\n0 - choose the graph\n1 - Tests\n2 - Single run\nDEFAULT - EXIT\n");
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
