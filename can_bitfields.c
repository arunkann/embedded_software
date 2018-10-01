/*

This is the memory mapped version of CAN.

        *********************************
0x1000  *   rsvd:16     * msg_id:16     *
        *********************************
0x1004  *   status: 16 * bus: 8 * len 8 *
        *********************************
0x1008          Data 0: 32
        *********************************
0x100c          Data 1: 32
        *********************************

status: txrdy: 1 and txbusy: 0

*/

#include <stdint.h>
#include <stdio.h>

struct can_regs {
        uint16_t msg_id:16;
        uint16_t rsvd:16;
        uint8_t len:8;
        uint8_t bus:8;
        uint16_t status:16;
        uint32_t data0;
        uint32_t data1;
};

struct can_regs can_reg_instance;

int can_write(uint16_t msg_id, uint8_t bus, uint8_t len, uint8_t* dataptr)
{
    int i;

    if (!(can_reg_instance.status & 0x1)) {
        printf("not busy\n");
        can_reg_instance.msg_id = msg_id;
        can_reg_instance.bus = bus;
        can_reg_instance.len = len;
        can_reg_instance.status = 0x2;
        for (i=0; i<len; i++) {
            if (i<4)
                *((uint8_t*)&can_reg_instance.data0 + i) = dataptr[i];
            else
                *((uint8_t*)&can_reg_instance.data1 + (i-4)) = dataptr[i];
        }
        return 0;
    }
    else {
        printf("busy\n");
        return -1;
    }
}

int main()
{
    //uint8_t data[] = {0x11, 0x22, 0x33, 0x44};
    uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    /* Init can reg instance */
    can_reg_instance.status = 0x0;
    //can_reg_instance.status = 0x1;

    /* write to can reg */
    can_write(0xAA, 0xB, sizeof(data)/sizeof(data[0]), data);

    /* Print can reg instance */
    printf("msg_id: 0x%x\n", can_reg_instance.msg_id);
    printf("status: 0x%x\n", can_reg_instance.status);
    printf("bus: 0x%x\n", can_reg_instance.bus);
    printf("len: 0x%x\n", can_reg_instance.len);
    printf("data0: 0x%x\n", can_reg_instance.data0);
    printf("data1: 0x%x\n", can_reg_instance.data1);

    return 0;
}
