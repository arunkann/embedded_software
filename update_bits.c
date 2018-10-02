#include <stdio.h>

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
