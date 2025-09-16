/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>
#include <square.h>

int Square(int N)
{
    if (square_counter != NULL && current_thread >= 0)
        square_counter[current_thread]++;

    if (N == 0) return (0);
    return (Square(N - 1) + N + N - 1);
}
