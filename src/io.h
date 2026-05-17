#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "datastructures.h"

/**
 * @brief Generates the human-readable allocation map and writes it to the specified file path.
 * @param outputPath The path where the output file will be created.
 * @param webs The list of webs that have been processed and allocated.
 * @param config The assignment configuration containing the number of available registers.
 * @param allocationSucceeded Boolean indicating whether the allocation process was successful.
 * @return True if the file was written successfully, false otherwise.
 * @note Time Complexity: O(W * L), where W is the number of webs and L is the maximum number of lines per web.
 */
bool writeAllocationOutput(const std::string& outputPath, const std::vector<Web>& webs, const AssignmentConfig& config, bool allocationSucceeded);

#endif 