#include <stdio.h>

#define ARRAY_LEN 100
#define STRLEN_MAX 256

/* Note this example uses strings. But it could as well be structs. */

/* this is an array of pointers of type char* */
char *myStrPtr[ARRAY_LEN] = {
    "If anything can go wrong, it will.",
    "Nothing is foolproof, because fools are so ingenious.",
    "Every solution breeds new problems."
};

int main()
{
    char **p = myStrPtr; /* name of array is a pointer to the type of content */
    int n = 3;

    /* dereference to get char* */
    for (int i=0; i<n; ++i) {
        printf("%s\n", myStrPtr[i]);
    }

    for (; p<myStrPtr+n; ++p) {
        printf("%s\n", *p); //*p dereferences to char* - which is content of the array
    }
}
