CXX = g++
CXX_FLAGS = -Wall -Wextra -std=c++17
TARGET_DIR = out
TARGET_BIN = game
SRC = main.cpp

all:
	mkdir -p $(TARGET_DIR)
	$(CXX) $(CXX_FLAGS) $(SRC) -o $(TARGET_DIR)/$(TARGET_BIN)

clean:
	rm -f $(TARGET_DIR)

