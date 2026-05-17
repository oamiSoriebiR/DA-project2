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

    // Print the total number of webs
    file << "webs: " << webs.size() << "\n";

    // Print each web's details (preserving formatting expectations)
    for (const auto& w : webs) {
        file << "web" << w.id << ": ";
        
        // Loop through lines in the set and format them
        size_t count = 0;
        for (int line : w.lines) {
            file << line;

            // Reassing '+' & '-' symbols
            if (count == 0) file << "+"; 
            if (count == w.lines.size() - 1) file << "-";
            
            if (++count < w.lines.size()) {
                file << ",";
            }
        }
        file << "\n";
    }

    // Print the number of physical hardware registers utilized
    if (allocationSucceeded) {
        file << "registers: " << config.k << "\n";
        
        // Print the register mapping
        for (const auto& w : webs) {
            if (w.assignedRegister != -1) {
                file << "r" << w.assignedRegister - 1 << ": web" << w.id << "\n";
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