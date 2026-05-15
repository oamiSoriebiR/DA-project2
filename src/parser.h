#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "datastructures.h"

std::vector<LiveRange> parseRanges(const std::string& input);
AssignmentConfig parseRegisters(const std::string& input);

#endif //PARSER_H