CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = string_matching
SOURCE = string_matching.cpp

.PHONY: all clean run test help

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)
	@echo "Build successful! Run with: ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Running test with overlapping pattern AAA..."
	@(echo "1"; echo "input.txt"; echo "3"; echo "AAA"; echo "6") | ./$(TARGET) | head -30
	@echo ""
	@echo "Running test with pattern DATA..."
	@(echo "1"; echo "input.txt"; echo "4"; echo "DATA"; echo "6") | ./$(TARGET) | head -30

clean:
	rm -f $(TARGET)
	@echo "Cleaned build artifacts"

help:
	@echo "Available targets:"
	@echo "  make         - Compile the program"
	@echo "  make run     - Compile and run the program"
	@echo "  make test    - Run automated tests"
	@echo "  make clean   - Remove compiled files"
	@echo "  make help    - Show this help message"
