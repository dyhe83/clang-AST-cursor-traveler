#include <iostream>
#include <clang-c/Index.h>
#include <cassert>
#include "Node.hpp"

using namespace std;

string findingString;
int numOfArgc = 0;

enum CXChildVisitResult printVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	Node node(cursor);
	if (numOfArgc == 2) {
		node.printInfo();
	} else if (numOfArgc == 3) {
		if (findingString == node.getKindSpelling() || findingString == node.getContent()) {
			node.printInfo();
		}
	}

	return CXChildVisit_Recurse;
}

int main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) {
		cerr << "You input wrong number arguments!" << endl;
		assert(0);
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == nullptr) {
		cerr << "Can't open the file: " << argv[1] << endl;
		assert(0);
	}
	fclose(fp);

	numOfArgc = argc;
	if (numOfArgc == 3) {
		findingString = argv[2];
	}

	// get cursor
	CXIndex Index = clang_createIndex(0, 0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, nullptr, argv, argc, nullptr, 0, CXTranslationUnit_None);
	CXCursor cursor = clang_getTranslationUnitCursor(TU);

	// visit AST
	clang_visitChildren(cursor, printVisitor, nullptr);

	// dispose clang CXTranslationUnit & CXIndex
	clang_disposeTranslationUnit(TU);
	clang_disposeIndex(Index);
	return 0;
}
