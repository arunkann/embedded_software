#include <stdio.h>
#include <stdlib.h>

/* Fibonacci sequence is a series of numbers where
 * 'next' number is found by adding up the two numbers before it.
 */

int fibonacci(int n, int *arr)
{
    if (n==0) {
        arr[n] = 0;
        //printf("n %d val %d\n", n, arr[n]);
        return 0;
    }
    else if (n==1) {
        arr[n] = 1;
        //printf("n %d val %d\n", n, arr[n]);
        return 1;
    }
    else {
        arr[n] = fibonacci(n-1, arr) + fibonacci(n-2, arr);
        //printf("n %d val %d\n", n, arr[n]);
        return arr[n];
    }
}

int main()
{
    int n = 20;
    int *arr;

    arr = malloc(n * sizeof(int));
    fibonacci(n, arr);

    printf("fibo list of %d numbers\n", n);
    for (int i=0; i < n; i++) {
        printf("%d\n", arr[i]);
    }
}
