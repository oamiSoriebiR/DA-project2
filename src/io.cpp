#include "io.h"
#include <iostream>
#include <fstream>
#include <set>

bool writeAllocationOutput(const std::string& outputPath, const std::vector<Web>& webs, const AssignmentConfig& config, bool allocationSucceeded) {
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open output file " << outputPath << std::endl;
        return false;
    }

    // 1. Print the total number of webs
    file << "webs: " << webs.size() << "\n";

    // 2. Print each web's details (preserving formatting expectations)
    for (const auto& w : webs) {
        file << "web" << w.id << ": ";
        
        // Loop through lines in the set and format them
        size_t count = 0;
        for (int line : w.lines) {
            file << line;
            
            // Note: If  parser strips the '+' and '-',  add them back:
            // if (count == 0) file << "+"; // Usually definition
            // if (count == w.lines.size() - 1) file << "-"; // Last use
            
            if (++count < w.lines.size()) {
                file << ",";
            }
        }
        file << "\n";
    }

    // 3. Print the number of physical hardware registers utilized
    if (allocationSucceeded) {
        file << "registers: " << config.k << "\n";
        
        // Print the register mapping
        for (const auto& w : webs) {
            if (w.assignedRegister != -1) {
                file << "r" << w.assignedRegister << ": web" << w.id << "\n";
            } else {
                file << "M: web" << w.id << "\n";
            }
        }
    } else {
        // If basic allocation fails completely, N = 0 registers used, everything maps to memory
        file << "registers: 0\n";
        for (const auto& w : webs) {
            file << "M: web" << w.id << "\n";
        }
        
        // Print the warning to standard error stream 
        std::cerr << "Warning: Register allocation to the provided number of registers (" 
                  << config.k << ") was not possible.\n";
    }

    file.close();
    return true;
}