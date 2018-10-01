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

#define CAN_BASE            ((unsigned)(&can_reg_instance)) /* This is obtained through ioremap() in kernel */
#define CAN_MSGID_OFFSET    (0x0)
#define CAN_MSGID_SHIFT     0
#define CAN_MSGID_MASK      (0xFFFF)
#define CAN_STS_OFFSET      (0x4)
#define CAN_STS_SHIFT       16
#define CAN_STS_MASK        (0xFFFF)
#define CAN_BUS_OFFSET      (0x4)
#define CAN_BUS_SHIFT       8
#define CAN_BUS_MASK        (0xFF)
#define CAN_LEN_OFFSET      (0x4)
#define CAN_LEN_SHIFT       0
#define CAN_LEN_MASK        (0xFF)
#define CAN_DATA0_OFFSET    (0x8)
#define CAN_DATA1_OFFSET    (0xC)

void writeb(unsigned value, unsigned address)
{
    *(uint8_t*) address = (uint8_t)value;
}

void writew(unsigned value, unsigned address)
{
    *(uint16_t*) address = (uint16_t)value;
}

void writel(unsigned value, unsigned address)
{
    *(uint32_t*) address = (uint32_t)value;
}

int can_write(uint16_t msg_id, uint8_t bus, uint8_t len, uint8_t* dataptr)
{
    int i;

    if (!(can_reg_instance.status & 0x1)) {
        printf("not busy\n");

        writew((msg_id & CAN_MSGID_MASK), CAN_BASE+CAN_MSGID_OFFSET);

        writel(((len & CAN_LEN_MASK) << CAN_LEN_SHIFT) |
               ((bus & CAN_BUS_MASK) << CAN_BUS_SHIFT) |
               ((0x2 & CAN_STS_MASK) << CAN_STS_SHIFT),
               CAN_BASE+CAN_STS_OFFSET);

        for (i=0; i<len; i++) {
            if (i<4)
                writeb(dataptr[i], CAN_BASE+CAN_DATA0_OFFSET+i);
            else
                writeb(dataptr[i], CAN_BASE+CAN_DATA1_OFFSET+(i-4));
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
