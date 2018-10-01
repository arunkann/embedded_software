#include <stdio.h>

int binary_search(int *aptr, int size, int value)
{
    int start_idx = 0, end_idx = size-1;
    int middle_idx = size/2;

    while(end_idx-start_idx > 1)
    {
        //printf("start: %d %d %d\n", start_idx, end_idx, middle_idx);

        if (aptr[middle_idx] > value) {
            end_idx = middle_idx;
        }
        else if (aptr[middle_idx] < value) {
            start_idx = middle_idx;
        }
        else /* same value */
            return middle_idx;

        middle_idx = start_idx + (end_idx-start_idx)/2;

        //printf("end: %d %d %d\n", start_idx, end_idx, middle_idx);
    }

    if (aptr[start_idx] == value)
        return start_idx;
    if (aptr[end_idx] == value)
        return end_idx;

    return -1;
}

int main()
{
    int a[] = {1, 3, 9, 12, 15, 20, 100};
    //int a[] = {1, 3, 9, 12, 15, 16, 20, 100};
    //int a[] = {};
    int value = 3;

    int index = binary_search(a, sizeof(a)/sizeof(a[0]), value);

    printf("index found: %d", index);

    return 0;
}
