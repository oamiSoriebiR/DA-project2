.PHONY: all build run clean presentation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
TARGET = allocator
SRCS = src/main.cpp src/parser.cpp src/web.cpp src/interference.cpp src/algorithms.cpp src/io.cpp

all: build

build:
	@echo "Building project..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: build
	@echo "Running project..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET)
	rm -f *.o
	rm -f src/*.o
	rm -f presentation/*.aux presentation/*.log presentation/*.nav presentation/*.out presentation/*.snm presentation/*.toc presentation/*.pdf

presentation:
	@echo "Building presentation..."
	cd presentation && pdflatex presentation.tex

help:
	@echo "Available targets:"
	@echo "  all          - Build the project"
	@echo "  build        - Compile the project"
	@echo "  run          - Run the project"
	@echo "  presentation - Build the LaTeX presentation"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"