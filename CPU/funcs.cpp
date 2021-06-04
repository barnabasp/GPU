#include "funcs.hpp"

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Class functions+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//

void ConwayTable::initialize()
{
    next_status.resize(m_rows, std::vector<int>(m_cols)); //defining the future grid - needed for both type of inputs
    if(m_inp)
    {
        grid.resize(m_rows, std::vector<int>(m_cols)); //defining the grid - grid needs to have the same size as the input
        //need to modify later
        std::ifstream inputfile;
        inputfile.open("C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\CPU\\input_10x10.txt");
        for(int i = 0; i < m_rows ; i++)
        {
            for(int j = 0; j < m_cols; j++)
            {
                inputfile >>  grid[i][j]; //read the current, single value
                std::cout << grid[i][j] << '\t';
            }
            std::cout << '\n';
        }

        inputfile.close();
    }
    else
    {
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for(int i = 0; i < m_rows ; i++)
        {
            std::vector<int> temp; //holder for 1 line
            for(int j = 0; j < m_cols; j++)
            {
                temp.push_back(dis(gen)> 0.5 ? 0 : 1);
            }
            grid.push_back(temp);
        }
    }
}
void ConwayTable::dumpGrid() //prints out grid to console
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
void ConwayTable::applyRules()
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
    //the next status is now the current
    grid.swap(next_status);
}
