// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Sequential version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"

int main(int, char**) 
{
    ConwayTable temp(10,10, 2, 1);
    temp.initialize();
    temp.dumpGrid();
    for(int gen = 0; gen < 10; gen++)
    {
        temp.applyRules();
        std::cout << '\n';
        temp.dumpGrid();
    }
}
