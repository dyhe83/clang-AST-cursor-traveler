//
// Created by dongying on 2/12/19.
//

#ifndef TRAVELER_NODE_HPP
#define TRAVELER_NODE_HPP

#include <iostream>

#include <clang-c/Index.h>

using namespace std;

class Node {
private:
	unsigned int staLine, endLine;
	unsigned int staIndex, endIndex;
	unsigned int staOffset, endOffset;
	string kindSpelling, content;

public:
	explicit Node(CXCursor);

	void printInfo();

	string getKindSpelling();

	string getContent();
};


#endif //TRAVELER_NODE_HPP
