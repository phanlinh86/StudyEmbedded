//
// Created by Phan Linh on 25/12/2024.
//

#ifndef MCUATMEGA328P_H
#define MCUATMEGA328P_H

#define FPU 							16000000

#define IO_BASE                         0x20U


#define reg8( r ) unsigned char r


/****************************************************************************
*									STM32 GPIOA_BASE						*
*									General Purpose I/O						*
*****************************************************************************/
// Field map
typedef volatile union
{
    uint8_t Register;
    struct
    {
        uint8_t PIN0:1;
        uint8_t PIN1:1;
        uint8_t PIN2:1;
        uint8_t PIN3:1;
        uint8_t PIN4:1;
        uint8_t PIN5:1;
        uint8_t PIN6:1;
        uint8_t PIN7:1;
    };
} PIN_reg;

typedef volatile union
{
    uint8_t Register;
    struct
    {
        uint8_t DDR0:1;
        uint8_t DDR1:1;
        uint8_t DDR2:1;
        uint8_t DDR3:1;
        uint8_t DDR4:1;
        uint8_t DDR5:1;
        uint8_t DDR6:1;
        uint8_t DDR7:1;
    };
} DDR_reg;

typedef volatile union
{
    uint8_t Register;
    struct
    {
        uint8_t PORT0:1;
        uint8_t PORT1:1;
        uint8_t PORT2:1;
        uint8_t PORT3:1;
        uint8_t PORT4:1;
        uint8_t PORT5:1;
        uint8_t PORT6:1;
        uint8_t PORT7:1;
    };
} PORT_reg;

typedef volatile union
{
	uint32_t Register;
	struct {
		uint8_t TOV:1;
		uint8_t OCFA:1;
		uint8_t OCFB:1;
		uint8_t RVSD:5;
	};
} TIFR_reg;

typedef volatile union
{
	uint32_t Register;
	struct {
		uint8_t PCIF0:1;
		uint8_t PCIF1:1;
		uint8_t PCIF2:1;
		uint8_t RVSD:5;
	};
} PCIFR_reg;

typedef volatile union
{
	uint32_t Register;
	struct {
		uint8_t INTF0:1;
		uint8_t INTF1:1;
		uint8_t RVSD:6;
	};
} EIFR_reg;

typedef volatile union
{
	uint32_t Register;
	struct {
		uint8_t INT0:1;
		uint8_t INT1:1;
		uint8_t RVSD:6;
	};
} EIMSK_reg; 

// Whole Register map
typedef volatile struct
{
    reg8(RSVD0);        // 0x00
    reg8(RSVD1);        // 0x01
    reg8(RSVD2);        // 0x02
    reg8(PINB);         // 0x03
    reg8(DDRB);         // 0x04
    reg8(PORTB);        // 0x05
    reg8(PINC);         // 0x06
    reg8(DDRC);         // 0x07
    reg8(PORTC);        // 0x08
    reg8(PIND);         // 0x09
    reg8(DDRD);         // 0x0A
    reg8(PORTD);        // 0x0B
    reg8(RSVDC);    	// 0x0C
    reg8(RSVDD);    	// 0x0D
    reg8(RSVDE);    	// 0x0E
    reg8(RSVDF);    	// 0x0F
    reg8(RSVD10);   	// 0x10
    reg8(RSVD11);   	// 0x11
    reg8(RSVD12);   	// 0x12
    reg8(RSVD13);   	// 0x13
    reg8(RSVD14);   	// 0x14
    TIFR_reg TIFR0;   	// 0x15
    TIFR_reg TIFR1;   	// 0x16
    TIFR_reg TIFR2;   	// 0x17
    reg8(RSVD18);       // 0x18
    reg8(RSVD19);       // 0x19
    reg8(RSVD1A);       // 0x1A
    PCIFR_reg PCIFR;    // 0x1B
    EIFR_reg EIFR;      // 0x1C
    EIMSK_reg EIMSK;    // 0x1D
    reg8(GPIOR0);       // 0x1E
    reg8(EECR);         // 0x1F
    reg8(EEDR);         // 0x20
    reg8(EEARL);        // 0x21
    reg8(EEARH);        // 0x22
    reg8(GTCCR);        // 0x23
    reg8(TCCR0A);       // 0x24
    reg8(TCCR0B);       // 0x25
    reg8(TCNT0);        // 0x26
    reg8(OCR0A);        // 0x27
    reg8(OCR0B);        // 0x28
    reg8(RSVD29);       // 0x29
    reg8(GPIOR1);       // 0x2A
    reg8(GPIOR2);       // 0x2B
    reg8(RSVD2C);   // 0x2C
    reg8(RSVD2D);   // 0x2D
    reg8(RSVD2E);   // 0x2E
    reg8(RSVD2F);   // 0x2F
    reg8(RSVD30);   // 0x30
    reg8(RSVD31);   // 0x31
    reg8(RSVD32);   // 0x32
    reg8(RSVD33);   // 0x33
    reg8(RSVD34);   // 0x34
    reg8(RSVD35);   // 0x35
    reg8(RSVD36);   // 0x36
    reg8(RSVD37);   // 0x37
    reg8(RSVD38);   // 0x38
    reg8(RSVD39);   // 0x39
    reg8(RSVD3A);   // 0x3A
    reg8(RSVD3B);   // 0x3B
    reg8(RSVD3C);   // 0x3C
    reg8(RSVD3D);   // 0x3D
    reg8(RSVD3E);   // 0x3E
    reg8(RSVD3F);   // 0x3F
    reg8(RSVD40);   // 0x40
    reg8(RSVD41);   // 0x41
    reg8(RSVD42);   // 0x42
    reg8(RSVD43);   // 0x43
    reg8(RSVD44);   // 0x44
    reg8(RSVD45);   // 0x45
    reg8(RSVD46);   // 0x46
    reg8(RSVD47);   // 0x47
    reg8(RSVD48);   // 0x48
    reg8(RSVD49);   // 0x49
    reg8(RSVD4A);   // 0x4A
    reg8(RSVD4B);   // 0x4B
    reg8(RSVD4C);   // 0x4C
    reg8(RSVD4D);   // 0x4D
    reg8(RSVD4E);   // 0x4E
    reg8(RSVD4F);   // 0x4F
    reg8(RSVD50);   // 0x50
    reg8(RSVD51);   // 0x51
    reg8(RSVD52);   // 0x52
    reg8(RSVD53);   // 0x53
    reg8(RSVD54);   // 0x54
    reg8(RSVD55);   // 0x55
    reg8(RSVD56);   // 0x56
    reg8(RSVD57);   // 0x57
    reg8(RSVD58);   // 0x58
    reg8(RSVD59);   // 0x59
    reg8(RSVD5A);   // 0x5A
    reg8(RSVD5B);   // 0x5B
    reg8(RSVD5C);   // 0x5C
    reg8(RSVD5D);   // 0x5D
    reg8(RSVD5E);   // 0x5E
    reg8(RSVD5F);   // 0x5F
    reg8(RSVD60);   // 0x60
    reg8(RSVD61);   // 0x61
    reg8(RSVD62);   // 0x62
    reg8(RSVD63);   // 0x63
    reg8(RSVD64);   // 0x64
    reg8(RSVD65);   // 0x65
    reg8(RSVD66);   // 0x66
    reg8(RSVD67);   // 0x67
    reg8(RSVD68);   // 0x68
    reg8(RSVD69);   // 0x69
    reg8(RSVD6A);   // 0x6A
    reg8(RSVD6B);   // 0x6B
    reg8(RSVD6C);   // 0x6C
    reg8(RSVD6D);   // 0x6D
    reg8(RSVD6E);   // 0x6E
    reg8(RSVD6F);   // 0x6F
    reg8(RSVD70);   // 0x70
    reg8(RSVD71);   // 0x71
    reg8(RSVD72);   // 0x72
    reg8(RSVD73);   // 0x73
    reg8(RSVD74);   // 0x74
    reg8(RSVD75);   // 0x75
    reg8(RSVD76);   // 0x76
    reg8(RSVD77);   // 0x77
    reg8(RSVD78);   // 0x78
    reg8(RSVD79);   // 0x79
    reg8(RSVD7A);   // 0x7A
    reg8(RSVD7B);   // 0x7B
    reg8(RSVD7C);   // 0x7C
    reg8(RSVD7D);   // 0x7D
    reg8(RSVD7E);   // 0x7E
    reg8(RSVD7F);   // 0x7F
    reg8(RSVD80);   // 0x80
    reg8(RSVD81);   // 0x81
    reg8(RSVD82);   // 0x82
    reg8(RSVD83);   // 0x83
    reg8(RSVD84);   // 0x84
    reg8(RSVD85);   // 0x85
    reg8(RSVD86);   // 0x86
    reg8(RSVD87);   // 0x87
    reg8(RSVD88);   // 0x88
    reg8(RSVD89);   // 0x89
    reg8(RSVD8A);   // 0x8A
    reg8(RSVD8B);   // 0x8B
    reg8(RSVD8C);   // 0x8C
    reg8(RSVD8D);   // 0x8D
    reg8(RSVD8E);   // 0x8E
    reg8(RSVD8F);   // 0x8F
    reg8(RSVD90);   // 0x90
    reg8(RSVD91);   // 0x91
    reg8(RSVD92);   // 0x92
    reg8(RSVD93);   // 0x93
    reg8(RSVD94);   // 0x94
    reg8(RSVD95);   // 0x95
    reg8(RSVD96);   // 0x96
    reg8(RSVD97);   // 0x97
    reg8(RSVD98);   // 0x98
    reg8(RSVD99);   // 0x99
    reg8(RSVD9A);   // 0x9A
    reg8(RSVD9B);   // 0x9B
    reg8(RSVD9C);   // 0x9C
    reg8(RSVD9D);   // 0x9D
    reg8(RSVD9E);   // 0x9E
    reg8(RSVD9F);   // 0x9F
    reg8(RSVDA0);   // 0xA0
    reg8(RSVDA1);   // 0xA1
    reg8(RSVDA2);   // 0xA2
    reg8(RSVDA3);   // 0xA3
    reg8(RSVDA4);   // 0xA4
    reg8(RSVDA5);   // 0xA5
    reg8(RSVDA6);   // 0xA6
    reg8(RSVDA7);   // 0xA7
    reg8(RSVDA8);   // 0xA8
    reg8(RSVDA9);   // 0xA9
    reg8(RSVDAA);   // 0xAA
    reg8(RSVDAB);   // 0xAB
    reg8(RSVDAC);   // 0xAC
    reg8(RSVDAD);   // 0xAD
    reg8(RSVDAE);   // 0xAE
    reg8(RSVDAF);   // 0xAF
    reg8(RSVDB0);   // 0xB0
    reg8(RSVDB1);   // 0xB1
    reg8(RSVDB2);   // 0xB2
    reg8(RSVDB3);   // 0xB3
    reg8(RSVDB4);   // 0xB4
    reg8(RSVDB5);   // 0xB5
    reg8(RSVDB6);   // 0xB6
    reg8(RSVDB7);   // 0xB7
    reg8(RSVDB8);   // 0xB8
    reg8(RSVDB9);   // 0xB9
    reg8(RSVDBA);   // 0xBA
    reg8(RSVDBB);   // 0xBB
    reg8(RSVDBC);   // 0xBC
    reg8(RSVDBD);   // 0xBD
    reg8(RSVDBE);   // 0xBE
    reg8(RSVDBF);   // 0xBF
    reg8(RSVDC0);   // 0xC0
    reg8(RSVDC1);   // 0xC1
    reg8(RSVDC2);   // 0xC2
    reg8(RSVDC3);   // 0xC3
    reg8(RSVDC4);   // 0xC4
    reg8(RSVDC5);   // 0xC5
    reg8(RSVDC6);   // 0xC6
    reg8(RSVDC7);   // 0xC7
    reg8(RSVDC8);   // 0xC8
    reg8(RSVDC9);   // 0xC9
    reg8(RSVDCA);   // 0xCA
    reg8(RSVDCB);   // 0xCB
    reg8(RSVDCC);   // 0xCC
    reg8(RSVDCD);   // 0xCD
    reg8(RSVDCE);   // 0xCE
    reg8(RSVDCF);   // 0xCF
    reg8(RSVDD0);   // 0xD0
    reg8(RSVDD1);   // 0xD1
    reg8(RSVDD2);   // 0xD2
    reg8(RSVDD3);   // 0xD3
    reg8(RSVDD4);   // 0xD4
    reg8(RSVDD5);   // 0xD5
    reg8(RSVDD6);   // 0xD6
    reg8(RSVDD7);   // 0xD7
    reg8(RSVDD8);   // 0xD8
    reg8(RSVDD9);   // 0xD9
    reg8(RSVDDA);   // 0xDA
    reg8(RSVDDB);   // 0xDB
    reg8(RSVDDC);   // 0xDC
    reg8(RSVDDD);   // 0xDD
    reg8(RSVDDE);   // 0xDE
    reg8(RSVDDF);   // 0xDF
    reg8(RSVDE0);   // 0xE0
    reg8(RSVDE1);   // 0xE1
    reg8(RSVDE2);   // 0xE2
    reg8(RSVDE3);   // 0xE3
    reg8(RSVDE4);   // 0xE4
    reg8(RSVDE5);   // 0xE5
    reg8(RSVDE6);   // 0xE6
    reg8(RSVDE7);   // 0xE7
    reg8(RSVDE8);   // 0xE8
    reg8(RSVDE9);   // 0xE9
    reg8(RSVDEA);   // 0xEA
    reg8(RSVDEB);   // 0xEB
    reg8(RSVDEC);   // 0xEC
    reg8(RSVDED);   // 0xED
    reg8(RSVDEE);   // 0xEE
    reg8(RSVDEF);   // 0xEF
    reg8(RSVDF0);   // 0xF0
    reg8(RSVDF1);   // 0xF1
    reg8(RSVDF2);   // 0xF2
    reg8(RSVDF3);   // 0xF3
    reg8(RSVDF4);   // 0xF4
    reg8(RSVDF5);   // 0xF5
    reg8(RSVDF6);   // 0xF6
    reg8(RSVDF7);   // 0xF7
    reg8(RSVDF8);   // 0xF8
    reg8(RSVDF9);   // 0xF9
    reg8(RSVDFA);   // 0xFA
    reg8(RSVDFB);   // 0xFB
    reg8(RSVDFC);   // 0xFC
    reg8(RSVDFD);   // 0xFD
    reg8(RSVDFE);   // 0xFE
    reg8(RSVDFF);   // 0xFF
} reg;


#define REG     ((reg*)IO_BASE)


#endif //MCUSTM32F411E_H
