#include <stdio.h>

int main()
{
    unsigned int a = 1; /* this spans multiple bytes i.e. multiple memory address locations */
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
