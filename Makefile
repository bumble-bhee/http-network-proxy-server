# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -Iinclude

# Target executable
TARGET = proxy

# Source files
SRC = $(wildcard src/*.cpp)

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean build files
clean:
	rm -f $(TARGET)
