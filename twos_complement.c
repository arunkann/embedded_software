#include <stdio.h>

void twos_complement(int num)
{
    int size  = sizeof(num);
    char *aptr = (char*) &num; /* this assumes LE machine */
    int i;

    printf("Twos complement representation: 0x");

    for (i = size-1; i >= 0; i--)
        printf("%x", aptr[i] & 0xff);

    printf("\n");
}


/* With B bits, we can represent
 * 2powB-1 positive numbers and 2powB negative numbers
 * using 2's complement notation.
 * This notation allows same arithmetic circuitry
 * to be used to be used for postive and negative numbers.
 * Negative numbers have MSB set to 1.
 */
int main()
{
    /* Lets do 125 - 127 = -2 */
    int num1 = 125;
    int num2 = 127;

    printf("Positive num: %d\n", num1);
    twos_complement(num1);
    printf("Positive num: %d\n", num2);
    twos_complement(num2);

    /* formula to convert positive num to negative num */
    /* Do complement and add 1 */
    int negnum = ~num2 + 1;

    printf("Negative num: %d\n", negnum);
    twos_complement(negnum);

    int sum = num1 + negnum;
    printf("Sum: %d\n", sum);
    twos_complement(sum);

    /* formula to convert negative num to positive num */
    /* Do complement and add 1 */
    int posnum = ~sum + 1;
    printf("Positive num: %d\n", posnum);
    twos_complement(posnum);

    return 0;
}
