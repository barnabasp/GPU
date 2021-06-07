// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// - Author: Barnabas Porfy
// - Game of Life: Parallel version
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
#include "funcs.hpp"
#define GEN 1
#define ROWS 6
#define COLS 6
#define RULE 2

int main(int, char**) 
{
    //create the object
    //constructor(row, col, rule)
    bool parallel = 1;
    ConwayTable table(ROWS+2, COLS+2, RULE);
    //time measurement
    std::vector<time_t> timeMeasure;
    table.initialize();
    table.dumpGrid();
    table.applyRules();
    table.dumpGrid();
    table.applyRules();
    table.dumpGrid();
    /*
    for(int gen = 0; gen < GEN; gen++)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        table.makeThreads();
        auto t1 = std::chrono::high_resolution_clock::now();
        //std::cout << '\n';
        table.dumpGrid();
        timeMeasure.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count());
        //table.writeFile(gen, timeMeasure[gen], parallel);
        //std::cout << "time: "<< timeMeasure[gen] << std::endl;
    }
    */
    /*
    parallel = 0;
    //create the object
    //constructor(row, col, rule)
    ConwayTable table_seq(ROWS, COLS, RULE);
    //time measurement
    timeMeasure.clear();

    table.initialize();
    //table.dumpGrid();
    for(int gen = 0; gen < GEN; gen++)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        table.applyRules();
        auto t1 = std::chrono::high_resolution_clock::now();
        //std::cout << '\n';
        //table.dumpGrid();
        timeMeasure.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count());
        table.writeFile(gen, timeMeasure[gen], parallel);
        //std::cout << "time: "<< timeMeasure[gen] << std::endl;
    }
    */
}
