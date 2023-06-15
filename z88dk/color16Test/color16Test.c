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
unsigned char testCB[8] = { 0x01, 0x23, 0x45, 0x67,
							0x89, 0xab, 0xcd, 0xef};

int main()
{
// write the palett values to the ULA Plus registers
	writeUlaPaletteA();
// set the border color for our mode
	writeUlaReg(UR_EXPBORD, 0xfc);
// set the 16 color mode
	setColor16Mode();
// clear the screen
	clearScreen(0x03);

	writeTestBlks();

// loop here since the video mode is incompatible
//	with the Timex display and we won't be able to
//	see anything if dropped back to the command line
	while(1);


}

// fill ULA palette #0 with 16 color values
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
		and	    $0F		; keep only the 4 lower bits, clear carry
		ld		b, a	; save the color value
		rla				; shift right 4 bits
		rla
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
	writeOneTestBlock(126, 0);
// write a test block to the lower left corner of DF0
	writeOneTestBlock(0, 88);
// write a test block to the lower right corner of DF0
	writeOneTestBlock(126, 88);

// write a test block to the upper left corner of DF1
	writeOneTestBlock(0, 96);
// write a test block to the upper right corner of DF1
	writeOneTestBlock(126, 96);
// write a test block to the lower left corner of DF1
	writeOneTestBlock(0, 184);
// write a test block to the lower right corner of DF1
	writeOneTestBlock(126, 184);
// write a test block that spans DF1 and DF2.
//	we write near the left side so we can use the
//	existing blocks as rulers
	writeOneTestBlock(3, 94);

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
		pixel = testCB[i] >> 4;
	// ... and write it
		setPixel(x, y, pixel);
	// advance to the next column
		x += 1;
	// get the odd pixel color...
		pixel = testCB[i] & 0x0F;
	// ... and set it
		setPixel(x, y, pixel);
	// back to the first column
		x -= 1;
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
	if (x > 127) return ;

// clip the color value
	color &= 0x0F;

// if the pixel is even, shift it left by 4 bits
	if ((x & 1) == 0) color = color << 4;

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
	adr = (unsigned char *)((y << 6) + (x >> 1) + baseAdr);
// get the current
	pixel = *adr;

//
	if (x & 1){
	// we are setting the odd pixel, so clear those bits
		pixel &= 0xF0;
	} else {
	// we are setting the even pixel, so clear those bits
		pixel &= 0x0F;
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


void setColor16Mode(void){
// set video A to TS vid
	writeUlaReg(UR_VMODEA, MODETS);
// enable advanced video and set to color16
	writeUlaReg(UR_EXMODE, (MSK_ENAEXT | MSK_XINKBLUE | MSK_XCOLOR16));
}


// write a value to a ULA register
void writeUlaReg(unsigned char reg, val){
	outp(ULA_ADR, reg);
	outp(ULA_DAT, val);
}

