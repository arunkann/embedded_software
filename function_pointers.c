#include <stdio.h>
#include <math.h>


void test(unsigned int jump_index, double a, double b)
{
    static double (*pf[])(double, double) = {pow, pow, pow, pow};

    if (jump_index < sizeof(pf) / sizeof(*pf))
    {
        // Call the function specified by jump_index
        printf("result: %f\n", pf[jump_index](a, b));
    }
}

int main()
{
    double result;
    double (*funcPtr) (double, double);

    funcPtr = pow; 		// funcptr = &pow;

    result = (funcPtr)(2.0, 3.0);	 // result = (*funcPtr)(1.5, 2.0)

    printf("result: %f\n", result);

    test(2, 2.0, 4.0);

    return 0;
}
