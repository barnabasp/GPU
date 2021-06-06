// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Parallel version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"

int main(int, char**) 
{
    //create the object
    //constructor(row, col, rule)
    bool parallel = 1;
    ConwayTable table(960, 960, 2);
    //time measurement
    std::vector<time_t> timeMeasure;

    table.paraInitialize();
    //table.dumpGrid();
    for(int gen = 0; gen < 5; gen++)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        table.makeThreads();
        auto t1 = std::chrono::high_resolution_clock::now();
        //std::cout << '\n';
        //table.dumpGrid();
        timeMeasure.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count());
        table.writeFile(gen, timeMeasure[gen], parallel);
    }
    parallel = 0;
    //create the object
    //constructor(row, col, rule)
    ConwayTable table_seq(960, 960, 2);
    //time measurement
    timeMeasure.clear();

    table.initialize();
    //table.dumpGrid();
    for(int gen = 0; gen < 5; gen++)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        table.applyRules();
        auto t1 = std::chrono::high_resolution_clock::now();
        //std::cout << '\n';
        //table.dumpGrid();
        timeMeasure.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count());
        table.writeFile(gen, timeMeasure[gen], parallel);
    }
}
