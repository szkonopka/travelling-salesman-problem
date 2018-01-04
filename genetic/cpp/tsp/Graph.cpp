#include "Graph.h"

Graph::Graph() {}

Graph::Graph(int size)
{
	this->size = size;
	cities = new int*[size];
	for (int i = 0; i < size; i++) {
		cities[i] = new int[size];
	}
}

Graph::~Graph() {}

void Graph::readFromFileSym(std::string fileName)
{
	std::string path = "data/";
	path += fileName;
	std::fstream file;
	file.open(path, std::ios::in);
	if (file.good()) {
		short tempValue;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j <= i; j++) {
				file >> tempValue;
				cities[i][j] = cities[j][i] = tempValue;
			}
		}
	}
	file.close();
}

void Graph::readFromFileAsym(std::string fileName) {
	std::string path = "data/";
	path += fileName;
	std::fstream file;
	file.open(path, std::ios::in);
	if (file.good()) {
		short tempValue;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				file >> tempValue;
				if (tempValue == 9999) {
					cities[i][j] = 0;
				}
				else
					cities[i][j] = tempValue;
			}
		}
	}
	file.close();
}

void Graph::fillSymmetricAuto() {
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j <= i; j++) {
			if (i != j)
				cities[i][j] = cities[j][i] = (rand() % 99) + 1;
			else
				cities[i][j] = 0;
		}
	}
}

void Graph::fillAsymmetricAuto() {
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j)
				cities[i][j] = (rand() % 99) + 1;
			else
				cities[i][j] = 0;

		}
	}
}

void Graph::display() {
	for (int i = 0; i < size; i++) {
		printf("\n");
		for (int j = 0; j < size; j++) {
			printf("%d ", cities[i][j]);;
		}
	}
	printf("\n");
}


bool Graph::readData(std::string fileName, int format) {
	if (format == 0) {
		fileName = fileName + ".xml";
		std::ifstream file(fileName.c_str());
		if (!file) {
			return false;
		}
		file.seekg(0, file.end);
		const auto fileSize = file.tellg();
		file.seekg(0);

		std::unique_ptr < char[] > xmlFileContext(new char[(unsigned int)(fileSize + static_cast < decltype(fileSize) >(1))]);
		file.read(xmlFileContext.get(), fileSize);

		xml_document <> document;
		document.parse < 0 >(xmlFileContext.get()); // <0>

		xml_node <>* startNode = document.first_node();

		xml_node <>* transitionNode = startNode->first_node();
		for (int a = 0; a < 5; a++, transitionNode = transitionNode->next_sibling()) {}
		xml_node <>* resizingOperation = transitionNode->first_node()->last_node();
		size = atoi(resizingOperation->value()) + 1;
		cities = new int*[size];
		for (int i = 0; i < size; i++) {
			cities[i] = new int[size];
		}
		int i = 0;
		for (xml_node <>* vertex = transitionNode->first_node(); vertex; i++, vertex = vertex->next_sibling()) { //vertex
			for (xml_node <>* edge = vertex->first_node(); edge; edge = edge->next_sibling()) { // edge
				cities[i][atoi(edge->value())] = (int)atof(edge->first_attribute()->value());
			}
			cities[i][i] = 0;
		}
		file.close();
		return true;
	}
	else {
		std::ifstream file;
		fileName = fileName + ".txt";
		file.open(fileName);

		if (file.good()) {
			file >> size;
			cities = new int*[size];
			for (int i = 0; i < size; i++) {
				cities[i] = new int[size];
			}
			for (int i = 0; i < size; i++) {
				for (int n = 0; n < size; n++) {
					file >> cities[i][n];
				}
			}
			size = 0;

		}
		else {
			return false;
		}
		file.close();
		return true;
	}
}