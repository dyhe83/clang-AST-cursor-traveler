# C++ version clang AST visitor
A simple C++ version clang AST visitor modified from [screader](https://github.com/shining1984/screader) by [shining1984](https://github.com/shining1984).

## How to build
1. You must install the clang on your computer.
   You can follow the doc: http://clang.llvm.org/get_started.html.
   But after you "make", you must "make install".

```sh
git clone https://github.com/dyhe83/clang-AST-cursor-traveler.git
cd clang-AST-cursor-traveler
sh build.sh
```

## How to use
```sh
cd build
./traveler <filename>            The traveler will output all the AST nodes' information.
./traveler <filename> <keyword>  The traveler will output the AST nodes' matched the keyword.
```

### example
```sh
$ cd build
$ cat ../test/HelloWorld.c
#include <stdio.h>
int main() {
    printf("Hello world.\n");
    return 0;
}


$ ./traveler ../test/HelloWorld.c printf

The AST node kind spelling is: FunctionDecl
The AST node spelling is: printf
Start Line: 318 Column: 1 Offset: 9869
End Line:   318 Column: 57 Offset: 9925
The AST node kind spelling is: CallExpr
The AST node spelling is: printf
Start Line: 4 Column: 5 Offset: 40
End Line:   4 Column: 29 Offset: 64
The AST node kind spelling is: UnexposedExpr
The AST node spelling is: printf
Start Line: 4 Column: 5 Offset: 40
End Line:   4 Column: 11 Offset: 46
The AST node kind spelling is: DeclRefExpr
The AST node spelling is: printf
Start Line: 4 Column: 5 Offset: 40
End Line:   4 Column: 11 Offset: 46
```
