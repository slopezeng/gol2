// golcommandline.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Life_1_06_formatter.h"
#include "Game.h"

using namespace std;
using namespace gol;

int main()
{
    auto res = readInput();
    Game g(res);

    g.Generate(10);

    writeOutput(g.CurrentState(true));

    //cout << "min: " << numeric_limits<LifeInt>::min() << endl;
    //cout << "max: " << numeric_limits<LifeInt>::max() << endl;
}
