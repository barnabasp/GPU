__kernel void applyRules()
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