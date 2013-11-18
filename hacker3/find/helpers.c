/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int array[], int n)
{
    int min = 0;
    int max = n;
    int mid = 0;

    if (n <= 0)
    {
        return false;
    }
    if ((array[min] > value) || (array[max - 1] < value))
    {
        return false;
    }
    while(true)
    {
        mid = (min + max)/2;
        if (array[mid] == value)
        {
            return true;
        }
        else if(array[mid] > value)
        {
            max = mid;
        }
        else if(array[mid] < value)
        {
            min = mid;
        }
        else
        {
            return false;
        }  
        if((max - min) == 1 && (array[max] == value))
        {
            return true;
        }
    }
    return true;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int element = 0;
    int j = 0;
    for(int i = 1; i < n; i++)
    {
        element = values[i];
        j = i;
        while ((j > 0) && (values[j - 1] > element))
        {
            values[j] = values[j - 1];
            j--;
        }
        values[j] = element;
    }
}
