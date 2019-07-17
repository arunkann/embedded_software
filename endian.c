#include <stdio.h>

int main()
{
    int a = 1; /* minimum size of int data type is at least 2 bytes per C standard */
    char *aptr = (char*) &a; /* aptr points to (lowest) address where a is located */

    /* if Low Address contains LSB, then LE machine.
     * if High Address contains LSB, then BE machine.
     */
    if (*aptr)
        printf("little endian\n");
    else
        printf("big endian\n");

    return 0;
}
