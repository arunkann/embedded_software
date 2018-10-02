#include <stdio.h>

int myArray[3][4] =
{
    {0, 3, 6, 9},
    {1, 4, 7, 10},
    {2, 5, 8, 11}
};

int main()
{
    printf("Print address of rows from array name:\n");
    /* This pointer can't be modified */
    printf("%p %p %p\n", myArray, myArray+1, myArray+2);

    int (*arr)[4] = myArray; /* this is a pointer to arrays */

    printf("Print address of rows from pointer to arrays:\n");
    /* This pointer can be modified */
    printf("%p\n", arr);
    arr++;
    printf("%p\n", arr);
    arr++;
    printf("%p\n", arr);

    arr = myArray;

    printf("Print some elements using the pointer:\n");
    printf("%d %d\n", (*arr)[0], (*arr)[1]);
    printf("%d %d\n", *(*arr+0), *(*arr+1));
    printf("%d %d\n", *(*(arr+1)+0), *(*(arr+1)+1));
    printf("%d %d\n", *(*(arr+2)+0), *(*(arr+2)+1));

    return 0;
}
