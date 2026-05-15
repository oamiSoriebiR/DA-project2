#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include "datastructures.h"

/**
 * Verifica se dois LiveRanges se intercetam (partilham pelo menos uma linha).
 */
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

    // Agrupar LiveRanges por nome de variável
    std::map<std::string, std::vector<LiveRange>> varGroups;
    for (const auto& r : ranges) {
        varGroups[r.varName].push_back(r);
    }

    int webIdCounter = 0;

    // Para cada variável, identificar as teias (webs)
    for (auto& entry : varGroups) {
        const std::string& varName = entry.first;
        std::vector<LiveRange>& varRanges = entry.second;

        // Vetor para marcar quais ranges já foram processados nesta variável
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
                
                // Adicionar as linhas deste range à Web
                newWeb.lines.insert(varRanges[currIdx].lines.begin(), varRanges[currIdx].lines.end());

                // Verificar outros ranges da mesma variável que intercetam este
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