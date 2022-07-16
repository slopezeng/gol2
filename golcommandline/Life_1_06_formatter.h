#pragma once
#include <vector>
#include "types.h"

namespace gol {
    Cells readInput();

    void writeOutput(const Cells& aliveCells);
}