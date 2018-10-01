#include <stdio.h>

void twos_complement(int num)
{
    int size  = sizeof(num);
    char *aptr = (char*) &num;

    printf("Twos complement representation: 0x");

    for (int i = size-1; i >= 0; i--)
        printf("%02x", aptr[i]);

    printf("\n");
}

int main()
{
    unsigned int num = 127;

    printf("Positive num: %d\n", num);
    twos_complement(num);

    /* formula to convert positive num to negative num */
    int negnum = ~num + 1;

    printf("Negative num: %d\n", negnum);
    twos_complement(negnum);

    return 0;
}
