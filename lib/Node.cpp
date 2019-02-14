#include "Node.hpp"

Node::Node(CXCursor cursor) {
	kindSpelling = clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(cursor)));
	content = clang_getCString(clang_getCursorSpelling(cursor));

	CXSourceRange range = clang_getCursorExtent(cursor);
	clang_getInstantiationLocation(clang_getRangeStart(range), nullptr, &staLine, &staIndex, &staOffset);
	clang_getInstantiationLocation(clang_getRangeEnd(range), nullptr, &endLine, &endIndex, &endOffset);
}

void Node::printInfo() {
	cout << "The AST node kind spelling is: " << kindSpelling << endl;
	cout << "The AST node spelling is: " << content << endl;
	cout << "Start Line: " << staLine << " Column: " << staIndex << " Offset: " << staOffset << endl;
	cout << "End Line:   " << endLine << " Column: " << endIndex << " Offset: " << endOffset << endl;
}

string Node::getKindSpelling() {
	return kindSpelling;
}

string Node::getContent() {
	return content;
}
