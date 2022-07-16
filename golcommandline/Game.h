#pragma once
#include <vector>
#include <unordered_set>
#include "types.h"


namespace gol {
    class Game
    {
    public:
        Game(const Cells& liveCells);

        // Generate the specified number of generations
        void Generate(int numGenerations);

        // Returns the current state of the Game - allow for sorting so
        // that consistent output can be obtained
        Cells CurrentState(bool sortState) const;

    private:
        const int MinPopulation = 2;
        const int MaxPopulation = 3;
        // Custom hash function so we can use the hash map for pairs.
        struct HashFunction
        {
            long long operator()(const Cell& coordinate) const
            {
                std::hash<long long> hasher;
                return hasher(coordinate.first) + 0x9e3779b9 +
                    (coordinate.second << 6) + (coordinate.second >> 2);
            }
        };
        std::unordered_set<Cell, HashFunction> m_aliveSet;

        Cells getLiveNeighbors(const Cell& coordinate) const;

        Cells getDeadNeighbors(const Cell& coordinate) const;

        Cells getValidNeighbors(const Cell& coordinate) const;

        void Generate();
    };
}

