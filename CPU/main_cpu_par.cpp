// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Parallel version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"

int main(int, char**) 
{
    ConwayTable temp(12,12, 2);
    temp.paraInitialize();
    temp.dumpGrid();
    for(int gen = 0; gen < 1; gen++)
    {
        temp.makeThreads();
        std::cout << '\n';
        temp.dumpGrid();
    }
}
