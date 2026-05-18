#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int get_factorial(int n);

int main(void)
{
    int n;
    char input [100];
    // Ask user for input
    printf("Enter a single +ve number: ");
    fgets(input, 100, stdin);
    n = atoi(input);

    if (n <= 0 || n > 19)
    {
        printf("INVALID:\nEnter a +ve interger between 1 and 19!\n");
    }
    else
    {
    // Output results
    printf("Factorial of %d is %d\n", n, get_factorial(n));
    }
    return 0;
}
int get_factorial(int n)
{
    int factorial = 1;
    for (int i = 1; i <= n; i++)
    {
        factorial *= i;
    }
    return factorial;
}
