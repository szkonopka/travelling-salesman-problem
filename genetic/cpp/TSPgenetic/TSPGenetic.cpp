#include "TSPGenetic.h"
#pragma optimize( "", on )

TSPGenetic::TSPGenetic() {}

TSPGenetic::TSPGenetic(Graph *inputGraph, int mutationOption) {
	this->inputGraph = inputGraph;
	this->g_size = inputGraph->size;
	this->order = new int[g_size];
	this->mutationOption = mutationOption;
	for (int i = 0; i < g_size; i++) {
		order[i] = i;
	}
	bestDist = LONG_MAX;
	bestPath = new int[g_size];
}

TSPGenetic::~TSPGenetic() {}

long TSPGenetic::searchForSolution(int popSize, int crossType, int expectedSolution) {
	population = new int*[popSize];
	fitnessScores = new float[popSize];
	for (int i = 0; i < popSize; i++) {
		population[i] = new int[g_size];
		for (int j = 0; j < g_size; j++) {
			population[i][j] = order[j];
		}
		shuffle(population[i], 5);
	}
	calcFitness(popSize);
	std::cout << "1. generation - route costs " << bestDist << std::endl;

	std::fstream file;
	std::string fileName = "results/" + std::to_string(g_size) + ".txt";
	file.open(fileName, std::ios::out);
	if (file.good()) {
		file << "Inti value: " << bestDist;
	}
	file.close();

	normalizeFitness(popSize);
	genNewGeneration(popSize, crossType);
	calcFitness(popSize);

	std::cout << "2. generation - route costs " << bestDist << std::endl;
	int i;
	int tmpDist, globMutation = 0;
	int diversificationUseless = 0;
	for (i = 0; i < 1000; i++) {
		tmpDist = bestDist;
		if (bestDist == expectedSolution)
			return bestDist;
		normalizeFitness(popSize);
		genNewGeneration(popSize, crossType);
		calcFitness(popSize);
		std::cout << i + 3 << ". generation - route costs " << bestDist << std::endl;
		if(MUTATE_PROB != 0.0)
			if (tmpDist == bestDist) {
				globMutation++;
				if (globMutation >= 500) {
					if (diversificationUseless >= 3) {
						return bestDist;
					}
					//std::cout << "MUTATE ALL POPULATION IN A RESULT OF DIVERSIFICATION!" << std::endl;
					globMutation = 0;
					float tmpMut = MUTATE_PROB;
					MUTATE_PROB = 0.3f;
					for (int j = 0; j < popSize; j++) {
						mutateOrder(population[j]);
					} 
					MUTATE_PROB = tmpMut;
					diversificationUseless++;
				}
			}
			else {
				globMutation = 0;
				diversificationUseless = 0;
			}
	}
	return bestDist;
}

long TSPGenetic::calcDistance(int *pop) {
	int indexA, indexB;
	long dSum = 0;
	for (int i = 0; i < g_size - 1; i++) {
		indexA = pop[i];
		indexB = pop[i + 1];
		dSum += inputGraph->cities[indexA][indexB];
	}
	dSum += inputGraph->cities[indexB][pop[0]];
	return dSum;
}

void TSPGenetic::calcFitness(int popSize) {
	int tempDist;
	for (int i = 0; i < popSize; i++) {
		tempDist = calcDistance(population[i]);
		if (tempDist < bestDist) {
			bestDist = tempDist;
			for (int j = 0; j < g_size; j++) {
				bestPath[j] = population[i][j];
			}
		}
		fitnessScores[i] = (float)1 / (float)(tempDist + 1);
	}
}

void TSPGenetic::normalizeFitness(int popSize) {
	float fitSum = 0;
	for (int i = 0; i < popSize; i++) {
		fitSum += fitnessScores[i];
	}
	for (int i = 0; i < popSize; i++) {
		fitnessScores[i] = fitnessScores[i] / fitSum;
	}
}

/*
	Selekcja - funkcja ruletki. Losuje losow¹ liczbê z przedzia³u od 0.0 do 1.0, nastêpnie odejmuje od jej wartoœci
	kolejne wartoœci oceny ruchu kolejnych osobników, w momencie w którym wartoœæ spadnie poni¿ej zera zwracamy indeks
	osobnika, który spowodowa³ t¹ wartoœæ.
*/
int TSPGenetic::pickBestOne(int popSize) {
	int i = 0;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	float r = (float)dis(gen);

	while ((r > (float)0) && (i < popSize)) {
		r = r - fitnessScores[i];
		i++;
	}
	i = i - 1;
 	return i;
}

/*
	Funkcja przetasowania - przetasowuje losowo wybrane elementy tablicy.
*/
void TSPGenetic::shuffle(int *arr, int num) {
	int indexA, indexB;
	for (int i = 0; i < num; i++) {
		indexA = (rand() % g_size) + 0;
		indexB = (rand() % g_size) + 0;
		swap(arr, indexA, indexB);
	}
}

/*
	SWAP odpowiedzialne jest za zamianê miejscami wybranych wczeœniej chromosonów.
*/
void TSPGenetic::swap(int *arr, int a, int b) {
	int tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
}

void TSPGenetic::invert(int *arr, int a, int b) {
	int i, tmp;
	if (a > b) {
		tmp = a;
		a = b;
		b = tmp;
	}
	if (a == b) {
		return;
	}
	else if (b == (a + 1)) {
		tmp = arr[a];
		arr[a] = arr[b];
		arr[b] = tmp;
	}

	int cutLength = b + 1 - a;
	int *cutArr = new int[cutLength];
	int j = 0;
	for (i = a; i < b + 1; i++) {
		cutArr[j] = arr[i];
		j++;
	}

	for (i = 0; i < (cutLength / 2); i++) {
		tmp = cutArr[i];
		cutArr[i] = cutArr[cutLength - 1 - i];
		cutArr[cutLength - 1 - i] = tmp;
	}

	j = 0;
	for (int i = a; i < b + 1; i++) {
		arr[i] = cutArr[j];
		j++;
	}
}

inline void TSPGenetic::insert(int *arr, int a, int b) {
	int *tmpArr = new int[g_size];
	if (a > b) {
		int tmp = a;
		a = b;
		b = tmp;
	}

	int i;
	for (i = 0; i < a; i++)
		tmpArr[i] = arr[i];
	tmpArr[a] = arr[b];
	tmpArr[a + 1] = arr[a];
	for (i = a + 2; i < b + 1; i++) {
		tmpArr[i] = arr[i - 1];
	}
	for (i = b + 1; i < g_size; i++) {
		tmpArr[i] = arr[i];
	}
	for (int i = 0; i < g_size; i++) {
		arr[i] = tmpArr[i];
	}
}

bool TSPGenetic::elemInArr(int *arr, int arrayLength, int elem) {
	for (int i = 0; i < arrayLength; i++) {
		if (elem == arr[i])
			return 1;
	}
	return 0;
}

int TSPGenetic::indexOf(int *arr, int arrLength, int num) {
	for (int i = 0; i < arrLength; i++)
		if (num == arr[i])
			return i;
}

void TSPGenetic::mutateOrder(int *arr) {
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	int indexA, indexB;
	for (int i = 0; i < g_size; i++) {
		if (dis(gen) < MUTATE_PROB) {
			indexA = (rand() % g_size) + 0;
			indexB = (rand() % g_size) + 0;
			if (indexA == indexB)
				indexB = (indexA + 1) % g_size;
			if(mutationOption == 1)
				insert(arr, indexA, indexB);
			else if (mutationOption == 2)
				swap(arr, indexA, indexB);	
			else
				invert(arr, indexA, indexB);	
		}
	}
}

void TSPGenetic::displayArr(int *arr, int arrayLength) {
	for (int i = 0; i < arrayLength; i++)
		printf("%hd ", arr[i]);
	printf("\n");
}

void TSPGenetic::genNewGeneration(int popSize, int crossType) {
	int **newPopulation = new int*[popSize];
	for (int i = 0; i < popSize; i++) {
		newPopulation[i] = new int[g_size];
		int *firstParent = population[pickBestOne(popSize)];
		int *secParent = population[pickBestOne(popSize)];
		switch (crossType) {
		case 1:
			newPopulation[i] = basicCross(firstParent, secParent);
			break;
		case 2:
			newPopulation[i] = crossPMX(firstParent, secParent);
			break;
		case 3:
			newPopulation[i] = crossOX(firstParent, secParent);
			break;
		case 4:
			newPopulation[i] = crossCX(firstParent, secParent);
			break;
		case 5:
			newPopulation[i] = crossERX(firstParent, secParent);
			break;
		}
	}
	int tmpInd;
	std::mt19937 gen(rd());
	for (int i = 0; i < popSize; i++) {
		mutateOrder(newPopulation[i]);
		std::uniform_real_distribution<> dis(0.0, 1.0);
		if (dis(gen) < CHOSE_PROB) {
				for (int j = 0; j < g_size; j++) {
					population[i][j] = newPopulation[i][j];
				}
		}
		else {
			tmpInd = getWorstIndex(population, popSize);
			for (int j = 0; j < g_size; j++) {
				population[tmpInd][j] = newPopulation[i][j];
			}
		}
	}
	delete newPopulation;
}

int TSPGenetic::getWorstIndex(int **pop, int popSize) {
	int worstIndex = 0;
	int tempWorst = INT_MIN;
	int currentDist;
	for (int i = 0; i < popSize; i++) {
		currentDist = calcDistance(pop[i]);
		if (currentDist > tempWorst) {
			tempWorst = currentDist;
			worstIndex = i;
		}
	}
	return worstIndex;
}

int *TSPGenetic::basicCross(int *firstParent, int *secParent) {
	int start, end, pivot, city;
	pivot = g_size / 2;
	start = (rand() % pivot) + 0;
	end = (rand() % (g_size - start)) + start;
	int *child = new int[g_size];
	for (int i = 0; i < g_size; i++) {
		child[i] = -1;
	}
	int tempLen = end - start;
	for (int i = 0; i < tempLen; i++) {
		child[i] = firstParent[start + i];
	}
	int citiesLeft = g_size - tempLen;
	int i = 0, j = 0;

	while(j < citiesLeft) {
		city = secParent[i];
		if (!elemInArr(child, g_size, city)) {
			child[tempLen + j] = city;
			j++;
		}
		i++;
	}
	return child;
}

int *TSPGenetic::crossPMX(int *firstParent, int *secParent) {
	int start, end, pivot;
	
	pivot = g_size / 2;
	start = (rand() % pivot) + 0;
	end = (rand() % (g_size - start)) + start;
	int *child = new int[g_size];

	for (int i = 0; i < g_size; i++)
		child[i] = -1;

	int tempLen = end - start;

	for (int i = start; i < end; i++) 
		child[i] = secParent[i];
	
	int *crossSpaceA = new int[tempLen], *crossSpaceB = new int[tempLen];
	for (int i = 0; i < tempLen; i++) {
		crossSpaceA[i] = firstParent[start + i];
		crossSpaceB[i] = secParent[start + i]; 
	}	

	for (int i = 0; i < g_size; i++) {
		if ((!elemInArr(crossSpaceA, tempLen, firstParent[i])) && (!elemInArr(child, g_size, firstParent[i]))) {
			child[i] = firstParent[i];
		}
	}

	int j = 0;
	for (int i = 0; i < g_size; i++) {
		if (child[i] == -1) {
			while (elemInArr(child, g_size, crossSpaceA[j]))
				j++;
			child[i] = crossSpaceA[j];
		}		
	}
	return child;
}

int *TSPGenetic::crossOX(int *firstParent, int *secParent) {
	int start, end, pivot;
	pivot = g_size / 2;
	start = (rand() % pivot) + 0;
	end = (rand() % (g_size - start)) + start;
	int *child = new int[g_size];

	for (int i = 0; i < g_size; i++) 
		child[i] = -1;

	int tempLen = end - start;

	for (int i = start; i < end; i++)
		child[i] = secParent[i];
	
	int *crossSpaceB = new int[tempLen];
	for (int i = 0; i < tempLen; i++)
		crossSpaceB[i] = secParent[start + i];

	int i = end, j = end, count = 0;
	while (count < g_size) {
		j = j % g_size;
		i = i % g_size;
		if (!elemInArr(crossSpaceB, tempLen, firstParent[i])) {
			child[j] = firstParent[i];
			j++;
		}
		i++;
		count++;
	}
	return child;
}

/*
	Krzy¿owanie CX (cycled crossover). Polega na odnalezieniu wœród rodziców cyklu, poczynaj¹c 
*/
int *TSPGenetic::crossCX(int *firstParent, int *secParent) {
	int i = 0, first = firstParent[i], second = secParent[i], current, currentIndex;
	int *child = new int[g_size];
	while (first == second) {
		i++;
		first = firstParent[i], second = secParent[i];
		if (i == g_size)
			return firstParent;
	}
	for (int i = 0; i < g_size; i++)
		child[i] = -1;

	current = second;
	currentIndex = i;
	child[currentIndex] = firstParent[currentIndex];
	while (first != current) {
		currentIndex = indexOf(firstParent, g_size, current);
		current = secParent[currentIndex];
		child[currentIndex] = firstParent[currentIndex];
	}
	for (int i = 0; i < g_size; i++) {
		if (child[i] == -1)
			child[i] = secParent[i];
	}
	return child;
}

/*
	Krzy¿owanie ERX - polega na wypisaniu dla ka¿dego wierzcho³ka s¹siadów (z prawej i lewej) z dwóch rodziców,
	w przypadku, je¿eli s¹siedzi powtarzaj¹ siê to bierzemy pod uwagê tylko pierwszego, po wypisaniu dla ka¿dego
	wierzcho³ka 
*/
int *TSPGenetic::crossERX(int *firstParent, int *secParent) {
	node *erxList = new node[g_size];
	int **firstNeighbours = new int*[g_size], **secNeighbours = new int*[g_size], *child = new int[g_size];
	for (int i = 0; i < g_size; i++) {
		child[i] = -1;
		firstNeighbours[i] = new int[2];
		secNeighbours[i] = new int[2];
		if ((i - 1) < 0) {
			firstNeighbours[i][0] = firstParent[i + 1];
			firstNeighbours[i][1] = firstParent[g_size - 1];
			secNeighbours[i][0] = secParent[i + 1];
			secNeighbours[i][1] = secParent[g_size - 1];
		}
		else {
			firstNeighbours[i][0] = firstParent[i - 1];
			firstNeighbours[i][1] = firstParent[(i + 1) % g_size];
			secNeighbours[i][0] = secParent[i - 1];
			secNeighbours[i][1] = secParent[(i + 1) % g_size];
		}
	}
	
	int secIndex;
	for (int i = 0; i < g_size; i++) {
		for (int j = 0; j < 4; j++) {
			erxList[i].sumNeighbours[j] = -1;
		}
		erxList[i].parent = firstParent[i];
		secIndex = indexOf(secParent, g_size, erxList[i].parent);
		for (int j = 0; j < 4; j++) {
			if (j < 2) {
				erxList[i].sumNeighbours[j] = firstNeighbours[i][j];
			}
			else if (j >= 2 && (!elemInArr(erxList[i].sumNeighbours, 4, secNeighbours[secIndex][j - 2]))) {
				erxList[i].sumNeighbours[j] = secNeighbours[secIndex][j - 2];
			}
			else {
				erxList[i].sumNeighbours[j] = -1;
			}
		}
	}

	for (int i = 0; i < g_size; i++) {
		erxList[i].length = elementsAmount(erxList[i].sumNeighbours, 4);
	}
	delete firstNeighbours, secNeighbours;

	int currentNode, i = 0, bestLength = 4, tempNode, tempNodeIndex, chosenNodeIndex = 0, newNode;
	currentNode = erxList[chosenNodeIndex].parent;
	while (elementsAmount(child, g_size) < g_size) {
		child[i] = currentNode;
		for (int i = 0; i < g_size; i++) {
			if (elemInArr(erxList[i].sumNeighbours, 4, currentNode)) {
				erxList[i].sumNeighbours[indexOf(erxList[i].sumNeighbours, 4, currentNode)] = -1;
				erxList[i].length = elementsAmount(erxList[i].sumNeighbours, 4);
			}
		}
		for (int j = 0; j < 4; j++) {
			if (erxList[chosenNodeIndex].sumNeighbours[j] == -1)
				continue;
			tempNode = erxList[chosenNodeIndex].sumNeighbours[j];
			tempNodeIndex = iOfParent(erxList, tempNode);
			if (erxList[tempNodeIndex].length <= bestLength) {
				bestLength = erxList[tempNodeIndex].length;
				newNode = tempNode;
			}
		}
		if (chosenNodeIndex == iOfParent(erxList, newNode)) {
			for (int i = 0; i < g_size; i++) {
				if (!elemInArr(child, g_size, erxList[i].parent)) {
					currentNode = erxList[i].parent;
					break;
				}
			}
		}
		else {
			chosenNodeIndex = iOfParent(erxList, newNode);
			currentNode = erxList[chosenNodeIndex].parent;
		}
		bestLength = 4;
		i++;
	}
	delete erxList;
	return child;
}

int TSPGenetic::iOfParent(node *erx, int parent) {
	for (int i = 0; i < g_size; i++) {
		if (erx[i].parent == parent)
			return i;
	}
}

int TSPGenetic::elementsAmount(int *arr, int arrLength) {
	int counter = 0;
	for (int i = 0; i < arrLength; i++) {
		if (arr[i] != -1)
			counter++;
	}
	return counter;
}

void TSPGenetic::getPath() {
	for (int i = 0; i < g_size; i++) {
		printf("%hd ", bestPath[i]);
	}
	printf("\n");
}

int *TSPGenetic::greedyPath() {
	int count = 0;
	int *path = new int[g_size], *visited = new int[g_size];
	int current_best, i = 0, next_elem, k = 0;
	for (int i = 0; i < g_size; i++) {
		visited[i] = 0;
	}
	while (count < g_size) {
		next_elem = -1;
		current_best = INT_MAX;
		for (int j = 0; j < g_size; j++) {
			if ((inputGraph->cities[i][j] < current_best) && visited[j] == 0) {
				current_best = inputGraph->cities[i][j];
				next_elem = j;
			}
		}
		visited[next_elem] = 1;
		path[k] = next_elem;
		k++;
		i = next_elem;
		count++;
	}
	return path;
}

#pragma optimize( "", off ) 