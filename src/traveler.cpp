#include <iostream>
#include <string>
#include <clang-c/Index.h>
#include <assert.h>

using namespace std;

class Node{
private:
	unsigned int staLine, endLine;
	unsigned int staIndex, endIndex;
	unsigned int staOffset, endOffset;
	string kindSpelling, content;

public:
	Node(CXCursor cursor) {
		kindSpelling = clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(cursor)));
		content = clang_getCString(clang_getCursorSpelling(cursor));

		CXSourceRange range = clang_getCursorExtent(cursor);
		clang_getInstantiationLocation(clang_getRangeStart(range), nullptr, &staLine, &staIndex, &staOffset);
		clang_getInstantiationLocation(clang_getRangeEnd(range), nullptr, &endLine, &endIndex, &endOffset);
	}

	void printInfo() {
		cout << "The AST node kind spelling is: " << kindSpelling << endl;
		cout << "The AST node spelling is: " << content << endl;
		cout << "Start Line: " << staLine << " Column: " << staIndex << " Offset: "<< staOffset << endl;
		cout << "End Line:   " << endLine << " Column: " << endIndex << " Offset: "<< endOffset << endl;
	}

	string getKindSpelling() {
		return kindSpelling;
	}

	string getContent() {
		return content;
	}
};

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
	if (fp == NULL) {
		cerr << "Can't open the file: " << argv[1] << endl;
		assert(0);
	}
	fclose(fp);

	numOfArgc = argc;
	if (numOfArgc == 3) {
		findingString = argv[2];
	}

	// get cursor
	CXIndex Index = clang_createIndex(0,0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, 0, argv, argc, 0, 0, CXTranslationUnit_None);
	CXCursor cursor = clang_getTranslationUnitCursor(TU);

	// visit AST
	clang_visitChildren(cursor, printVisitor, NULL);

	// dispose clang CXTranslationUnit & CXIndex
	clang_disposeTranslationUnit(TU);
	clang_disposeIndex(Index);
	return 0;
}
