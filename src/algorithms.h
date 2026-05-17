// src/algorithms.h
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "datastructures.h"
#include <vector>

void resetRegisterAssignment(std::vector<Web>& webs);
bool runBasicAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

int selectSpillVictim(Graph<int>& graph, const std::vector<Web>& webs);
bool runSpillingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

bool splitWeb(std::vector<Web>& webs, int victimId);
int selectSplittingVictim(Graph<int>& graph);
bool runSplittingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

bool runFreeAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

#endif //ALGORITHMS_H