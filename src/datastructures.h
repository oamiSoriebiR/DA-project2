#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <set>

struct AssignmentConfig {
    int k = 0;                  // Número de registos
    std::string algorithm = ""; // "basic", "spilling" , "splitting" ou "free"
};

struct LiveRange {
    std::string varName;
    std::set<int> lines; // Usamos set para manter ordenado e evitar duplicados
};

struct Web {
    int id;
    std::string varName;
    std::set<int> lines;
    int assignedRegister = 0; // -1 = Memória/Spill; 0 = Unassigned
};

#endif //DATASTRUCTURES_H