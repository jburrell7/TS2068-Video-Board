
// these registers are for general purpose use
#define FPGA_TEST_PORT	0xF0
#define FPGA_REG		0xC3
// returns the FPGA version number
#define FPGA_VER		0xC4

// Hardware stack ports
// read/write data from/to the stack
#define DOSSTK_DAT		0xC0
// read/write
// read: 	get the current stack pointer value
// write:	set the current stack pointer value
#define DOSSTKPTRL		0xC1
#define DOSSTKPTRH		0xC2

// SPI peripheral registers
// write: 	send a byte via the SPI peripheral
// read:	get a byte via the SPI peripheral
#define SPI_DREG		0xC8
// various SPI configuration bits
#define SPI_SELREG		0xC9
#define		BIT_SPIMODE		7	// '1' sets SPI mode to 2 else SPI mode 0

#define     MSK_SPIMODE     1 << BIT_SPIMODE
// masks to assert one of the four peripheral select lines
#define		MSK_PSEL0		0x40
#define		MSK_PSEL1		0x41
#define		MSK_PSEL2		0x42
#define		MSK_PSEL3		0x43
//
#define SPI_STAT		0xCA
#define		BIT_SPI_BUSY		0
#define		BIT_SPI_MODE		7

#define		NSK_SPI_BUSY		1 << BIT_SPI_BUSY
#define		NSK_SPI_MODE		1 << BIT_SPI_MODE

#define VDP_MEM_ADR 0xA0
#define VDP_REG_ADR	0xA1


// I/O address of the text mode peripheral
#define TEXT_PORT 0xA2
// bit numbers for use by the Z80 bit instructions
#define		BIT_TXTSTAT_DONE	1
#define		BIT_TXTSTAT_BUSY	6
// masks for using AND/OR instructions
#define		MSK_TXTSTAT_DONE	1 << BIT_TXTSTAT_DONE
#define		MSK_TXTSTAT_BUSY	1 << BIT_TXTSTAT_BUSY


// I/O addresses to access the mamory mapper register set
#define MAP_ADR	0xB0
// I/O address to read/write data to the mapper register set
#define MAP_DAT 0x81

// MAP_ADR values for the home mapping registers
#define		HOME0	0x00
#define		HOME1	0x01
#define		HOME2	0x02
#define		HOME3	0x03
#define		HOME4	0x04
#define		HOME5	0x05
#define		HOME6	0x06
#define		HOME7	0x07
// MAP_ADR values for the home mapping registers
#define		EXROM0	0x08
#define		EXROM1	0x09
#define		EXROM2	0x0A
#define		EXROM3	0x0B
#define		EXROM4	0x0C
#define		EXROM5	0x0D
#define		EXROM6	0x0E
#define		EXROM7	0x0F
// dock mapping
#define     DOCK0	0x10
#define     DOCK1	0x11
#define     DOCK2	0x12
#define     DOCK3	0x13
#define     DOCK4	0x14
#define     DOCK5	0x15
#define     DOCK6	0x16
#define     DOCK7	0x17

// set this bit to write enable the chunk
#define			BIT_CHUNK_WE	6
#define			MSK_CHUNK_WE	1 << BIT_CHUNK_WE
// set this bit to allow a chunk to be mappable
#define			BIT_CHUNK_MAPE	7
#define			MSK_CHUNK_MAPE	1 << BIT_CHUNK_MAPE


// MAP_ADR value to access the mapper control register
#define		REG_MAPCTL	0x18
// mapper control register bits
// setting one or more of these bits enables the corresponding
//	banks for mapping

// bit numbers for use by the Z80 bit instructions
#define			BIT_ENAHOME		0
#define			BIT_ENAEXROM	1
#define			BIT_ENADOCK		2
// masks for using AND/OR instructions
#define			MSK_ENAHOME		(1 << BIT_ENAHOME)
#define			MSK_ENAEXROM	(1 << BIT_ENAEXROM)
#define			MSK_ENADOCK		(1 << BIT_ENADOCK)



// I/O addresses to access the ULA Plus register set
//	This I/O space is used for the ULA Plus as well
//	as other functions
#define ULA_ADR 0xBF3B
#define ULA_DAT 0xFF3B

#define ULA_PALA0		0x00
#define 	ULA_PALA1	0x01
#define 	ULA_PALA2	0x02
#define 	ULA_PALA3	0x03
#define 	ULA_PALA4	0x04
#define 	ULA_PALA5	0x05
#define 	ULA_PALA6	0x06
#define 	ULA_PALA7	0x07
#define 	ULA_PALA8	0x08
#define 	ULA_PALA9	0x09
#define 	ULA_PALAA	0x0A
#define 	ULA_PALAB	0x0B
#define 	ULA_PALAC	0x0C
#define 	ULA_PALAD	0x0D
#define 	ULA_PALAE	0x0E
#define 	ULA_PALAF	0x0F

#define ULA_PALB0		0x10
#define 	ULA_PALB1	0x11
#define 	ULA_PALB2	0x12
#define 	ULA_PALB3	0x13
#define 	ULA_PALB4	0x14
#define 	ULA_PALB5	0x15
#define 	ULA_PALB6	0x16
#define 	ULA_PALB7	0x17
#define 	ULA_PALB8	0x18
#define 	ULA_PALB9	0x19
#define 	ULA_PALBA	0x1A
#define 	ULA_PALBB	0x1B
#define 	ULA_PALBC	0x1C
#define 	ULA_PALBD	0x1D
#define 	ULA_PALBE	0x1E
#define 	ULA_PALBF	0x1F

#define ULA_PALC0		0x20
#define 	ULA_PALC1	0x21
#define 	ULA_PALC2	0x22
#define 	ULA_PALC3	0x23
#define 	ULA_PALC4	0x24
#define 	ULA_PALC5	0x25
#define 	ULA_PALC6	0x26
#define 	ULA_PALC7	0x27
#define 	ULA_PALC8	0x28
#define 	ULA_PALC9	0x29
#define 	ULA_PALCA	0x2A
#define 	ULA_PALCB	0x2B
#define 	ULA_PALCC	0x2C
#define 	ULA_PALCD	0x2D
#define 	ULA_PALCE	0x2E
#define 	ULA_PALCF	0x2F

#define ULA_PALD0		0x30
#define 	ULA_PALD1	0x31
#define 	ULA_PALD2	0x32
#define 	ULA_PALD3	0x33
#define 	ULA_PALD4	0x34
#define 	ULA_PALD5	0x35
#define 	ULA_PALD6	0x36
#define 	ULA_PALD7	0x37
#define 	ULA_PALD8	0x38
#define 	ULA_PALD9	0x39
#define 	ULA_PALDA	0x3A
#define 	ULA_PALDB	0x3B
#define 	ULA_PALDC	0x3C
#define 	ULA_PALDD	0x3D
#define 	ULA_PALDE	0x3E
#define 	ULA_PALDF	0x3F

#define	ULA_ENA		0x40
#define 	BIT_ULA_ENA		0
#define 	MSK_ULA_ENA		(1 << ULA_ENA)


#define UR_EXMODE	0x80
// set this bit to enable the extended modes
#define		BIT_ENAEXT	7
// extended mode standard 64 column attribute
#define		MSK_ENAEXT 	  (1 << BIT_ENAEXT)

#define		MSK_XINKBLACK (0x0 << 3)
#define		MSK_XINKBLUE  (0x1 << 3)
#define		MSK_XINKRED   (0x2 << 3)
#define		MSK_XINKMAGEN (0x3 << 3)
#define		MSK_XINKGREEN (0x4 << 3)
#define		MSK_XINKCYAN  (0x5 << 3)
#define		MSK_XINKYELL  (0x6 << 3)
#define		MSK_XINKWHITE (0x7 << 3)
// extended mode vide modes
#define		MSK_XMODES0		0x00		// display file 1
#define		MSK_XMODES1		0x00		// display file 2
#define		MSK_XHICLR		0x02		// high color mode
#define		MSK_XHIRES		0x06		// 512x192 hires mode
#define		MSK_XHIRESP		0x0E		// 512x192 hires with attributes
#define		MSK_XCOLOR4		0x0F		// 4 color 256x192 mode
#define 	MSK_XCOLOR16	0x0B		// 16 color 128 x 192 mode


#define UR_COLOR	0x81	// upper 8 bits of the 9 bit ULA color value
#define UR_BLULOW	0x82	// low bit of the ULA blue
#define UR_EXPBORD	0x83	// expanded mode border color
#define UR_SPECYBLK	0x84	// '1' enables Spectrum black (only one shade of black)
#define UR_CLR4BNK	0x85	// upper 4 bits to access the ULA bank for 4 bit color


// transparent color byte: gggrrrbb
#define UR_TRANSCLR	0x86
// selects the video output mux control
#define UR_VMODEA	0x87
#define UR_VMODEB	0x88
#define 	MODETS	0x00	// select 2068 video
#define		MODEHR	0x01	// select hires videl
#define		MODETH	0x02	// timex foreground, hires background
#define		MODEHT	0x03	// hires foreground, timex background
#define		MODEVDP	0x40	// select VDP9918
#define		MODETXT	0x80	// select text mode

#define UR_HRDAT	0xA0		// port to read/write to hires screen buffer
#define UR_HRCMD	0xA1		// hires command. writing to this port executes the command
#define		HRC_NOP		0x00
#define		HRC_FILL	0x01
#define		HRC_COPY	0x02
#define		HRC_BTOS	0x03
#define		HRC_STOB	0x04

// x and y coordinates of the upper left
//	corner of the desired rectangle
#define UR_HRULX	0xA2
#define UR_HRULY	0xA3
// x and y coordinates of the lower right
//	corner of the desired rectangle
#define UR_HRLRX	0xA4
#define UR_HRLRY	0xA5

// x and y coordinates of the upper left
//	corner of the destination of a copy
//	command
#define UR_HRDESTX	0xA6
#define UR_HRDESTY	0xA7
// fill color
#define UR_HRFILL	0xA8

// set operation for fill and copy
//		commands
#define UR_HROP		0xA9
#define		HRNOP		0x00
#define		HRAND		0x01
#define		HROR		0x02
#define		HRXOR		0x03
#define		HRINV		0x04
#define		HRNAND		0x05
#define		HRNOR		0x06
#define		HRXNOR		0x07
// inhibits auto increment when set
#define		HRC_INHINC	0x80


#define UR_HRBORDER	0xAA		// border color for hires
#define UR_HRBDAT	0xAB		// read/write buffer memory.

