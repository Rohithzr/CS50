/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// location of blank - size of 2 because it has two coordinates.
int blanklocation[2];

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init()
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = pow(d,2) - (d*i + j + 1);
        }
    }
    board[d-1][d-1] = pow(d,2); 
    if (d % 2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
    blanklocation[0] = d-1;
    blanklocation[1] = d-1;
   
}
    

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Iterate over all of the rows and columns.
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // If the number is between 1 and 9, format in a certain way.
            if (board[i][j] < 10 && board[i][j] > 0 && board[i][j] != pow(d,2))
            {
                printf(" %d ", board[i][j]);
            }
            // If the number is two digits, format it differently.
            else if (board[i][j] >= 10 && board[i][j] != pow(d,2))
            {
                printf("%d ", board[i][j]);
            }
            // If the number is "d^2", but a blank/underscore in.
            else if (board[i][j] == pow(d,2))
            {
                printf(" _ ");
            }
        }
        printf("\n");
    }  
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Iterate over all of the rows and columns
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // Check if the current row/column pair has the entered tile.
            if(board[i][j] == tile)
            {
                // Check if the tile is adjacent to a blank (labeled "0")
                if ((abs(i - blanklocation[0]) <= 1 && abs(j - blanklocation[1]) == 0)
                    || (abs(i - blanklocation[0]) == 0 && abs(j - blanklocation[1]) <= 1))
                {
                    // Put the tile where the blank (0) is. Move the blank (0) where the tile was.
                    // Set zerolocation to where the blank (0) now is.
                    board[blanklocation[0]][blanklocation[1]] = tile;
                    board[i][j] = pow(d,2);
                    blanklocation[0] = i;
                    blanklocation[1] = j;
                    return true;
                } 
            }   
        }
    } 
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{

    bool result;
    
    // Compare the board to the ideal version
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if (board[i][j] == (d*i + j + 1))
            {
                result = true;
            }
            else
            {
                result = false;
                break;
            }  
        }
        
    }    
    
    return result;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
	fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
	fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
