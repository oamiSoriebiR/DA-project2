#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "datastructures.h"

// Generates the human-readable allocation map to the specified file path
bool writeAllocationOutput(const std::string& outputPath, const std::vector<Web>& webs, const AssignmentConfig& config, bool allocationSucceeded);

#endif 