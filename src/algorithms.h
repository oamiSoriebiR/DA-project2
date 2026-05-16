#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "datastructures.h"
#include <vector>

void resetRegisterAssignment(std::vector<Web>& webs);

bool runBasicAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

int selectSpillVictim(Graph<int>& graph);
bool runSpillingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

bool runSplittingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

bool runFreeAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

#endif //ALGORITHMS_H