#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "datastructures.h"

void parseRanges(const std::string& input, std::vector<LiveRange> *ranges);
void parseRegisters(const std::string& input, AssignmentConfig *config);

#endif