// src/algorithms.h
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "datastructures.h"
#include <vector>
/**
 * @brief Resets the assigned registers of all webs to 0.
 * @param webs Vector of Web structures to reset.
 * @note Time Complexity: O(W), where W is the number of webs.
 */
void resetRegisterAssignment(std::vector<Web>& webs);

/**
 * @brief Runs the basic register allocation algorithm using graph coloring.
 * It attempts to color the graph without any spilling or splitting.
 * @param graph The interference graph representing the webs.
 * @param webs Vector of Web structures to allocate registers for.
 * @param k The number of available registers (colors).
 * @return True if allocation succeeded without spilling, false otherwise.
 * @note Time Complexity: O(V^2 + V*k), where V is the number of vertices (webs) and k is the number of registers.
 */
bool runBasicAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

/**
 * @brief Selects the best victim web to spill based on a heuristic metric (degree / web size).
 * @param graph The interference graph.
 * @param webs Vector of Web structures.
 * @return The ID of the web chosen to be spilled.
 * @note Time Complexity: O(V * W), where V is the number of vertices and W is the number of webs.
 */
int selectSpillVictim(Graph<int>& graph, const std::vector<Web>& webs);

/**
 * @brief Runs the register allocation algorithm with support for spilling to memory.
 * If the basic algorithm fails, it iteratively spills the most suitable webs until the graph is colorable.
 * @param graph The interference graph representing the webs.
 * @param webs Vector of Web structures to allocate registers for.
 * @param k The number of available registers (colors).
 * @return Always returns true as uncolorable webs are spilled to memory.
 * @note Time Complexity: O(V^2 * W + W^2 * L), where V is the number of vertices, W is the number of webs, and L is the max number of lines in a web.
 */
bool runSpillingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

/**
 * @brief Runs the register allocation algorithm with support for web splitting.
 * If the graph cannot be colored, it attempts to split webs into smaller segments to reduce interference.
 * @param graph The interference graph.
 * @param webs Vector of Web structures.
 * @param k The number of available registers.
 * @return True if allocation succeeded, either natively or by successfully splitting webs.
 * @note Time Complexity: O(C * (W^2 * L + V^2)), where C is a constant max splits, W is the number of webs, L is the max lines per web, and V is the number of vertices.
 */
bool runSplittingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

/**
 * @brief Splits a web into two smaller webs at the point where interference ends, to increase colorability.
 * @param webs The vector of Web structures. The newly created web fragment is appended to this list.
 * @param victimId The ID of the web to split.
 * @return True if the web was successfully split, false if it could not be split (e.g., only 1 line).
 * @note Time Complexity: O(W * L * log(L)), where W is the number of webs and L is the number of lines in the victim web.
 */
bool splitWeb(std::vector<Web>& webs, int victimId);

/**
 * @brief Selects the best victim web to split based on a heuristic metric (maximum degree).
 * @param graph The interference graph.
 * @return The ID of the web chosen to be split.
 * @note Time Complexity: O(V), where V is the number of vertices in the interference graph.
 */
int selectSplittingVictim(Graph<int>& graph);

/**
 * @brief Runs an advanced/free algorithm combining splitting with dynamic thresholds and spilling as a fallback.
 * @param graph The interference graph.
 * @param webs Vector of Web structures.
 * @param k The number of available registers.
 * @return True if the process completed successfully.
 * @note Time Complexity: O(C * (W^2 * L + V^2)), where C is a constant max splits, W is the number of webs, L is the max lines per web, and V is the number of vertices.
 */
bool runFreeAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k);

#endif //ALGORITHMS_H