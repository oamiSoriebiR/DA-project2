#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "datastructures.h"

/**
 * @brief Parses live ranges from a given input file.
 * Reads the file line by line, extracting the variable name and the lines where it's live.
 * @param input The path to the input file containing live range data.
 * @return A vector of parsed LiveRange structures.
 * @note Time Complexity: O(N), where N is the total number of characters in the file.
 */
std::vector<LiveRange> parseRanges(const std::string& input);

/**
 * @brief Parses the register assignment configuration from a given file.
 * Reads the available number of registers (k) and the chosen algorithm.
 * @param input The path to the input file containing configuration data.
 * @return An AssignmentConfig structure containing the parsed settings.
 * @note Time Complexity: O(N * logL), where N is the total number of characters in the file & L is the maximum nunber of lines in a range.
 */
AssignmentConfig parseRegisters(const std::string& input);

#endif //PARSER_H