#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utility.h"

/*This function will generate a number within the specified range (Min and Max).*/
int generateRandom(int min, int max)
{
    int result;

    result = min + rand() % (max - min + 1);
    return result;
}

/*This function will generate a random stat, exit, and pathway to connect the starting and ending point of the maze.*/
int pathway(char *row, char *prev_row, int cols, int lastrow)
{
    static int prev_path_pos;
    int path_pos, n;

    /*randomly place start in the first row*/
    if (!prev_row)
    {
        path_pos = generateRandom(1, cols - 2);
        *(row + path_pos) = 'S';
        prev_path_pos = path_pos;
    }

    /*create a trail that guarantees a connection from start to exit*/
    if (!lastrow && prev_row)
    {
        /*assures that the first row after the start is directly connected for a path*/
        *(row + prev_path_pos) = '.';

        /*Create a path that's near the last path position*/
        for (n = 2; n < (cols / 2); n++)
        {
            /*Place path near previous position (maximum 2 steps away)*/
            path_pos = prev_path_pos + generateRandom(-1, 1);

            /*Keep position within bounds*/
            if (path_pos <= 1)
                path_pos = 1;
            if (path_pos >= cols - 1)
                path_pos = cols - 2;

            /*Mark the path with the symbol "."*/
            *(row + path_pos) = '.';

            /*Remember last position*/
            prev_path_pos = path_pos;
        }
    }

    /*randomly place end in the last row, near the last path position*/
    if (lastrow)
    {
        /*Place exit near last path position*/
        path_pos = prev_path_pos;

        /*Keep position within bounds*/
        if (path_pos <= 1)
            path_pos = 1;
        if (path_pos >= cols - 1)
            path_pos = cols - 2;

        /*Mark the exit with the symbol "E"*/
        *(row + path_pos) = 'E';
    }
    return EXIT_SUCCESS;
}

/*This function will loop through the row to clear the memory out (Preventing Memory Leak).*/
int freeMaze(char **maze_arr, int rows)
{
    int index;

    /*Return error message if freeing memory has failed.*/
    if (!maze_arr)
    {
        printf("Error on freeing the maze!");
        return EXIT_FAILURE;
    }

    /*loop through each of the rows*/
    for (index = 0; index < rows; index++)
    {
        if (*(maze_arr + index))
        {
            free(*(maze_arr + index));
        }
    }
    
    /*This will remove the pointer to maze_arr (removing the container)*/
    free(maze_arr);
    return 0;
}