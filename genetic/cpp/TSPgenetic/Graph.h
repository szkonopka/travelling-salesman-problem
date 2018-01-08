#pragma once
#include <string>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp" 
#include <memory> 
using namespace rapidxml;


class Graph
{
friend class TSPGenetic;
protected:
	int size;
	int **cities;
public:
	Graph();
	Graph(int s);
	~Graph();

	void readFromTXTFileSym(std::string fileName);
	void readFromTXTFileAsym(std::string fileName);

	void fillSymmetricAuto();
	void fillAsymmetricAuto();

	void display();
	bool readDataParser(std::string fileName, int format);
};

