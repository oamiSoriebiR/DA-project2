#ifndef INTERFERENCE_H
#define INTERFERENCE_H

#include "Graph.h"
#include "datastructures.h"
#include <vector>

/**
 * @brief Checks if there is an interference (overlapping live ranges) between two webs.
 * Uses a two-pointer approach since the lines are stored in ordered sets.
 * @param w1 The first Web.
 * @param w2 The second Web.
 * @return True if they interfere, false otherwise.
 * @note Time Complexity: O(L1 + L2), where L1 and L2 are the number of lines in w1 and w2.
 */
bool checkInterference(const Web& w1, const Web& w2);

/**
 * @brief Builds the interference graph from a list of webs.
 * Creates an undirected graph where vertices are web IDs and edges represent interferences.
 * @param webs Vector of Web structures.
 * @return The constructed interference graph.
 * @note Time Complexity: O(W^2 * L), where W is the number of webs and L is the maximum number of lines in a web.
 */
Graph<int> buildInterferenceGraph(const std::vector<Web>& webs);

#endif //INTERFERENCE.H