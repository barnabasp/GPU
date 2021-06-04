#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <numeric>


class ConwayTable
{
private:
    std::vector<std::vector<int> > grid; //starting canvas or grid with size N+2, M+2 for the edges
    std::vector<std::vector<int> > next_status; //next status holder canvas or grid, same size
    //dimensions of the table
    int m_rows;
    int m_cols;
    int m_rule;
public:
    //constructors
    ConwayTable() = default;
    ConwayTable(int rows, int cols, int rule)
    {
        m_rows = rows;
        m_cols = cols;
        m_rule = rule;
        next_status.resize(m_rows, std::vector<int>(m_cols));
        grid.resize(m_rows, std::vector<int>(m_cols));
        /*
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for(int i = 0; i < m_rows ; i++)
        {
            std::vector<int> temp;
            for(int j = 0; j < m_cols; j++)
            {
                temp.push_back(dis(gen)> 0.5 ? 0 : 1);
            }
            grid.push_back(temp);
        }*/
        std::ifstream inputfile;
        inputfile.open("C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\CPU\\input_10x10.txt");
        for(int i = 0; i < m_rows ; i++)
        {
            std::vector<int> temp;
            for(int j = 0; j < m_cols; j++)
            {
//                temp.push_back(inputfile);
                inputfile >>  grid[i][j];
                std::cout << grid[i][j] << '\t';
            }
            std::cout << '\n';
            grid.push_back(temp);
        }
        inputfile.close();
    }
    void dumpGrid() //prints out grid to console
    {
        for(int i_row = 0; i_row < m_rows ; i_row++)
        {
            for(int i_col = 0; i_col < m_cols; i_col++)
            {
                if(grid[i_row][i_col] == 1) std::cout << "+\t";
                else std::cout << "-\t";
            }
            std::cout << '\n';
        }
    }
    void applyRules() //checks and applies the rules of the game
    {
        int temp;
        for(int i_row = 0; i_row < m_rows; i_row++)
        {
        for(int i_col = 0; i_col < m_cols; i_col++)
        {
        temp = 0;
        for(int j_row = i_row-1; j_row <= i_row+1; j_row++) //loops through surrounding rows
        {
            for(int j_col = i_col-1; j_col <= i_col+1; j_col++) //loops through surrounding columns
            {
                int x = j_row;
                int y = j_col;
                if(j_row == i_row && j_col == i_col) //if on the cell we are checking, jump out of the loop
                {
                    continue;
                }
                else //if periodic boundaries are set, loop them on the other side
                {
                    if(j_row == -1) x += m_rows;
                    if(j_col == -1) y += m_cols;
                    if(j_row == m_rows) x -= m_rows;
                    if(j_col == m_cols) y -= m_cols;
                    temp += grid[x][y];
                }

            }

        }
        //    cout << "i: " << i << " j: " << j << " "  << temp << endl;
        if(temp == m_rule) //stays
        {
            next_status[i_row][i_col] = grid[i_row][i_col];
        }
        else if(temp == m_rule+1) //lives
        {
            next_status[i_row][i_col] = 1; 
        }
        else //dies
        {
            next_status[i_row][i_col] = 0; 
        }
    }
    }
    grid.swap(next_status);
    }

};

#endif /*FUNCS_HPP*/