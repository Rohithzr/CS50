/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
       
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIMIT 65536

int main(int argc, string argv[])
{
    // Makes sure there are two or three command line arguments to fit usage.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // Converts the first argument past the name into an integer
    int n = atoi(argv[1]);

    // If there exists a seed, converts that into a positive integer too.
    // Then, runs srand with the integer as a seed, returning a seed for rand.
    if (argc == 3)
    {
        srand((unsigned int) atoi(argv[2]));
    }
    // If there does not exist a seed, runs srand without the seed, returning
    // a seed for rand.
    else
    {
        srand((unsigned int) time(NULL));
    }

    // Prints each integer with LIMIT as the upper bound.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", rand() % LIMIT);
    }

    // that's all folks
    return 0;
}
