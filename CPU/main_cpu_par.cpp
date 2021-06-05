// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Parallel version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"

int main(int, char**) 
{
    ConwayTable temp(100,100, 2);
    temp.paraInitialize();
    //temp.dumpGrid();
    for(int gen = 0; gen < 100; gen++)
    {
        temp.applyRules();
        //std::cout << '\n';
        //temp.dumpGrid();
    }
}
