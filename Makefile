SRC_DIR = .
BIN_DIR = ./bin
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
BINS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRCS))

.PHONY: all clean
all: $(BINS)

CC = g++
CXXFLAGS = -fdiagnostics-color=always -std=c++17 -g3 -ggdb -Wall -Wextra

$(BIN_DIR):
	mkdir -p $@

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp $(SRC_DIR)/AVL_Tree.h | $(BIN_DIR)
	$(CC) $(CXXFLAGS) $< -o $@

clean: 
	rm -rf $(BIN_DIR)/*