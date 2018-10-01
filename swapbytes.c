#include <stdio.h>

void swap_bytes(unsigned int a)
{
    int i;
    int size = sizeof(a);
    char temp;
    char *a_temp = (char*) &a;

    for (i=0; i<size/2; i++)
    {
        temp = a_temp[i];
        a_temp[i] = a_temp[size-1-i];
        a_temp[size-1-i] = temp;
    }

    printf("a value 0x%x\n", a);
}


int main()
{
    unsigned int value = 0xa2876542;
    swap_bytes(value);

    return 0;
}
