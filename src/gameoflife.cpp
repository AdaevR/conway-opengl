#include "gameoflife.h"
#include <unordered_map>
#include <utility>

void GameOfLife::step()
{
    CellNeighborsMap neighborsMap = getNeighborsMap();

    std::set<std::pair<int, int>> newCells = applyRules(neighborsMap);

    cells = std::move(newCells);
}

void GameOfLife::setCell(std::pair<int, int> position, bool value)
{
    if (value && !cells.contains(position))
    {
        cells.insert(position);
    }
    else if (!value && cells.contains(position))
    {
        cells.erase(position);
    }
}

std::set<std::pair<int, int>> GameOfLife::getCells()
{
    return cells;
}

std::set<std::pair<int, int>> GameOfLife::applyRules(CellNeighborsMap neighborsMap)
{
    std::set<std::pair<int, int>> newCells{};

    for (const auto& [cell, count] : neighborsMap)
    {
        bool isAlive = cells.contains(cell);
        if (isAlive && (count == 2 || count == 3))
        {
            newCells.insert(cell);
        }
        else if (!isAlive && count == 3)
        {
            newCells.insert(cell);
        }
    }

    return newCells;
}

CellNeighborsMap GameOfLife::getNeighborsMap()
{
    CellNeighborsMap neighborsCounter;

    for (const auto& [cellX, cellY]: cells)
    {
        for (int dx{-1}; dx <= 1; ++dx)
        {
            for (int dy{-1}; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0)
                    continue;

                int neighborX = cellX + dx;
                int neighborY = cellY + dy;
                neighborsCounter[{neighborX, neighborY}]++;
            }
        }
    }

    return neighborsCounter;
}
