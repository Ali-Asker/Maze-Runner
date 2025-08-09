#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utility.h"

/*This function will create the maze followed by the generated value from the rows and cols variable.*/
char **maze(int rows, int cols)
{
    int lastrow, index, jdex;
    char **arr, *row;

    /*Will allocate size for rows given by the generated number function into rows variable!*/
    arr = malloc(rows * sizeof(char *));

    /*If allocation fails, will print the statement and return a null value before ending the program!*/
    if (!arr)
    {
        printf("%s", "Memory Allocation has failed! ABORTING!");
        return NULL;
    }

    /*for loop commencing the 2d array for a maze being filled with "*" as grid!*/
    for (index = 0; index < rows; index++)
    {
        /*Will allocate size for cols given by the generated number function into cols variable!*/
        row = malloc(cols * sizeof(char));

        /*If allocation fails, will print the statement and return a null value before ending the program!*/
        if (!row)
        {
            printf("%s", "Memory Allocation has failed! ABORTING!");
            return NULL;
        }

        *((char **)((char *)arr + sizeof(char *) * index)) = row;

        /*Start to populate each column with the "#" symbol*/
        for (jdex = 0; jdex < cols; jdex++)
        {
            *((char *)((char *)row + sizeof(char) * jdex)) = '#';
        }

        lastrow = index == rows - 1;

        /*Will trigger pathway for the rest of the maze*/
        if (index > 0)
        {
            pathway(row, *(char **)((char *)arr + sizeof(char *) * (index - 1)), cols, lastrow);
        }

        /*will trigger pathway for the first row as the previous row is NULL*/
        else
        {
            pathway(row, NULL, cols, index == rows - 1);
        }
    }

    return arr;
}

/*This function will receive user's input and move the start position according to the W/A/S/D moveset.*/
int player(char **maze_arr, int rows, int cols, char choice)
{
    static int player_row = -1;  /*Player's current row position*/
    static int player_col = -1;  /*Player's current column position*/
    static char prev_char = '*'; /*Character that was at player's position (default path)*/
    int i, j, new_row, new_col;

    /*if r is pressed it will reset the player position*/
    if (choice == 'R' || choice == 'r')
    {
        player_row = -1;
        player_col = -1;
        prev_char = '*';
        return 0;
    }

    /*find player's starting position*/
    if (player_row == -1 && player_col == -1)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                if (*(*(maze_arr + i) + j) == 'S')
                {
                    player_row = i;
                    player_col = j;
                    break;
                }
            }
            if (player_row != -1)
                break;
        }
    }

    /*Calculate new position based on direction*/
    new_row = player_row;
    new_col = player_col;

    switch (choice)
    {
    case 'W':
    case 'w':
        new_row--;
        break; /*Move up*/
    case 'A':
    case 'a':
        new_col--;
        break; /*Move left*/
    case 'S':
    case 's':
        new_row++;
        break; /*Move down*/
    case 'D':
    case 'd':
        new_col++;
        break; /*Move right*/
    }

    /*Check if move is valid (not a wall and within bounds)*/
    if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols &&
        *(*(maze_arr + new_row) + new_col) != '#')
    {
        /*Check if player reached the exit*/
        if (*(*(maze_arr + new_row) + new_col) == 'E')
        {
            return 1; /* Return 1 to indicate win condition */
        }

        /*Restore previous position with saved character*/
        *(*(maze_arr + player_row) + player_col) = prev_char;

        /*Save character at new position before overwriting*/
        prev_char = '*';

        /*Update player position*/
        player_row = new_row;
        player_col = new_col;

        /*Place player at new position*/
        *(*(maze_arr + player_row) + player_col) = 'S';

        return 0; /*Successful move*/
    }

    printf("You hit a Wall! Please Try Again.\n");
    return -1; /*Invalid move*/
}

int main()
{
    int rows, cols, move = 0, totalmove = 0, goal = 0, i, j;
    char **maze_arr, choice;

    srand(time(NULL));

    /*randomly get a value from generaterandom function and store it for rows and column to create random maze*/
    rows = generateRandom(10, 20);
    cols = generateRandom(10, 20);

    printf("Welcome to Maze Runner!\n"
           "Maze Generated. Reach the exit (E).\n\n");

    /*pass through the maze function to create the maze*/
    maze_arr = maze(rows, cols);

    /*returns an error if the maze failed to generate*/
    if (!maze_arr)
    {
        return EXIT_FAILURE;
    }

    /*looping each row with predefined path for the maze on console*/
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("%c", *(*(maze_arr + i) + j));
        }
        printf("\n");
    }

    printf("\nUse W/A/S/D to move. Press Q to quit. Press R to regenerate Maze.\n");
    choice = ' ';

    /*user functionality to move the player in the maze*/
    while (1)
    {
        printf("Input W/A/S/D: ");
        scanf(" %c", &choice);
        printf("\n");
        switch (choice)
        {
        /*accepts W/A/S/D (including lowercase) and pass through the player function to move accordingly to the key pressed*/
        case 'W':
        case 'w':
        case 'A':
        case 'a':
        case 'S':
        case 's':
        case 'D':
        case 'd':
            move++;
            /*if the player reaches to "E" it will congratulate their goal and display the moves it took to reach to end*/
            if (player(maze_arr, rows, cols, choice) == 1)
            {
                goal++;
                totalmove += move;
                printf("\nCongratulations! You've reached the exit! Total moves: %d \n", move);
                printf("|=========================|\n"
                       "|  Score  |  Total Moves  |\n"
                       "|=========|===============|\n"
                       "|    %d    |       %d      |\n"
                       "|=========================|\n",
                       goal, totalmove);
                printf("\nUse W/A/S/D to move. Press Q to quit. Press R to regenerate Maze.\n");
                move = 0;
                freeMaze(maze_arr, rows);

                /*will generate a new maze*/
                maze_arr = maze(rows, cols);

                /*Reset player position tracking for new maze*/

                /*Calling with space will just reset position*/
                player(maze_arr, rows, cols, 'R');
            }
            break;

        /*will accept "R" (including lowercase) to regenerate the maze*/
        case 'R':
        case 'r':

            /*will release the current maze memory before generating a new one*/
            freeMaze(maze_arr, rows);

            /*will generate a new maze*/
            maze_arr = maze(rows, cols);

            /*Calling with space will just reset position*/
            player(maze_arr, rows, cols, 'R');
            /*Reset move counter*/
            move = 0;
            break;

            /*Will free the memory before exiting the maze game*/
        case 'Q':
        case 'q':
            printf("|===============================|\n"
                   "|  Final Score  |  Total Moves  |\n"
                   "|===============|===============|\n"
                   "|       %d       |       %d      |\n"
                   "|===============================|\n",
                   goal, totalmove);
            freeMaze(maze_arr, rows);
            printf("Exiting Program... Program made by Ali Asker!\n");
            exit(0);
            break;

            /*if option pressed not according to the case, it will print an error message*/
        default:
            printf("Invalid Option please try again!\n");
        }

        /*Display the maze again after the user made a move with the new player position*/
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                printf("%c", *(*(maze_arr + i) + j));
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
