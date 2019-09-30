#include <stdio.h>

/* Visual:
 *
 *    Say lsbpos = 5 and msbpos = 25
 * 
 *    31 30 39 28  27 26 25 24  23 22 21 20  19 18 17 16
 *     1  1  1  1   1  1  1  1
 *
 *    15 14 13 12  11 10 09 08  07 06 05 04  03 02 01 00
 *                                        1   1  1  1  1
 *
 *    With this mask, you can AND to clear out in-between.
 *
 */

int update_bits(int val, int field, int msb_pos, int lsb_pos)
{
    int mask1 = (1 << lsb_pos) - 1; /* 1's from 0 to lsb_pos */
    int mask2 = -1 << (msb_pos+1); /* 1's from MSB to msb_pos+1 */
    int mask = mask1 | mask2; /* all 1's except field bits */

    /* shift field to appropriate location */
    field = field << lsb_pos;

    /* Clear the Val where field has to be updated and update the field. */
    val = (val & mask) | field;

    return val;
}

int main()
{
    int val = 0xF0F00000, updated_val;
    int field = 0x1;
    int lsb_pos = 20;
    int msb_pos = 23;

    /* insert field */
    updated_val = update_bits(val, field, msb_pos, lsb_pos);

    printf("0x%x updated to %x\n", val, updated_val);

    return 0;
}
