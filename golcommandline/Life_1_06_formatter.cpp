#include <iostream>
#include <string>
#include <sstream>
#include "Life_1_06_formatter.h"

using namespace std;

namespace gol {

    Cells readInput()
    {
        Cells result;
        string line;

        while (getline(cin, line))
        {
            // ignore lines with '#'
            if (line.find("#") != string::npos) continue;

            vector<long long> numbers;
            long long number = 0;
            istringstream iss(line);
            pair<long long, long long> coordinate;
            while (iss >> number)
                numbers.push_back(number);

            if (numbers.size() != 2)
                throw invalid_argument("invalid input file format");

            result.push_back(make_pair(numbers[0], numbers[1]));
        }
        return result;
    }

    void writeOutput(const Cells& aliveCells)
    {
        cout << "#Life 1.06\n";
        for (const auto& aliveCell : aliveCells) {
            cout << aliveCell.first << " " << aliveCell.second << endl;
        }
    }

}
