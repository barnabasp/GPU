#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <array>
#include <numeric>
//parallelisation
#include <thread>
//time
#include <chrono>


class ConwayTable
{
private:
    std::array<std::vector<int>, 2> grids; //container for the current and next status of the grid
    std::vector<std::thread> m_threads;
    //dimensions of the table
    int m_rows;
    int m_cols;
    //rule for the game
    int m_rule;
    //if there's an input - DO NOT USE, IT'S NOT SET UP
    bool m_inp;

    int max_num_of_threads;
    int leftover_row;
    
public:
    //constructors
    ConwayTable() = default;
    ConwayTable(int rows, int cols, int rule)
    {
        m_rows = rows;
        m_cols = cols;
        m_rule = rule;
        m_inp = 0;
        leftover_row = 0;
    }

    ConwayTable(int rows, int cols, int rule, bool inp)
    {
        m_rows = rows;
        m_cols = cols;
        m_rule = rule;
        m_inp = inp;
        leftover_row = 0;
    }
    int getMaxThreads() {return max_num_of_threads;};
    int getRule() {return m_rule;};
    auto setGrid() { grids[0].swap(grids[1]); };
    void initialize();
    void dumpGrid(); //prints out grid to console
    void writeFile(int gen, time_t genTime, bool parallel); //write grid out to a file;
    void applyRules(); //checks and applies the rules of the game
    void makeThreads(); //add tasks to threads
    void paraInitialize(); //paralel: initialization
    void paraApplyRules(int i_thread, int row_per_thread); //paralel: checks and applies the rules of the game

};


#endif /*FUNCS_HPP*/