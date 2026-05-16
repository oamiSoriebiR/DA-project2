#include <vector>
#include <stack>
#include <set>
#include "Graph.h"
#include "datastructures.h"
#include "interference.h"

// Reset web register assignment
void resetRegisterAssignment(std::vector<Web>& webs){
    for (auto& w : webs) {
        w.assignedRegister = 0;
    }
}

// Function for basic algorithm
bool runBasicAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k){
    // TODO: Implement basic coloring algorithm
    return false;
}


// Auxiliary function to determine next web to be affected
int selectVictim(Graph<int>& graph) {
    auto vertices = graph.getVertexSet();
    int victimId = -1;
    int maxDegree = -1;

    for (auto v : vertices) {
        int id = v->getInfo();
        int degree = v->getAdj().size();
        if (degree > maxDegree || (degree == maxDegree && id > victimId)) {
            maxDegree = degree;
            victimId = v->getInfo();
        }
    }
    return victimId;
}

// Function for spilling algorithm
bool runSpillingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (!runBasicAlgorithm(graph, webs, k)){
        resetRegisterAssignment(webs);

        std::set<int> spilledWebIds;

        // Test until there is no web on graph
        while (graph.getNumVertex() > 0) {
            bool found = false;
            auto vertices = graph.getVertexSet();

            // Find web with degree < k
            for (auto v : vertices) {
                if ((int)v->getAdj().size() < k) {
                    int id = v->getInfo();
                    graph.removeVertex(id);
                    found = true;
                    break; 
                }
            }
            
            // If block, spill highest degree web
            if (!found && graph.getNumVertex() > 0) {
                int victimId = selectVictim(graph);
                if (victimId != -1) {
                    spilledWebIds.insert(victimId);
                    
                    for (auto& w : webs) {
                        if (w.id == victimId) {
                            w.assignedRegister = -1;
                            break;
                        }
                    }
                graph.removeVertex(victimId);
                }
            }
        }

        // Recolor webs that didn't get spilled
        for (auto& currentWeb : webs) {
            if (currentWeb.assignedRegister == -1) {
                continue; 
            }

            // Find colors currently in use by neighboring webs
            std::set<int> neighborColors;
            for (auto& otherWeb : webs) {
                if (otherWeb.id != currentWeb.id && checkInterference(currentWeb, otherWeb)) {
                    if (otherWeb.assignedRegister > 0) {
                        neighborColors.insert(otherWeb.assignedRegister);
                    }
                }
            }

            // Assign first unused color
            bool colored = false;
            for (int c = 1; c <= k; ++c) {
                if (neighborColors.find(c) == neighborColors.end()) {
                    currentWeb.assignedRegister = c;
                    colored = true;
                    break;
                }
            }

            // If still uncolored, spill
            if (!colored) {
                currentWeb.assignedRegister = -1;
                spilledWebIds.insert(currentWeb.id);
            }
        }

        return true;
    } else return true;
}

// Auxiliary function to divide a web in half
bool splitWeb(std::vector<Web>& webs, int victimId) {
    size_t victimIndex = 0;
    bool found = false;
    
    for (size_t i = 0; i < webs.size(); ++i) {
        if (webs[i].id == victimId) {
            victimIndex = i;
            found = true;
            break;
        }
    }

    // If web cannot be split, problem is not solvable with K registers
    if (!found || webs[victimIndex].lines.size() <= 1) {
        return false;
    }

    // Save all data individually instead of using pointers to avoid dangling pointers
    std::string varName = webs[victimIndex].varName;
    std::vector<int> sortedLines(webs[victimIndex].lines.begin(), webs[victimIndex].lines.end());
    size_t mid = sortedLines.size() / 2;

    // Create new web
    Web newWeb;
    newWeb.id = webs.size();
    newWeb.varName = varName;
    newWeb.assignedRegister = 0;
    for (size_t i = mid; i < sortedLines.size(); ++i) {
        newWeb.lines.insert(sortedLines[i]);
    }

    // Refresh original web
    webs[victimIndex].lines.clear();
    for (size_t i = 0; i < mid; ++i) {
        webs[victimIndex].lines.insert(sortedLines[i]);
    }

    webs.push_back(newWeb);
    return true;
}

// Function for splitting algorithm
bool runSplittingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (!runBasicAlgorithm(graph, webs, k)){
        while (true) {
            resetRegisterAssignment(webs);

            Graph<int> newGraph = buildInterferenceGraph(webs);

            // Try to run algorithm again
            if (runBasicAlgorithm(newGraph, webs, k)) {
                return true;
            }

            // If failed, find victim to split
            int victimId = selectVictim(newGraph);

            // If unable to split web, problem is not possible with K registers
            if (!splitWeb(webs, victimId)) return false;
        }
    }
    return true;
}

// Function for custom algorithm (to be designed)
bool runFreeAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (!runBasicAlgorithm(graph, webs, k)){
        resetRegisterAssignment(webs);
        // TODO: Implement free algorithm
        return false;
    }
    return true;
}