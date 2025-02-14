#define ACCEL_SENSOR_SLAVE_ADDR 		0x19
#define MAGNET_SENSOR_SLAVE_ADDR 		0x1E

#define reg8( r ) unsigned char r
#define reg16( r ) unsigned short r
// LSM303AGR Register map
#define STATUS_REG_AUX_A_REG 		0x07
#define OUT_TEMP_A_REG 				0x0C
#define WHO_AM_I_A_REG				0x0F
#define CTRL_REG1_A_REG				0x20
#define CTRL_REG2_A_REG				0x21
#define CTRL_REG3_A_REG				0x22
#define CTRL_REG4_A_REG				0x23
#define OUT_X_A_REG					0x28
#define OUTX_REG_M_REG				0x68

typedef volatile struct
{
    reg8(RVSD0);                    // 0x00
    reg8(RVSD1);                    // 0x01
    reg8(RVSD2);                    // 0x02
    reg8(RVSD3);                    // 0x03
    reg8(RVSD4);                    // 0x04
    reg8(RVSD5);                    // 0x05
    reg8(RVSD6);                    // 0x06
    reg8(STATUS_REG_AUX_A);         // 0x07
    reg8(RVSD8);    // 0x08
    reg8(RVSD9);    // 0x09
    reg8(RVSDA);    // 0x0A
    reg8(RVSDB);    // 0x0B
    reg16(OUT_TEMP_A);              // 0x0C - 0X0D
    reg8(INT_COUNTER_REG_A);        // 0x0E
    reg8(WHO_AM_I_A);               // 0x0F
    reg8(RVSD10);   // 0x10
    reg8(RVSD11);   // 0x11
    reg8(RVSD12);   // 0x12
    reg8(RVSD13);   // 0x13
    reg8(RVSD14);   // 0x14
    reg8(RVSD15);   // 0x15
    reg8(RVSD16);   // 0x16
    reg8(RVSD17);   // 0x17
    reg8(RVSD18);   // 0x18
    reg8(RVSD19);   // 0x19
    reg8(RVSD1A);   // 0x1A
    reg8(RVSD1B);   // 0x1B
    reg8(RVSD1C);   // 0x1C
    reg8(RVSD1D);   // 0x1D
    reg8(RVSD1E);   // 0x1E
    reg8(TEMP_CFG_REG_A);           // 0x1F
    reg8(CTRL_REG1_A);              // 0x20
    reg8(CTRL_REG2_A);              // 0x21
    reg8(CTRL_REG3_A);              // 0x22
    reg8(CTRL_REG4_A);              // 0x23
    reg8(CTRL_REG5_A);              // 0x24
    reg8(CTRL_REG6_A);              // 0x25
    reg8(REFERENCE_A);              // 0x26
    reg8(STATUS_REG_A);             // 0x27
    reg16(OUT_X_A);                 // 0x28 - 0x29
    reg16(OUT_Y_A);                 // 0x2A - 0x2B
    reg16(OUT_Z_A);                 // 0x2C - 0x2D
    reg8(FIFO_CTRL_REG_A);          // 0x2E
    reg8(FIFO_SRC_REG_A);           // 0x2F
    reg8(INT1_CFG_A);               // 0x30
    reg8(INT1_SRC_A);               // 0x31
    reg8(INT1_THS_A);               // 0x32
    reg8(INT1_DURATION_A);          // 0x33
    reg8(INT2_CFG_A);               // 0x34
    reg8(INT2_SRC_A);               // 0x35
    reg8(INT2_THS_A);               // 0x36
    reg8(INT2_DURATION_A);          // 0x37
    reg8(CLICK_CFG_A);              // 0x38
    reg8(CLICK_SRC_A);              // 0x39
    reg8(CLICK_THS_A);              // 0x3A
    reg8(TIME_LIMIT_A);             // 0x3B
    reg8(TIME_LATENCY_A);           // 0x3C
    reg8(TIME_WINDOW_A);            // 0x3D
    reg8(ACT_THS_A);                // 0x3E
    reg8(ACT_DUR_A);                // 0x3F
    reg8(RVSD40);   // 0x40
    reg8(RVSD41);   // 0x41
    reg8(RVSD42);   // 0x42
    reg8(RVSD43);   // 0x43
    reg8(RVSD44);   // 0x44
    reg16(OFFSET_X_REG_M);          // 0x45-0x46
    reg16(OFFSET_Y_REG_M);          // 0x47-0x48
    reg16(OFFSET_Z_REG_M);          // 0x49-0x4A
    reg8(RVSD4B);   // 0x4B
    reg8(RVSD4C);   // 0x4C
    reg8(RVSD4D);   // 0x4D
    reg8(RVSD4E);   // 0x4E
    reg8(WHO_AM_I_M);               // 0x4F
    reg8(RVSD50);   // 0x50
    reg8(RVSD51);   // 0x51
    reg8(RVSD52);   // 0x52
    reg8(RVSD53);   // 0x53
    reg8(RVSD54);   // 0x54
    reg8(RVSD55);   // 0x55
    reg8(RVSD56);   // 0x56
    reg8(RVSD57);   // 0x57
    reg8(RVSD58);   // 0x58
    reg8(RVSD59);   // 0x59
    reg8(RVSD5A);   // 0x5A
    reg8(RVSD5B);   // 0x5B
    reg8(RVSD5C);   // 0x5C
    reg8(RVSD5D);   // 0x5D
    reg8(RVSD5E);   // 0x5E
    reg8(RVSD5F);   // 0x5F
    reg8(CFG_REG_A_M);              // 0x60
    reg8(CFG_REG_B_M);              // 0x61
    reg8(CFG_REG_C_M);              // 0x62
    reg8(INT_CTRL_REG_M);           // 0x63
    reg8(INT_SOURCE_REG_M);         // 0x64
    reg8(INT_THS_L_REG_M);          // 0x65
    reg8(INT_THS_H_REG_M);          // 0x66
    reg8(STATUS_REG_M);             // 0x67
    reg16(OUTX_REG_M);              // 0x68-0x69
    reg16(OUTY_REG_M);              // 0x6A-0x6B
    reg16(OUTZ_REG_M);              // 0x6C-0x6D
    reg8(RVSD6E);   // 0x6E
    reg8(RVSD6F);   // 0x6F
} lsm303agr_regs;


#define LSM303AGR     ((lsm303agr_regs*)0x00U)

