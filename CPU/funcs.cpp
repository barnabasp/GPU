#include "funcs.hpp"

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Class functions+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//

void ConwayTable::initialize()
{
    grids[0].resize(m_cols * m_rows);
    grids[1].resize(m_cols * m_rows);
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < m_rows * m_cols; i++)
    {
            //grids[0][i] = (dis(gen) > 0.5 ? 0 : 1);
            grids[0][i] = 0;
    }
    grids[0][2 + m_cols * 2] = 1;
    grids[0][2 + m_cols * 3] = 1;
    grids[0][2 + m_cols * 4] = 1;
}
void ConwayTable::dumpGrid() //prints out grid to console
{
    int idx;
    for (int i_row = 1; i_row < m_rows-1; i_row++)
    {
        for (int i_col = 1; i_col < m_cols-1; i_col++)
        {
            idx = i_row * m_cols + i_col;
            std::cout << grids[0][idx];
        }
        std::cout << "\n";
    }
}
void ConwayTable::writeFile(int gen, time_t genTime, bool parallel)
{
    //seting up the string name for the file
    std::stringstream fileName(""); fileName << "C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\CPU\\outputs\\" << "para" << parallel << "-"<< m_rows << "x" << m_cols << "-gen_" << gen  << ".txt";
    std::ofstream file(fileName.str().c_str());
    //header
    file << "#T " << genTime << std::endl;
    //body
    int idx;
    for (int i_row = 1; i_row < m_rows-1; i_row++)
    {
        for (int i_col = 1; i_col < m_cols-1; i_col++)
        {
            idx = i_row * m_cols + i_col;
            if (grids[0][idx] == 1)
                file << "+,";
            else
                file << "-,";
        }
        file << "\n";
    }
    file.close();

}
void ConwayTable::applyRules() //apply the rules of the game
{
    int temp;
    int idx = 0;
    //loop throught the grid, excluding the border indices
    for (int i_row = 1; i_row < m_rows-1; i_row++)
    {
        for (int i_col = 1; i_col < m_cols-1; i_col++)
        {
            temp = 0;                                                //holder for dead or alive cells
            idx = i_row * m_cols + i_col;
            temp += grids[0][(i_row - 1) * m_cols + i_col] + //up
                    grids[0][(i_row + 1) * m_cols + i_col] + //down
                    grids[0][i_row * m_cols + (i_col - 1)] + //left
                    grids[0][i_row * m_cols + (i_col + 1)] + //right
                    grids[0][(i_row - 1) * m_cols + (i_col - 1)] + //up and right
                    grids[0][(i_row - 1) * m_cols + (i_col + 1)] + //up and right
                    grids[0][(i_row + 1) * m_cols + (i_col - 1)] + //down and right
                    grids[0][(i_row + 1) * m_cols + (i_col + 1)]; //down and right
            
            if (temp == m_rule) //stays
            {
                grids[1][idx] = grids[0][idx];
            }
            else if (temp == m_rule + 1) //lives
            {
                grids[1][idx] = 1;
            }
            else //dies
            {
                grids[1][idx] = 0;
            }
        }
    }
    grids[0] = grids[1];
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Parallel Class functions+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void ConwayTable::paraInitialize() //initialize for parallel - maybe not needed a separate function, but this is maybe easier on the eye
{
    max_num_of_threads = (int)std::thread::hardware_concurrency(); //number of threads
    leftover_row = (m_rows-2)%max_num_of_threads;
    std::cout << "The grid dimensions are: ";
    std::cout << "Rows: " << m_rows << " Cols: " << m_cols << std::endl;
    grids[0].resize(m_cols * m_rows);
    grids[1].resize(m_cols * m_rows);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < m_rows * m_cols; i++)
    {
            //grids[0][i] = (dis(gen) > 0.5 ? 0 : 1);
            grids[0][i] = 0;
    }
    grids[0][2 + m_cols * 2] = 1;
    grids[0][2 + m_cols * 3] = 1;
    grids[0][2 + m_cols * 4] = 1;
}
void ConwayTable::paraApplyRules(int i_thread, int row_per_thread)
{
    int temp;
    int idx;
    int leftrow = 0; //extra row for the end
    if(i_thread == max_num_of_threads - 1) leftrow +=leftover_row; //if we are at the last thread add the remaining rows
    for (int i_row = i_thread * row_per_thread; i_row < (i_thread + 1) * row_per_thread + leftrow; i_row++)
    {
        if(i_row == 0) i_row++;
        for (int i_col = 1; i_col < m_cols-1; i_col++)
        {
            temp = 0; //holder for dead or alive cells
            idx = i_row * m_cols + i_col;
            temp += grids[0][(i_row - 1) * m_cols + i_col] + //up
                    grids[0][(i_row + 1) * m_cols + i_col] + //down
                    grids[0][i_row * m_cols + (i_col - 1)] + //left
                    grids[0][i_row * m_cols + (i_col + 1)] + //right
                    grids[0][(i_row - 1) * m_cols + (i_col - 1)] + //up and right
                    grids[0][(i_row - 1) * m_cols + (i_col + 1)] + //up and right
                    grids[0][(i_row + 1) * m_cols + (i_col - 1)] + //down and right
                    grids[0][(i_row + 1) * m_cols + (i_col + 1)]; //down and right
            if (temp == m_rule) //stays
            {
                grids[1][idx] = grids[0][idx];
            }
            else if (temp == m_rule + 1) //lives
            {
                grids[1][idx] = 1;

            }
            else //dies
            {
                grids[1][idx] = 0;
            }
        }
    }
}
void ConwayTable::makeThreads() //add a row to each threads and then execute
{
    m_threads.clear(); //clear the holder for the new generation
    int row_per_thread = (m_rows-2) / max_num_of_threads; 
    for(int i_thread = 0; i_thread < max_num_of_threads; i_thread++)
    {
        //give the threads each their own task
        //std::cout << "\nrow_counter: " <<i_row_count << std::endl;
        m_threads.push_back(std::thread(&ConwayTable::paraApplyRules, this, i_thread, row_per_thread));
    }
    for (auto &t : m_threads)
        t.join();
}