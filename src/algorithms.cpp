#include <vector>
#include <stack>
#include <set>
#include <map>
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
    resetRegisterAssignment(webs);
    
    std::stack<int> S;
    std::map<int, int> degrees;
    std::set<int> activeNodes;
    std::map<int, int> assignedColors; // webId -> assigned color (1 to k)

    for (auto v : graph.getVertexSet()) {
        int id = v->getInfo();
        activeNodes.insert(id);
        degrees[id] = v->getAdj().size();
        assignedColors[id] = 0; // 0 means unassigned
    }

    bool spillOccurred = false;

    // Simplification
    while (!activeNodes.empty()) {
        int nodeToRemove = -1;

        for (int id : activeNodes) {
            if (degrees[id] < k) {
                nodeToRemove = id;
                break;
            }
        }

        if (nodeToRemove == -1) {
            spillOccurred = true;

            int maxDegree = -1;
            int victimId = -1;
            for (int id : activeNodes) {
                if (degrees[id] > maxDegree || (degrees[id] == maxDegree && id > victimId)) {
                    maxDegree = degrees[id];
                    victimId = id;
                }
            }
            nodeToRemove = victimId;
        } else {
            S.push(nodeToRemove);
        }

        activeNodes.erase(nodeToRemove);
        auto v = graph.findVertex(nodeToRemove);
        if (v != nullptr) {
            for (const auto& edge : v->getAdj()) {
                int neighborId = edge.getDest()->getInfo();
                if (activeNodes.count(neighborId) > 0) {
                    degrees[neighborId]--;
                }
            }
        }
    }

    // Selection
    while (!S.empty()) {
        int id = S.top();
        S.pop();

        std::vector<bool> availableColors(k + 1, true);

        auto v = graph.findVertex(id);
        if (v != nullptr) {
            for (const auto& edge : v->getAdj()) {
                int neighborId = edge.getDest()->getInfo();
                int neighborColor = assignedColors[neighborId];
                if (neighborColor >= 1 && neighborColor <= k) {
                    availableColors[neighborColor] = false;
                }
            }
        }

        int chosenColor = -1;
        for (int c = 1; c <= k; ++c) {
            if (availableColors[c]) {
                chosenColor = c;
                break;
            }
        }

        if (chosenColor != -1) {
            assignedColors[id] = chosenColor;
        } else {
            return false;
        }
    }

    for (auto& w : webs) {
        if (assignedColors[w.id] == 0) {
            w.assignedRegister = -1; 
        } else {
            w.assignedRegister = assignedColors[w.id];
        }
    }

    return !spillOccurred;
}

// Auxiliary function to determine next web to be affected
int selectSpillVictim(Graph<int>& graph, const std::vector<Web>& webs) {
    auto vertices = graph.getVertexSet();
    int victimId = -1;
    double maxMetric = -1.0;

    for (auto v : vertices) {
        int id = v->getInfo();
        int degree = v->getAdj().size();
        
        // Find the corresponding web to get its live range length
        size_t webSize = 1; // Default fallback to avoid division by zero
        for (const auto& w : webs) {
            if (w.id == id) {
                if (!w.lines.empty()) {
                    webSize = w.lines.size();
                }
                break;
            }
        }

        // Calculate the Spill Metric
        double metric = static_cast<double>(degree) / webSize;

        // Select the vertex with the maximum metric
        if (metric > maxMetric || (metric == maxMetric && id > victimId)) {
            maxMetric = metric;
            victimId = id;
        }
    }
    return victimId;
}

// Function for spilling algorithm
bool runSpillingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (!runBasicAlgorithm(graph, webs, k)){
        resetRegisterAssignment(webs);

        std::set<int> spilledWebIds;

        // Test until there is no web on the graph
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
            
            // If blocked, spill the web with the highest Spill Metric
            if (!found && graph.getNumVertex() > 0) {
                int victimId = selectSpillVictim(graph, webs);
                if (victimId != -1) {
                    spilledWebIds.insert(victimId);
                    
                    for (auto& w : webs) {
                        if (w.id == victimId) {
                            w.assignedRegister = -1; // Mark as spilled to memory
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

            // Assign the first unused color
            bool colored = false;
            for (int c = 1; c <= k; ++c) {
                if (neighborColors.find(c) == neighborColors.end()) {
                    currentWeb.assignedRegister = c;
                    colored = true;
                    break;
                }
            }

            // If still uncolored, spill it
            if (!colored) {
                currentWeb.assignedRegister = -1;
                spilledWebIds.insert(currentWeb.id);
            }
        }

        return true;
    } else return true;
}

// Auxiliary function to divide a web exactly where the interference bottleneck ends
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

    if (!found || webs[victimIndex].lines.size() <= 1) {
        return false;
    }

    const auto& victimWeb = webs[victimIndex];
    
    // Track which lines in this web actually have an interference conflict
    std::set<int> conflictingLines;
    for (int line : victimWeb.lines) {
        for (const auto& otherWeb : webs) {
            if (otherWeb.id != victimId && otherWeb.lines.count(line) > 0) {
                conflictingLines.insert(line);
                break; // Move to the next line once a conflict is found
            }
        }
    }

    std::set<int> preBottleneckLines;
    std::set<int> postBottleneckLines;

    // If there are conflicting lines, find the point where they end!
    if (!conflictingLines.empty()) {
        // Get the absolute last line that has an active conflict
        int lastConflictLine = *conflictingLines.rbegin();

        for (int line : victimWeb.lines) {
            if (line <= lastConflictLine) {
                // Keep the entire conflict zone together
                preBottleneckLines.insert(line);
            } else {
                // Everything after the conflict zone clears out into the safe zone
                postBottleneckLines.insert(line);
            }
        }
    }

    //  If the conflict spans the entire web, or if the slice results
    //  in an empty side, fall back cleanly to the midpoint.
    if (preBottleneckLines.empty() || postBottleneckLines.empty()) {
        std::vector<int> sortedLines(victimWeb.lines.begin(), victimWeb.lines.end());
        size_t mid = sortedLines.size() / 2;
        
        preBottleneckLines.clear();
        postBottleneckLines.clear();
        
        for (size_t i = 0; i < mid; ++i) preBottleneckLines.insert(sortedLines[i]);
        for (size_t i = mid; i < sortedLines.size(); ++i) postBottleneckLines.insert(sortedLines[i]);
    }

    // Create the new derived safe-zone web fragment
    Web newWeb;
    newWeb.id = webs.size();
    newWeb.varName = victimWeb.varName;
    newWeb.assignedRegister = 0;
    newWeb.lines = postBottleneckLines;

    // Retain only the conflict zone lines in the original web entry
    webs[victimIndex].lines = preBottleneckLines;

    webs.push_back(newWeb);
    return true;
}

int selectSplittingVictim(Graph<int>& graph) {
    auto vertices = graph.getVertexSet();
    int victimId = -1;
    int maxDegree = -1;
    for (auto v : vertices) {
        int id = v->getInfo();
        int degree = v->getAdj().size();
        if (degree > maxDegree || (degree == maxDegree && id > victimId)) {
            maxDegree = degree;
            victimId = id;
        }
    }
    return victimId;
}

// Function for splitting algorithm 
bool runSplittingAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (runBasicAlgorithm(graph, webs, k)){
        return true;
    }

    int maxSplits = 30;
    int splitsDone = 0;

    while (splitsDone < maxSplits) {
        resetRegisterAssignment(webs);
        Graph<int> newGraph = buildInterferenceGraph(webs);

        // Try to run basic algorithm 
        if (runBasicAlgorithm(newGraph, webs, k)) {
            return true;
        }

        // If it failed, find victim to split
        int victimId = selectSplittingVictim(newGraph);

        // If splitWeb returns false, the victim cannot be divided anymore
        // Break out to save and color the remaining segments
        if (!splitWeb(webs, victimId)) {
            break;
        }
        
        splitsDone++;
    }

    // Final allocation attempt to preserve color mappings for valid slices
    resetRegisterAssignment(webs);
    Graph<int> finalGraph = buildInterferenceGraph(webs);
    runBasicAlgorithm(finalGraph, webs, k);

    return true;
}

// Function for custom algorithm (splitting + spilling based on dynamic efficiency threshold)
bool runFreeAlgorithm(Graph<int>& graph, std::vector<Web>& webs, int k) {
    if (runBasicAlgorithm(graph, webs, k)){
        return true;
    }

    int maxSplits = 20; 
    int splitsDone = 0;
    std::set<int> forceSpilledIds;

    while (splitsDone < maxSplits) {
        resetRegisterAssignment(webs);
        Graph<int> newGraph = buildInterferenceGraph(webs);

        for (int spillId : forceSpilledIds) {
            newGraph.removeVertex(spillId);
        }

        if (runBasicAlgorithm(newGraph, webs, k)) {
            for (auto& w : webs) {
                if (forceSpilledIds.count(w.id) > 0) w.assignedRegister = -1;
            }
            return true;
        }

        int victimId = selectSplittingVictim(newGraph);

        // Dynamic efficiency calculator
        size_t currentWebSize = 0;
        for (const auto& w : webs) {
            if (w.id == victimId) {
                currentWebSize = w.lines.size();
                break;
            }
        }
        
        auto vertex = newGraph.findVertex(victimId);
        size_t currentDegree = (vertex != nullptr) ? vertex->getAdj().size() : 1;

        // Efficiency threshold
        double splitEfficiency = static_cast<double>(currentWebSize) / currentDegree;
        double dynamicCutoff = 1.5 / static_cast<double>(k); 

        if (splitEfficiency < dynamicCutoff || currentWebSize <= 2) {
            forceSpilledIds.insert(victimId);
            continue;
        }

        if (splitWeb(webs, victimId)) {
            splitsDone++;
        } else {
            forceSpilledIds.insert(victimId);
        }
    }

    // Final Cleanup Pass
    resetRegisterAssignment(webs);
    Graph<int> finalGraph = buildInterferenceGraph(webs);
    for (int spillId : forceSpilledIds) {
        finalGraph.removeVertex(spillId);
    }
    runBasicAlgorithm(finalGraph, webs, k);

    for (auto& w : webs) {
        if (forceSpilledIds.count(w.id) > 0) w.assignedRegister = -1;
    }

    return true;
}