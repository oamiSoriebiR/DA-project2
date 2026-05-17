#ifndef WEB_H
#define WEB_H

#include <vector>
#include "datastructures.h"

/**
 * @brief Checks if two live ranges intersect (share at least one line).
 * @param r1 The first live range.
 * @param r2 The second live range.
 * @return True if they intersect, false otherwise.
 * @note Time Complexity: O(L1 + L2), where L1 and L2 are the number of lines in each live range.
 */
bool rangesIntersect(const LiveRange& r1, const LiveRange& r2);

/**
 * @brief Groups intersecting live ranges of the same variable into independent Webs.
 * @param ranges A vector of all parsed live ranges.
 * @return A vector of constructed Web structures.
 * @note Time Complexity: O(R^2 * L), where R is the number of ranges and L is the max lines per range.
 */
std::vector<Web> webLinking(std::vector<LiveRange> ranges);

#endif //WEB_H