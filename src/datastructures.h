#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <set>

/**
 * @brief Represents the configuration for the register assignment process.
 */
struct AssignmentConfig {
    int k = 0;                  /**< @brief Number of available registers. */
    std::string algorithm = ""; /**< @brief Algorithm to use: "basic", "spilling", "splitting", or "free". */
};

/**
 * @brief Represents a live range of a variable.
 */
struct LiveRange {
    std::string varName; /**< @brief Name of the variable. */
    std::set<int> lines; /**< @brief Set of lines where the variable is live, kept sorted to avoid duplicates. */
};

/**
 * @brief Represents a Web, which is an independent live range of a variable.
 */
struct Web {
    int id;                   /**< @brief Unique identifier for the web. */
    std::string varName;      /**< @brief Name of the variable associated with the web. */
    std::set<int> lines;      /**< @brief Set of lines where the web is live. */
    int assignedRegister = 0; /**< @brief Assigned register ID (-1 = Spilled to Memory, 0 = Unassigned). */
};

#endif //DATASTRUCTURES_H