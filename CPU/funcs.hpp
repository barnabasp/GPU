#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <numeric>
//parallelisation
#include <thread>
#include <mutex>
#include <atomic>
//time
#include <chrono>


class ConwayTable
{
private:
    std::vector<std::vector<int> > grid; //starting canvas or grid with size N+2, M+2 for the edges
    std::vector<std::vector<int> > next_status; //next status holder canvas or grid, same size
    //dimensions of the table
    int m_rows;
    int m_cols;
    int m_rule;
    bool m_inp;
    
public:
    //constructors
    ConwayTable() = default;
    ConwayTable(int rows, int cols, int rule)
    {
        m_rows = rows;
        m_cols = cols;
        m_rule = rule;
        m_inp = 0;
    }

    ConwayTable(int rows, int cols, int rule, bool inp)
    {
        m_rows = rows;
        m_cols = cols;
        m_rule = rule;
        m_inp = inp;
    }
    void initialize();
    void dumpGrid(); //prints out grid to console
    void applyRules(); //checks and applies the rules of the game
    void paraInitialize(); //paralel: initialization
    void paraApplyRules(); //paralel: checks and applies the rules of the game

};


#endif /*FUNCS_HPP*/