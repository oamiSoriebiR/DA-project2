.PHONY: all build run clean

all: build

build:
	@echo "Building project..."
	g++ -std=c++17 -o allocator src/main.cpp

run: build
	@echo "Running project..."
	./allocator

clean:
	@echo "Cleaning up..."
	# Add your clean commands here
	rm -rf build/ *.o

help:
	@echo "Available targets:"
	@echo "  all     - Build the project"
	@echo "  build   - Compile the project"
	@echo "  run     - Run the project"
	@echo "  clean   - Remove build artifacts"
	@echo "  help    - Show this help message"