/* solution to */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void* aligned_malloc(size_t required_bytes, size_t alignment)
{
    void* p1;
    void* p2;
    int offset = 0;

    if (alignment == 0)
            alignment = 1;

    /* Pad for alignment + storage for orig malloc returned address.
     * Even if no alignement is needed, you need to allocate for
     * extra address. */
    offset = (alignment - 1) + sizeof(void*);
    printf("offset: %d\n", offset);
    p1 = (void*)malloc(required_bytes + offset);
    printf("malloc returns: 0x%p\n", p1);

    if (p1 == NULL) {
        return NULL;
    }

    /* Add the offset no matter what to save the orig address */
    p2 = (void*)((size_t)(p1) + offset);
    printf("malloc + offset: 0x%p\n", p2);
    /* Align the address, now its both aligned and
     * has space for extra address. */
    p2 = (void*)((size_t)p2 & ~(alignment -1));
    printf("aligned address: 0x%p\n", p2);


    /* p2 is void* so can't dereference.
       Instead treat it as void** i.e. pointer to array of void*.
       This allows you to go back by 1 and save p1 in an earlier slot. */
    ((void **)p2)[-1] = p1;
    /* Alternatively, treat it as pointer type */
    //*(((uintptr_t*)p2)-1) = (uintptr_t)p1;
    printf("free address loc: 0x%p\n", ((void **)p2)[-1]);

    return p2;
}

void aligned_free(void *p2)
{
    void* p1 = ((void**)p2)[-1];
    free(p1);
}

int main()
{
    void* ptr;
    int alignment = 8;
    ptr = aligned_malloc(100, alignment);

    printf("Aligned at %d malloc address: 0x%p\n", alignment, ptr);

    aligned_free(ptr);
}
