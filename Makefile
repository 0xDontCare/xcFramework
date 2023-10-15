# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g -Iinclude

# Source and object files
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

# Executable name
EXEC = bin/main
DEPS = $(wildcard include/*.h)

# Test files and executables
TESTS = $(wildcard tests/*.c)
TESTS_EXEC = $(TESTS:tests/%.c=bin/test_%)

# Default target
all: $(EXEC) $(TESTS_EXEC)

# Creating executable from object files
$(EXEC): $(OBJ)
	@echo "Linking $@ ..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Done"

# Creating object files from source files
obj/%.o: src/%.c $(DEPS)
	@echo "Compiling $< ..."
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo "Done"

# Creating test executables from test files
bin/test_%: tests/%.c $(DEPS) $(OBJ)
	@echo "Compiling and linking $< ..."
	$(eval TEST_OBJ := $(filter-out obj/main.o, $(OBJ))) 
	$(CC) $(CFLAGS) -o $@ $< $(TEST_OBJ)
	@echo "Done"

# Cleaning object files and executable
clean:
	@rm -rf ./bin/*

.PHONY: all