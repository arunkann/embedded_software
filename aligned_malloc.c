/* solution to */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 
 * Alignment value should be power of 2.
 *     0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 etc.
 * Plus it should be a multiple of sizeof(void*).
 *     That means 0, 2, 4 are invalid.
 *
 * Alignment is in units of bytes.
 * Align by 8 means, last 3 bits of address are 0. So, valid address are 0, 8, 16, 24 i.e. multiple of 8.
 * Align by 4096 means, last 12 bits of address are 0. So, valid addresses are 4k, 8k, 12k etc.
 *
 */

bool is_power_of_2(size_t alignment)
{
    if (alignment == 0)
	return true;

    if (alignment == 1)
	return false;

    while (!(alignment  & 1)) {
        alignment = alignment >> 1;
    }

    alignment = alignment >> 1;

    if (alignment == 0)
	return true;
    else
	return false;
}

void* aligned_malloc(size_t required_bytes, size_t alignment)
{
    void* p1;
    void* p2;
    int offset = 0;
    size_t void_ptr_size = sizeof(void*);

    /* validate alignment */
    if (alignment < void_ptr_size) {
        printf("alignment value too small\n");
	return NULL;
    }
    if (!is_power_of_2(alignment)) {
        printf("alignment not power of 2\n");
	return NULL;
    }

    /* Pad for alignment + storage for orig malloc returned address. */
    /* Example: if align is 8 and orig malloc addr is 9, we need to return 16,
     * as that is the next aligned address.
     * Only way we can do that is if we allocate large enough in the first place.
     * So, (16-9) should be added. In general; (align - 1).
     * We also need to store the orig malloc address; so another 8 bytes for that.
     * So, we alloc for size + 7 + 8 bytes in this case.
     */
    offset = (alignment - 1) + sizeof(void*);
    printf("offset: %d\n", offset);
    p1 = (void*)malloc(required_bytes + offset);
    printf("malloc returns: 0x%p\n", p1);

    if (p1 == NULL) {
        return NULL;
    }

    /* Add the offset no matter what to save the orig address */
    /* At this point, you have can chop off LSB to align as needed */
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
    int alignment = 16;
    ptr = aligned_malloc(100, alignment);

    printf("Aligned at %d malloc address: 0x%p\n", alignment, ptr);

    if (ptr)
        aligned_free(ptr);
    else
        printf("skip free\n");
}
