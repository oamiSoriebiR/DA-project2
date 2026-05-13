#include "Graph.h"
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

Graph<int> parseRanges(const std::string& input) {
    Graph<int> ranges;

    std::fstream file(input);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input << std::endl;
        return ranges;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto nums = extractInts(line);
        if (nums.empty()) continue;

        if (nums.size() >= 2) {
            int start = std::min(nums[0], nums[1]);
            int end = std::max(nums[0], nums[1]);

            for (int v = start; v <= end; ++v) {
                ranges.addVertex(v);
                if (v < end) ranges.addEdge(v, v + 1, 1.0);
            }
        } else {
            ranges.addVertex(nums[0]);
        }
    }

    file.close();
    return ranges;
}

std::pair<Graph<int>, std::string> parseRegisters(const std::string& input) {
    Graph<int> registers;
    std::string algorithm;

    std::fstream file(input);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input << std::endl;
        return {registers, algorithm};
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        // Extract algorithm from line starting with "algorithm:"
        if (line.find("algorithm:") == 0) {
            algorithm = trim(line.substr(10)); // Remove "algorithm:" and trim
            continue;
        }

        // Skip lines that are not register data (like "registers: 1")
        if (line.find("registers:") == 0) continue;

        auto nums = extractInts(line);
        if (nums.empty()) continue;

        for (int v : nums) {
            registers.addVertex(v);
        }

        // Connect values in the order they appear on the line.
        for (size_t i = 1; i < nums.size(); ++i) {
            registers.addEdge(nums[i - 1], nums[i], 1.0);
        }
    }

    file.close();
    return {registers, algorithm};
}