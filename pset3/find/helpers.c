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
bool search(int value, int values[], int n)
{
    //TODO: implement a binary searching algorithm. 
    //Search array for needle value / see if it exists.
    
    int midpoint = n / 2;
    
    while (n > -1)
    {
        if (value == values[midpoint])
        {
            return true;
        }
        else if (value < values[midpoint])
        {
            //search left side of array
            midpoint = midpoint / 2;
        }
        else if (value > values[midpoint])
        {
            //search right side of array
            midpoint = midpoint + (midpoint / 2);
        }
        
        n--;
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    
    // TODO: implement an O(n^2) sorting algorithm. Will use selection sort.
    
    for (int i = 0; i < n; i++)
    {
        int minIndex = i;
        
        for (int j = i+1; j < n; j++)
        {
            if (values[j] < values[minIndex])
            {
                minIndex = j;
            }
        }
        
        //swap
        if (minIndex != i)
        {
            int save = values[i];
            values[i] = values[minIndex];
            values[minIndex] = save;
        }
        
    }
    
}