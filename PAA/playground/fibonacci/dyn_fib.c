/**
 * Fibonacci implementation using dynamic programming aproach.
 *
 * For each call of fibonacci function the memo[] array store 
 * fibonacci return value of a given number.
 *
 * Dynamic programming uses memoization of values to not 
 * calculate this values more than once. With that, the 
 * algorithm avoid repeatedly executions.
 *
 * Optimal Substructure:
 *
 *     By definition we have that fibonacci problem is defined
 * by the following recursion: 
 *         F(0) = 0
 *         F(1) = 1
 *         F(n) = F(n - 1) + F(n - 2)
 *
 *     This problem has the optimal substructure. As its solution
 * is optimal, when the algorithms reduce the problem in 
 * subproblems (eg: F(n - 1)) and apply the same solution to this
 * subproblems we satisfy the optimal substructure that says:
 *     
 *      "Optimal solutions to a problem incorporate optimal solutions to
 *      related subproblems, wich we may solve independently" (CORMEN, p362)
 *     
 *
 * Overlapping Subproblems:
 *
 *     Fibonacci recursive solution compute many times the same 
 * values. It is subproblems overlapping. Dynamic programming 
 * eliminate this redundancy.
 *
 * @autho: Gustavo Pantuza
 * @since: 05.05.2013
 */

#include <stdio.h>
#include <limits.h>


/* UNKNOWN is the value to initialize all memo[] values */
#define UNKNOWN INT_MIN

/* Type for long integers */
#define INT long long


/* Fibonacci function implemented using dynamic programming */
INT fibonacci(int number, INT memo[])
{
    if(memo[number] == UNKNOWN)                    // avoiding one recursion
        memo[number] = fibonacci(number - 1, memo) + memo[number - 2]; 

    return memo[number];
}


void main(int argc, char *argv[])
{
    int argument; 

    if(argc > 0)
    {
        argument = atoi(argv[1]);
        INT memo[argument + 1];
        
        int i = 0;
        for(; i <= argument; i++)
            memo[i] = UNKNOWN;

        /* Optimization to avoid a comparison on every fibonacci call */
        memo[0] = 0; // F(0) = 0
        memo[1] = 1; // F(1) = 1

        INT fib = fibonacci(argument, memo);

        printf("# Fibonacci\n\nargument: %d\nresult: %d\n", argument, fib);

    } else printf("Arguments error");
}
