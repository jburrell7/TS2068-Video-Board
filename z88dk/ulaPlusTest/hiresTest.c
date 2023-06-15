#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "fpga.h"


void writeUlaReg(reg, val);
void setWindow(unsigned char xl, yl, xr, yr);
void fillWind(unsigned char fill);
void setOp(unsigned char op);
void copyWnd(unsigned char dx, dy);

void waitForKey(void);
void makeRedRectangle(void);
void copyRectXor(void);
void copyRect(void);
void setModeHT(void);
void setModeHTRed(void);
void fillSquare(void);

void copyFromBuffer(void);
void copyToBuffer(void);
void WriteToBuffer(void);
void copyGreenFromBuffer(void);
void hrClrScr(unsigned char color);




unsigned char brick[64] = {	0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
 							0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
						    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18,
							0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
 							0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18};

unsigned char brickg[64] = {0x30, 0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30,
 							0x30, 0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30,
						    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30, 0x30,
							0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30, 0x30,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0x30, 0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30,
 							0x30, 0x30, 0x30, 0x30, 0xFF, 0x30, 0x30, 0x30};


void main()
{
// select hires on display B
	writeUlaReg(UR_VMODEB, MODEHR);

	hrClrScr(0x03);

	waitForKey();
	makeRedRectangle();
	waitForKey();
	copyRectXor();
	waitForKey();
	copyRect();
	waitForKey();
	setModeHT();
	waitForKey();
	setModeHTRed();
	waitForKey();
	fillSquare();
	waitForKey();
	copyToBuffer();
	waitForKey();
	copyFromBuffer();
	waitForKey();
	WriteToBuffer();
	waitForKey();
	copyGreenFromBuffer();


}

void waitForKey(void){
int c;

	c = getchar();

}


void makeRedRectangle(void){
	printf("Make red rectangle\n");
	setWindow(48, 48, 48 + 120, 48 + 40);
	fillWind(0x1E);
}

void copyRectXor(void){
	printf("Copy rectangle XOR\n");
	writeUlaReg(UR_HROP, HRXOR);
	copyWnd(99, 68);
}

void copyRect(void){
	printf("Copy rectangle\n");
	writeUlaReg(UR_HROP, HRNOP);
	copyWnd(12, 8);
}

void setModeHT(void){
	printf("Mode HT\n");
	writeUlaReg(UR_TRANSCLR, 0x00);
	writeUlaReg(UR_VMODEB, MODEHT);
}


void setModeHTRed(void){
	printf("Mode HT Red\n");
	writeUlaReg(UR_TRANSCLR, 0x1E);
}


void fillSquare(void){
unsigned char x, y, i;

	printf("Fill a square bricks\n");
	setModeHT();

	y = 20 * 8;
	x = 6 * 8;
	setWindow(x, y, x + 7, y + 7);
	outp(ULA_ADR, UR_HRDAT);
	for (i = 0; i < 64; i++){
		outp(ULA_DAT, brick[i]);
	}
}


void copyToBuffer(void){
unsigned char x, y, i;

	printf("Copy bricks to buffer\n");

	y = 20 * 8;
	x = 6 * 8;
	setWindow(x, y, x + 7, y + 7);

	writeUlaReg(UR_HRDESTX, 0);
	writeUlaReg(UR_HRDESTY, 0);

	writeUlaReg(UR_HRCMD, HRC_STOB);

}


void copyFromBuffer(void){
	printf("Copy bricks from buffer\n");

	setWindow(0, 0, 7, 7);

	writeUlaReg(UR_HRDESTX, 0);
	writeUlaReg(UR_HRDESTY, 0);

	writeUlaReg(UR_HRCMD, HRC_BTOS);
}


void WriteToBuffer(void){
unsigned char i;

	printf("Write green bricks to buffer\n");

	setWindow(8, 8, 15, 15);
	outp(ULA_ADR, UR_HRBDAT);

	for (i = 0; i < 64; i++){
		outp(ULA_DAT, brickg[i]);
	}

}


void copyGreenFromBuffer(void){
	printf("Copy green bricks from buffer\n");

	setWindow(8, 8, 15, 15);

	writeUlaReg(UR_HRDESTX, 8);
	writeUlaReg(UR_HRDESTY, 0);

	writeUlaReg(UR_HRCMD, HRC_BTOS);
}




///////////////////////////////////////////////////

void setWindow(unsigned char xl, yl, xr, yr){
	writeUlaReg(UR_HRULX, xl);
	writeUlaReg(UR_HRULY, yl);
	writeUlaReg(UR_HRLRX, xr);
	writeUlaReg(UR_HRLRY, yr);
}

void fillWind(unsigned char fill){
	writeUlaReg(UR_HRFILL, fill);
	writeUlaReg(UR_HRCMD, HRC_FILL);
}

void setOp(unsigned char op){
	writeUlaReg(UR_HROP, op);
}


void copyWnd(unsigned char dx, dy){
	writeUlaReg(UR_HRDESTX, dx);
	writeUlaReg(UR_HRDESTY, dy);
	writeUlaReg(UR_HRCMD, HRC_COPY);

}


void writeUlaReg(reg, val){
	outp(ULA_ADR, reg);
	outp(ULA_DAT, val);
}

void hrClrScr(unsigned char color){
	printf("clearing screen\n");
	setOp(HRNOP);
	setWindow(0, 0, 255, 191);
	fillWind(color);
}

void copyToBuf(unsigned char dx, dy){


}

