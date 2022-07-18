/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	Endpoint.cpp
**
**  Author: 
**
**  Description:
**
**  History:
*/
#include    "pch.h"

#define CUTOFF 16   //Subarrays this size or lower are sorted using shortSort

//Make it easy to reuses this routine for other specialized sorts
#define DataType    Endpoint*

//swap two array elements
static inline void swap(DataType*   a,
                        DataType*   b)
{
    ZAssert (a != b);

    DataType tmp = *a;
    *a = *b;
    *b = tmp;        
}

//compare two array elements
static inline int  comp(DataType const * a,
                        DataType const * b)
{
    return ((*a)->value > (*b)->value)
           ? 1
           : (((*a)->value < (*b)->value)
              ? -1
              : (int)((*b)->highF - (*a)->highF));  //In the event of a tie, high endpoints go before low endpoints
}

//sort the array between lo and hi (inclusive)
void Endpoint::longSort(DataType* lo,
                        DataType* hi)
{
    ZAssert (lo);
    ZAssert (hi);
    if (lo != hi)
    {
        int         stkptr = 0;     /* stack for saving sub-array to be processed */
        DataType*   lostk[30];
        DataType*   histk[30];
        /* Note: the number of stack entries required is no more than
           1 + log2(size), so 30 is sufficient for any array */

        ZAssert (lo < hi);

        /* this entry point is for pseudo-recursion calling: setting
           lo and hi and jumping to here is like recursion, but stkptr is
           prserved, locals aren't, so we preserve stuff on the stack */
        recurse:

        unsigned size = (hi - lo) + 1;  /* number of el's to sort */

        /* below a certain size, it is faster to use a O(n^2) sorting method */
        if (size <= CUTOFF)
        {
             shortSort(lo, hi);
        }
        else
        {
            /* First we pick a partititioning element.  The efficiency of the
               algorithm demands that we find one that is approximately the
               median of the values, but also that we select one fast.  Using
               the first one produces bad performace if the array is already
               sorted, so we use the middle one, which would require a very
               wierdly arranged array for worst case performance.  Testing shows
               that a median-of-three algorithm does not, in general, increase
               performance. */

            DataType*   mid = &lo[size >> 1];   /* find middle element */
            swap(mid, lo);                      /* swap it to beginning of array */

            /* We now wish to partition the array into three pieces, one
               consisiting of elements <= partition element, one of elements
               equal to the parition element, and one of element >= to it.  This
               is done below; comments indicate conditions established at every
               step. */

            DataType*   loguy = lo;
            DataType*   higuy = hi + 1;

            /* Note that higuy decreases and loguy increases on every iteration,
               so loop must terminate. */
            for (;;)
            {
                /* lo <= loguy < hi, lo < higuy <= hi + 1,
                   A[i] <= A[lo] for lo <= i <= loguy,
                   A[i] >= A[lo] for higuy <= i <= hi */

                do
                {
                    loguy += 1;
                } while (loguy <= hi && comp(loguy, lo) <= 0);

                /* lo < loguy <= hi+1, A[i] <= A[lo] for lo <= i < loguy,
                   either loguy > hi or A[loguy] > A[lo] */

                do
                {
                    higuy -= 1;
                } while (higuy > lo && comp(higuy, lo) >= 0);

                /* lo-1 <= higuy <= hi, A[i] >= A[lo] for higuy < i <= hi,
                   either higuy <= lo or A[higuy] < A[lo] */

                if (higuy < loguy)
                    break;

                /* if loguy > hi or higuy <= lo, then we would have exited, so
                   A[loguy] > A[lo], A[higuy] < A[lo],
                   loguy < hi, highy > lo */

                swap(loguy, higuy);

                /* A[loguy] < A[lo], A[higuy] > A[lo]; so condition at top
                   of loop is re-established */
            }

            /*     A[i] >= A[lo] for higuy < i <= hi,
                   A[i] <= A[lo] for lo <= i < loguy,
                   higuy < loguy, lo <= higuy <= hi
               implying:
                   A[i] >= A[lo] for loguy <= i <= hi,
                   A[i] <= A[lo] for lo <= i <= higuy,
                   A[i] = A[lo] for higuy < i < loguy */

            if (lo != higuy)
                swap(lo, higuy);     /* put partition element in place */

            /* OK, now we have the following:
                  A[i] >= A[higuy] for loguy <= i <= hi,
                  A[i] <= A[higuy] for lo <= i < higuy
                  A[i] = A[lo] for higuy <= i < loguy    */

            /* We've finished the partition, now we want to sort the subarrays
               [lo, higuy-1] and [loguy, hi].
               We do the smaller one first to minimize stack usage.
               We only sort arrays of length 2 or more.*/

              
            if ( higuy  - lo > hi - loguy ) //was this: if ( higuy - 1 - lo >= hi - loguy ) 
            {
                if (lo + 1 < higuy)
                {
                    lostk[stkptr] = lo;
                    histk[stkptr] = higuy - 1;
                    ++stkptr;
                }                           /* save big recursion for later */

                if (loguy < hi) {
                    lo = loguy;
                    goto recurse;           /* do small recursion */
                }
            }
            else
            {
                if (loguy < hi)
                {
                    lostk[stkptr] = loguy;
                    histk[stkptr] = hi;
                    ++stkptr;               /* save big recursion for later */
                }

                if (lo + 1 < higuy) {
                    hi = higuy - 1;
                    goto recurse;           /* do small recursion */
                }
            }
        }

        /* We have sorted the array, except for any pending sorts on the stack.
           Check if there are any, and do them. */

        --stkptr;
        if (stkptr >= 0)
        {
            lo = lostk[stkptr];
            hi = histk[stkptr];
            goto recurse;           /* pop subarray from stack */
        }
    }
}

//Bubble-sort the array between lo and hi (inclusive)
void Endpoint::shortSort(DataType* lo,
                         DataType* hi)
{
    ZAssert (lo < hi);

    //Use a bubble sort since that works well when the list is almost sorted (since the
    //sort will terminate early and most things will not have to bubble far).
    DataType*   p = lo; //[lo, p] is sorted
    do
    {
        DataType*   next = p + 1;
        if (comp(p, next) > 0)
        {
            //*p > *next (& therefore either p or next is out of order)
            //Assume it is next and move it to the correct location in the list.
            //Note: we know the list is sorted from lo to p so we could use a binary
            //search. But since the original list is supposed to be "almost sorted"
            //a linear search is probably faster.
            DataType*   back = p - 1;
            while ((back >= lo) && (comp(back, next) > 0))
                back--;

            //Either at the start of the list or *back <= *next
            //In either case, insert next just after back
            DataType    tmp = *next;
            DataType*   bp1 = back + 1;
            DataType*   t = next;
            ZAssert (t > bp1);
            ZAssert (bp1 >= lo);
            do
            {
                //This is a safer way of doing: *t = *(--t);
                DataType&   scratch = *t;
                scratch = *(--t);
            }
            while (t > bp1);

            *bp1 = tmp;
        }
        p = next;
    }
    while (p < hi);
}
