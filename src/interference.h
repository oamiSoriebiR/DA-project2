#ifndef INTERFERENCE_H
#define INTERFERENCE_H

#include "Graph.h"
#include "datastructures.h"
#include <vector>

bool checkInterference(const Web& w1, const Web& w2);
Graph<int> buildInterferenceGraph(const std::vector<Web>& webs);

#endif //INTERFERENCE.H