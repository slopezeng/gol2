#include <algorithm>
#include "Game.h"

using namespace std;


static vector<pair<int, int>> directions =
{
    {0,1}, // north
    {1,1}, // northeast
    {1,0}, // east
    {1,-1}, // southeast
    {0,-1}, // south
    {-1,-1}, // southwest
    {-1,0}, // west
    {-1,1}, // northwest
};

namespace gol {
    Game::Game(const Cells& liveCells)
    {
        for (auto& cell : liveCells) {
            m_aliveSet.insert(cell);
        }
    }

    void Game::Generate(int numGenerations)
    {
        for (int i = 0; i < numGenerations; i++)
        {
            Generate();
        }
    }

    Cells Game::CurrentState(bool sortState) const
    {
        Cells result;
        for (auto& aliveCell : m_aliveSet) {
            result.push_back(aliveCell);
        }

        if (sortState) {
            sort(result.begin(), result.end());
        }        

        return result;
    }

    Cells Game::getLiveNeighbors(const Cell& coordinate) const
    {
        Cells result;
        auto validNeighs = getValidNeighbors(coordinate);
        for_each(validNeighs.begin(), validNeighs.end(), [&](const auto& cell) {
            if (m_aliveSet.find(cell) != m_aliveSet.end())
            {
                result.push_back(cell);
            }
        });
        return result;
    }

    Cells Game::getDeadNeighbors(const Cell& coordinate) const
    {
        Cells result;
        auto validNeighs = getValidNeighbors(coordinate);
        for_each(validNeighs.begin(), validNeighs.end(), [&](const auto& cell) {
            if (m_aliveSet.find(cell) == m_aliveSet.end())
            {
                result.push_back(cell);
            }
        });
        return result;
    }

    Cells Game::getValidNeighbors(const Cell& coordinate) const
    {
        Cells result;

        for (const auto& dir : directions)
        {
            // x at left boundary
            if (dir.first < 0 && coordinate.first == numeric_limits<LifeInt>::min()) continue;
            // x at right boundary
            if (dir.first > 0 && coordinate.first == numeric_limits<LifeInt>::max()) continue;
            // y at bottom boundary
            if (dir.second < 0 && coordinate.second == numeric_limits<LifeInt>::min()) continue;
            // y at top boundary
            if (dir.second > 0 && coordinate.second == numeric_limits<LifeInt>::max()) continue;

            result.push_back(make_pair(coordinate.first + dir.first, coordinate.second + dir.second));
        }

        return result;
    }

    void Game::Generate()
    {
        std::unordered_set<Cell, HashFunction> newDeadCells;
        std::unordered_set<Cell, HashFunction> deadSurroundingCells;
        std::unordered_set<Cell, HashFunction> newAliveCells;

        for (const auto& cell : m_aliveSet)
        {
            // determine if a live cell needs to be killed
            auto liveSurroundingCells = getLiveNeighbors(cell);
            if (liveSurroundingCells.size() < MinPopulation ||
                liveSurroundingCells.size() > MaxPopulation)
            {
                newDeadCells.insert(cell); // kill it
            }

            // gather all the dead cells around this live cell - the 
            // unordered_set will dedup for us
            for (const auto& deadSurroundingCell : getDeadNeighbors(cell))
            {
                deadSurroundingCells.insert(deadSurroundingCell);
            }
        }

        for (const auto& cell : deadSurroundingCells)
        {
            // determine if a dead cell needs to be revived
            auto liveSurroundingCells = getLiveNeighbors(cell);
            if (liveSurroundingCells.size() == MaxPopulation)
            {
                newAliveCells.insert(cell); // revive it
            }
        }

        // now to update the main live set, which will become the
        // next generation

        // first remove newly dead cells
        for (const auto& cell : newDeadCells)
        {
            m_aliveSet.erase(cell);
        }

        // then add newly live cells
        for (const auto& cell : newAliveCells)
        {
            m_aliveSet.insert(cell);
        }
    }

}
