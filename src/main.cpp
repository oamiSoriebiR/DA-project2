#include <string>
#include <iostream>
#include <fstream>
#include "datastructures.h"
#include "parser.h"

void runMenu();
bool fileExists(const std::string& filename);
int runAllocation(const std::string& rangeFile, const std::string& registersFile, const std::string& allocationFile);

std::string range;
std::string registers;
std::string allocation;

std::vector<LiveRange> ranges;
AssignmentConfig config;


int main(int argc, char* argv[]) {
    
    // CLI mode
    if(argc == 1){
        runMenu();
        return 0;
    }
    // Batch mode "myProg -b ranges.txt registers.txt allocation.txt"
    range = argv[2];
    registers = argv[3];
    allocation = argv[4];

    if (!fileExists(range) || !fileExists(registers)) {
        std::cout << "One or more input files do not exist. Please provide valid files." << std::endl;
        return 1;
    }

    runAllocation(range, registers, allocation);

    return 0;
}

void runMenu() {
    std::cout << "Compiler Register Allocation" << std::endl;
    std::cout << "1. Range" << std::endl;
    std::cout << "2. Registers" << std::endl;
    std::cout << "3. Select allocation output file" << std::endl;
    std::cout << "4. Run allocation" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "Select an option: ";

    int choice;
    std::cin >> choice;
    std::string range;
    std::string registers;
    std::string allocation;
    
    switch (choice) {
        case 1:
            std::cout << "Range selection" << std::endl;
            std::cout << "Enter the path for the range file: ";
            std::cin >> range;
            if (!fileExists(range)) {
                std::cout << "File does not exist. Please select a valid file." << std::endl;
                range.clear();
            }
            break;
        case 2:
            std::cout << "Registers selection" << std::endl;
            std::cout << "Enter the path for the registers file: ";
            std::cin >> registers;
            if (!fileExists(registers)) {
                std::cout << "File does not exist. Please select a valid file." << std::endl;
                registers.clear();
            }
            break;
        case 3:
            std::cout << "Allocation selection" << std::endl;
            std::cout << "Enter the path for the allocation output file: ";
            std::cin >> allocation;
            if (!fileExists(allocation)) {
                std::cout << "File does not exist. Please select a valid file." << std::endl;
                allocation.clear();
            }
            break;
        case 4:
            std::cout << "Run allocation" << std::endl;
            if (range.empty() || registers.empty() || allocation.empty()) {
                std::cout << "Please select the range, registers and allocation output file before running the allocation." << std::endl;
                break;
            }
            runAllocation(range, registers, allocation);
            break;
        case 5:
            std::cout << "Exiting..." << std::endl;
            exit(0);
        default:
            std::cout << "Invalid option" << std::endl;
    }
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int runAllocation(const std::string& rangeFile, const std::string& registersFile, const std::string& allocationFile) {
    std::cout << "Running allocation with:" << std::endl;
    std::cout << "Range file: " << rangeFile << std::endl;
    std::cout << "Registers file: " << registersFile << std::endl;
    std::cout << "Allocation output file: " << allocationFile << std::endl;
    

    ranges = parseRanges(rangeFile);
    config = parseRegisters(registersFile);
    
    std::cout << "Algorithm: " << config.algorithm << std::endl;
}