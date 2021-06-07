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
    std::stringstream fileName(""); fileName << "C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\CPU\\outputs\\" << "para" << parallel << "-"<< m_rows << "x" << m_cols << "-gen_" << gen  << ".txt";
    std::ofstream file(fileName.str().c_str());
    std::vector<std::vector<int>> currGrid = getGrid();
    //header
    file << "#T " << genTime << std::endl;
    //body
    for (int i_row = 0; i_row < m_rows; i_row++)
    {
        for (int i_col = 0; i_col < m_cols; i_col++)
        {
            if (currGrid[i_row][i_col] == 1)
                file << "+,";
            else
                file << "-,";
        }
        file << '\n';
    }
    file.close();

}
void ConwayTable::applyRules()
{
    int temp;
    int idx = 0;
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
void ConwayTable::paraInitialize()
{
    max_num_of_threads = (int)std::thread::hardware_concurrency(); //number of threads
    //for easier use, resize the grid if the dimensions have left over when dividing by the number of threads
    row_per_thread = m_rows / max_num_of_threads;
    col_per_thread = m_cols / max_num_of_threads;
    std::cout << "The grid dimensions are: ";
    if (m_rows % max_num_of_threads != 0)
        m_rows = row_per_thread * max_num_of_threads;
    if (m_cols % max_num_of_threads != 0)
        m_cols = col_per_thread * max_num_of_threads;
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
void ConwayTable::paraApplyRules(int i_thrd)
{
    int temp;
    int idx;
    std::vector<int> currentLine(m_cols); //holder for this thread's line
    //std::unique_lock<std::mutex> lock(m);
    for (int i_row = i_thrd * row_per_thread; i_row < (i_thrd + 1) * row_per_thread; i_row++)
    {
        for (int i_col = 0; i_col < m_cols; i_col++)
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
        //not exactly sure...
        /*
        if(threadCnt == getMaxThreads() - 1)
        {
            cv.notify_all();
        }
        else 
        {
            threadCnt++;
            cv.wait(lock);
        }
        */
        //give the current rows the values
        grid[i_row] = currentLine;
    }
}
void ConwayTable::makeThreads()
{
    m_threads.clear(); //clear the holder for the new threads
    for (int i_thread = 0; i_thread < getMaxThreads(); i_thread++)
    {
        //give the threads each their own task
        m_threads.push_back(std::thread(&ConwayTable::paraApplyRules, this, i_thread));
    }
    for (auto &t : m_threads)
        t.join();
}