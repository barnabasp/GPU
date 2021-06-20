__kernel void applyRules(__global int* grid1, __global int* grid2, int N, int M, int rule)
{

    //get the two dimension
    int i_row = get_global_id(0);
    int i_col = get_global_id(1);
    //initialize
    int neighbors = 0;
    int x = i_row;
    int y = i_col;
    //solution for the edges
    if(i_row-1 == -1) x += N;
    if(i_col-1 == -1) y += M;
    if(i_row == N) x -= N;
    if(i_col == M) y -= M;
    //index for the 1D array
    int idx = x * M + y;
    //loop throught the grid, excluding the border indices
    neighbors += grid1[(x - 1) * M + y] + //up
            grid1[(x + 1) * M + y] + //down
            grid1[x * M + (y - 1)] + //left
            grid1[x * M + (y + 1)] + //right
            grid1[(x - 1) * M + (y - 1)] + //up and right
            grid1[(x - 1) * M + (y + 1)] + //up and right
            grid1[(x + 1) * M + (y - 1)] + //down and right
            grid1[(x + 1) * M + (y + 1)]; //down and right
    //Decision on the life of the cell.
    if (neighbors == rule) //stays
    {
        grid2[idx] = 1*grid1[idx];
    }
    else if (neighbors == rule + 1) //lives
    {
        grid2[idx] = 1;
    }
    else //dies
    {
        grid2[idx] = 0;
    }
}