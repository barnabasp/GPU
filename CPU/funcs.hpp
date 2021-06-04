#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <vector>
#include <random>
#include <numeric>


class ConwayTable
{
private:
    std::vector<std::vector<int> > grid; //starting canvas or grid with size N+2, M+2 for the edges
    std::vector<std::vector<int> > next_status; //next status holder canvas or grid, same size
    //dimensions of the table
    int m_N;
    int m_M;
public:
    //constructors
    ConwayTable() = default;
    ConwayTable(int N, int M)
    {
        m_N = N;
        m_M = M;
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for(int i = 0; i < m_N ; i++)
        {
            std::vector<int> temp;
            for(int j = 0; j < m_M; j++)
            {
                temp.push_back(dis(gen)> 0.5 ? 0 : 1);
            }
            grid.push_back(temp);
        }
    }
    void printGrid() //prints grid
    {
        for(int i = 0; i < m_N ; i++)
        {
            for(int j = 0; j < m_M; j++)
            {
                if(grid[i][j] == 1) std::cout << "+\t";
                else std::cout << "-\t";
                //            std::cout << table[i][j] << '\t';
            }
            std::cout << '\n';
        }
    }

};

#endif /*FUNCS_HPP*/