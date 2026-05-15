#include "Graph.h"
#include "datastructures.h"
#include <regex>
#include <string>
#include <fstream>
#include <sstream>

static std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

static std::vector<int> extractInts(const std::string& line) {
    std::vector<int> values;
    static const std::regex numberPattern(R"(-?\d+)");
    for (auto it = std::sregex_iterator(line.begin(), line.end(), numberPattern);
         it != std::sregex_iterator();
         ++it) {
        values.push_back(std::stoi(it->str()));
    }
    return values;
}

std::vector<LiveRange> parseRanges(const std::string& input) {
    std::vector<LiveRange> ranges;
    std::fstream file(input);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input << std::endl;
        return ranges;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

         // Formato esperado: "varName: 1+, 2, 3-"
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        LiveRange lr;
        // 1. Extrair o nome da variável (antes dos dois pontos)
        lr.varName = trim(line.substr(0, colonPos));

        // 2. Extrair os números (depois dos dois pontos)
        std::string rightSide = line.substr(colonPos + 1);
        std::vector<int> nums = extractInts(rightSide);

        if (nums.empty()) continue;

        // 3. Inserir números na LiveRange
        for (int n : nums) {
            lr.lines.insert(n);
        }

        ranges.push_back(lr);
    }

    file.close();
    return ranges;
}

AssignmentConfig parseRegisters(const std::string& input) {
    AssignmentConfig config;
    std::fstream file(input);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.find("registers:") == 0) {
            std::string contentAfter = trim(line.substr(10));
            auto nums = extractInts(contentAfter);
            config.k = nums[0];
            continue;
        }

        // Extract algorithm from line starting with "algorithm:"
        if (line.find("algorithm:") == 0) {
            config.algorithm = trim(line.substr(10)); // Remove "algorithm:" and trim
            continue;
        }
    }

    file.close();
    return config;
}