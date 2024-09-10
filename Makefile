# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -Wshadow -Wstrict-overflow -std=gnu11 -Iinclude
AR = llvm-ar
ARFLAGS = rcs

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build
LIB_DIR = lib

# Library name
LIB_NAME = libxcFramework.a

# Source files
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Test files
TEST_FILES = $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJ_FILES = $(TEST_FILES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_EXECUTABLES = $(TEST_FILES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

# Targets
.PHONY: all clean

all: $(LIB_DIR)/$(LIB_NAME) $(TEST_EXECUTABLES)

# Build static library
$(LIB_DIR)/$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executables
$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(LIB_DIR)/$(LIB_NAME)
	$(CC) $< -L$(LIB_DIR) -lxcFramework -o $@ -lcunit

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)