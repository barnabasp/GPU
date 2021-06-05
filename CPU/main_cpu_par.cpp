// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Parallel version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"

int main(int, char**) 
{
    ConwayTable temp(10,10, 2);
    temp.initialize();
    temp.dumpGrid();
    for(int gen = 0; gen < 10; gen++)
    {
        temp.applyRules();
        std::cout << '\n';
        temp.dumpGrid();
    }
}
