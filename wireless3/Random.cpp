//====================================================================
//Random.cpp: Some functions product random numbers.
//
//----Author: baobao7766
//----Time:   2015/02/21
//====================================================================

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Random.h"

//=====================================
// Simple random
//=====================================
int* simple_rand( int n )
{
   int i;
   int* RanNum = new int [n];
   for( i = 0; i < n; i++ )
	   RanNum[i] = rand();
   return RanNum;

}

//=====================================
// Ranged random
//=====================================
int* ranged_rand( int range_min, int range_max, int n )
{
   // Generate random numbers in the half-closed interval
   // [range_min, range_max). In other words,
   // range_min <= random number < range_max
   int i;
   int* RanNum = new int [n];
   for ( i = 0; i < n; i++ )
   {
      RanNum[i] = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
   }
   return RanNum;
}

//=====================================
// Bit random
//=====================================
int* bit_rand( int n )
{
   int i;
   int* RanNum = new int [n];
   for( i = 0; i < n; i++ )
	   RanNum[i] = rand() / ((RAND_MAX+3) / 2);
   return RanNum;
}