#pragma once

#include <set>
#include <unordered_map>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

using CellNeighborsMap = std::unordered_map<std::pair<int, int>, int, PairHash>;

class GameOfLife
{
private:
    std::set<std::pair<int, int>> cells{};

    std::set<std::pair<int, int>> applyRules(CellNeighborsMap neighborsMap);
    CellNeighborsMap getNeighborsMap();

public:
    bool isRunning = false;
    float stepTime = 0.02;

    void step();
    void setCell(std::pair<int, int> position, bool value);
    std::set<std::pair<int, int>> getCells();
};

