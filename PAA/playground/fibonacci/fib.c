#include <stdio.h>
/**
 * This file implements a fibonacci function recursively 
 * based on the following recurrence:
 *
 *   F(0) = 0,
 *   
 *   F(1) = 1,
 *
 *   F(n) = F(n-1) + F(n-2)
 *
 * @author: Gustavo Pantuza
 * @since: 05.05.2013
 */


/**
 * Fibonacci function implemented recursively
 */
int fibonacci(int number)
{
    if(number < 2)
        return number;

    return fibonacci(number - 1) + fibonacci(number - 2);
}


void main(int argc, char *argv[])
{
    int argument; 

    if(argc > 0)
    {
        argument = atoi(argv[1]);

        int fib = fibonacci(argument);

        printf("# Fibonacci\n\nargument: %d\nresult: %d\n", argument, fib);

    } else printf("Arguments error");
}
