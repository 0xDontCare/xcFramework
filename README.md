# xcFramework
The "eXtra C Framework" includes a set of commonly used data structures and functions implemented in C language. The main goal of this project is to provide a simple, easy-to-use and lightweight library that could be used in various projects. The library also aims to (eventually) provide complete alternative to the standard C library functions while keeping same abstraction from the operating system (compatible on Windows and Unix-like platforms) and providing additional features.

## How to use:
1. Clone the repository
2. Build the library using provided `Makefile`
3. Include the header files in your project
4. Link the library to your project

## How to build:
1. Run `make` in the root directory of the project
2. The library will be built in the `lib` directory along with test executables for each module in the `build` directory
3. For cleaning the project run `make clean`

## How to test:
1. Run the test executables in the `build` directory
2. The test executables are named after the module they test

## Available modules:
- Memory copying, comparing and hashing functions (`xMemtools.h`)
- Safer string type along with its functions and copy-on-write mechanism (`xString.h`)
- Dynamic generic array implementation (`xArray.h`)
- Deferrable function calls module (`xDefer.h`)
- Mathematical matrix operations module (`xMatrix.h`)
- Dynamic generic stack implementation (`xStack.h`)
### Listed modules are tested and ready for use in projects

## Experimental modules (lacking tests, documentation or are incomplete):
- Dynamic generic linked list implementation (`xList.h`)
- Dynamic generic treemap implementation (`xDictionary.h`)
- Dynamic generic queue implementation (`xQueue.h`)
- Custom memory allocation module (`xAlloc.h`)
### These modules are available in respective `dev-X` branches, bugs and issues are expected until proper testing is done

## Planned modules (could be implemented in the future):
- Hash table implementation
- Priority queue implementation
- File I/O module
- Directory manipulation module
- I/O for `xString` module (both file and console)
- Command line argument parsing module
- Logging module (with different log levels)
- SIMD operations for `xMatrix` and other applicable modules
### List is subject to change and does not represent the order in which modules will be implemented
