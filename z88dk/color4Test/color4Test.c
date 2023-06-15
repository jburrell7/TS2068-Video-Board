#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "fpga.h"

void writeUlaPaletteA(void);
void writeUlaReg(unsigned char reg, val);
void clearScreen(unsigned int clrColor) __z88dk_fastcall;
void setColor4Mode(void);
void setColor16Mode(void);
void writeTestBlks(void);
void setPixel(unsigned int x, y, unsigned char color);
void writeOneTestBlock(unsigned int x, y);



// ULA Plus palette color definitions
unsigned char ulaPA[16] = {	0x00, 0x03, 0x1c, 0x1f,
							0xe0, 0xe3, 0xfc, 0xff,
							0x49, 0x01, 0x0c, 0x0e,
							0x60, 0x61, 0x76, 0x6d};

// definition for a 2X8 test block in packed format
unsigned char testCB[8] = { 0x1B, 0x00, 0x55, 0xAA,
							0xFF, 0x05, 0x5F, 0xB1};

int main()
{
// write the palett values to the ULA Plus registers
	writeUlaPaletteA();
// set the border color for our mode
	writeUlaReg(UR_EXPBORD, 0xfc);
// set the 16 color mode
	setColor4Mode();
// clear the screen
	clearScreen(0x03);

	writeTestBlks();

// loop here since the video mode is incompatible
//	with the Timex display and we won't be able to
//	see anything if dropped back to the command line
	while(1);


}


void writeUlaPaletteA(void){
unsigned char i;

	for (i = ULA_PALA0; i < ULA_PALB0; i++){
		writeUlaReg(i, ulaPA[i]);
	}
}


void clearScreen(unsigned int clrColor) __z88dk_fastcall
{

	#asm
	; this section makes the color value a proper
	;	8 bit value to write to memory to clear the screen
		ld		a, l	; get the color value
		and	    $03		; keep only the 4 lower bits, clear carry
		ld		b, a	; save the color value
		rla				; shift right 4 bits
		rla
		or		b
		ld		b, a
		rla
		rla
		or		b		; the color value is now ready in A
		ld		b, a
		rla
		rla
		or		b		; the color value is now ready in A

	; we clear the screen by writing the color value to
	;	each byte of DF0...
		ld		hl, $4000	; DF0 address
		ld		de, $4001
		ld		bc, 6143
		ld		(hl), a
		ldir
	; ... and repeating for DF1
		ld		hl, $6000
		ld		de, $6001
		ld		bc, 6143
		ld		(hl), a
		ldir

	#endasm
}

void writeTestBlks(void){
unsigned int i, j;
unsigned char *adr;

// write a test block to the upper left corner of DF0
	writeOneTestBlock(0, 0);
// write a test block to the upper right corner of DF0
	writeOneTestBlock(252, 0);
// write a test block to the lower left corner of DF0
	writeOneTestBlock(0, 88);
// write a test block to the lower right corner of DF0
	writeOneTestBlock(252, 88);

// write a test block to the upper left corner of DF1
	writeOneTestBlock(0, 96);
// write a test block to the upper right corner of DF1
	writeOneTestBlock(252, 96);
// write a test block to the lower left corner of DF1
	writeOneTestBlock(0, 184);
// write a test block to the lower right corner of DF1
	writeOneTestBlock(252, 184);
// write a test block that spans DF1 and DF2.
//	we write near the left side so we can use the
//	existing blocks as rulers
	writeOneTestBlock(6, 94);

}


//-----------------------------------------------------
// Write a test block at the pixel coordinates x, y
//	x and y specify the upper left corner of the
//	test block
//
void writeOneTestBlock(unsigned int x, y){
unsigned char i;
unsigned char pixel;

// loop for the 8 vertical color bars
	for (i = 0; i < 8; i++){
	// get the even pixel color...
		pixel = testCB[i] >> 6;
	// ... and write it
		setPixel(x, y, pixel);
	// advance to the next column
		x += 1;

	// get the even pixel color...
		pixel = testCB[i] >> 4;
	// ... and write it
		setPixel(x, y, pixel);
	// advance to the next column
		x += 1;

	// get the even pixel color...
		pixel = testCB[i] >> 2;
	// ... and write it
		setPixel(x, y, pixel);
	// advance to the next column
		x += 1;

	// get the odd pixel color...
		pixel = testCB[i] & 0x03;
	// ... and set it
		setPixel(x, y, pixel);
	// back to the first column
		x -= 3;
	// next line
		y += 1;
	}
}

//-----------------------------------------------------
// Set a pixel at x, y with "color"
//	this routine silently returns if the pixel location
//	is invalid
//
void setPixel(unsigned int x, y, unsigned char color){
unsigned char *adr;
unsigned int baseAdr;
unsigned char pixel;

// ensure our pixel stays within the
//	screen bounds
	if (y > 191) return;
	if (x > 255) return ;

// clip the color value
	color &= 0x03;

// if the pixel is even, shift it left by 4 bits
	switch (x & 3){
	case 0:
		color = color << 6;
		break;
	case 1:
		color = color << 4;
		break;
	case 2:
		color = color << 2;
		break;
	}


// set the base address of the block based on
//	the y coordinate
	baseAdr = 16384;
	if (y > 95){
	// in DF1
		baseAdr += 8192;
	// make compatible with the succeeding computations
		y -= 96;
	}

// compute the address of the pixel
//	there are 64 bytes per scan line, the left shift
//		by 6 does this
	adr = (unsigned char *)((y << 6) + (x >> 2) + baseAdr);
// get the current
	pixel = *adr;

//

	switch (x & 3){
	case 0:
		pixel &= 0x3F;
		break;
	case 1:
		pixel &= 0xCF;
		break;
	case 2:
		pixel &= 0xF3;
		break;
	case 3:
		pixel &= 0xFC;
		break;
	}

// put the new pixel color in place
	pixel |= color;
// and write to video memory
	*adr = pixel;

}


void setColor4Mode(void){
// set video A to TS vid
	writeUlaReg(UR_VMODEA, MODETS);
// enable advanced video and set to color4
	writeUlaReg(UR_EXMODE, (MSK_ENAEXT | MSK_XINKBLUE | MSK_XCOLOR4));
}



// write a value to a ULA register
void writeUlaReg(unsigned char reg, val){
	outp(ULA_ADR, reg);
	outp(ULA_DAT, val);
}

