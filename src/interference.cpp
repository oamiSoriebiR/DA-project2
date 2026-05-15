#include "Graph.h"
#include "datastructures.h"
#include <vector>

/**
 * Verifica a interseção entre duas Webs.
 * Como as linhas estão num std::set (ordenado), usamos a técnica de dois ponteiros.
 */
bool checkInterference(const Web& w1, const Web& w2) {
    auto it1 = w1.lines.begin();
    auto it2 = w2.lines.begin();

    while (it1 != w1.lines.end() && it2 != w2.lines.end()) {
        if (*it1 < *it2) {
            ++it1;
        } else if (*it2 < *it1) {
            ++it2;
        } else {
            return true; // Encontrou pelo menos uma linha em comum
        }
    }
    return false;
}

/**
 * Constrói o Grafo de Interferência.
 * Simulamos a não-direcionalidade adicionando arestas
 *  nos dois sentidos para cada interferência detetada.
 */
Graph<int> buildInterferenceGraph(const std::vector<Web>& webs) {
    Graph<int> ig;

    // Adicionar todas as Webs como vértices
    for (const auto& w : webs) {
        ig.addVertex(w.id);
    }

    // Analisar interferências par a par
    for (size_t i = 0; i < webs.size(); ++i) {
        for (size_t j = i + 1; j < webs.size(); ++j) {
            
            if (checkInterference(webs[i], webs[j])) {
                ig.addEdge(webs[i].id, webs[j].id, 1.0);
                ig.addEdge(webs[j].id, webs[i].id, 1.0);
            }
        }
    }

    std::cout << "Interference Graph built. Vertices: " << ig.getNumVertex() << std::endl;
    return ig;
}