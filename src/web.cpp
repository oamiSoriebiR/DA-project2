#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include "datastructures.h"

// Checks if 2 LiveRanges intersect
bool rangesIntersect(const LiveRange& r1, const LiveRange& r2) {
    auto it1 = r1.lines.begin();
    auto it2 = r2.lines.begin();

    while (it1 != r1.lines.end() && it2 != r2.lines.end()) {
        if (*it1 < *it2) {
            ++it1;
        } else if (*it2 < *it1) {
            ++it2;
        } else {
            return true;
        }
    }
    return false;
}

std::vector<Web> webLinking(std::vector<LiveRange> ranges) {
    std::vector<Web> finalWebs;

    if (ranges.empty()) {
        std::cout << "No ranges loaded into memory.\n";
        return finalWebs;
    }

    // Group LiveRanges by var name
    std::map<std::string, std::vector<LiveRange>> varGroups;
    for (const auto& r : ranges) {
        varGroups[r.varName].push_back(r);
    }

    int webIdCounter = 0;

    // For each variable, identify webs
    for (auto& entry : varGroups) {
        const std::string& varName = entry.first;
        std::vector<LiveRange>& varRanges = entry.second;

        // Vector to save which ranges have been processed
        std::vector<bool> visited(varRanges.size(), false);

        for (size_t i = 0; i < varRanges.size(); ++i) {
            if (visited[i]) continue;

            Web newWeb;
            newWeb.id = webIdCounter++;
            newWeb.varName = varName;
            newWeb.assignedRegister = -1;

            std::vector<size_t> q;
            q.push_back(i);
            visited[i] = true;

            size_t head = 0;
            while(head < q.size()){
                size_t currIdx = q[head++];
                
                // Add all lines of this range to web
                newWeb.lines.insert(varRanges[currIdx].lines.begin(), varRanges[currIdx].lines.end());

                // Check other ranges of the variable that intersect with the one being processed
                for (size_t j = 0; j < varRanges.size(); ++j) {
                    if (j != currIdx && !visited[j] && rangesIntersect(varRanges[currIdx], varRanges[j])) {
                        visited[j] = true;
                        q.push_back(j);
                    }
                }
            }
            finalWebs.push_back(newWeb);
        }
    }

    std::cout << "Web linking completed. Created " << finalWebs.size() << " webs from " << ranges.size() << " ranges.\n";
    return finalWebs;
}