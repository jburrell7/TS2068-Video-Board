#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "fpga.h"

#define TEXT_PORT 0xA2

void waitText(void);
void sendTextChar(unsigned char c);
void clrScr(void);
void sendCmd(char* str, unsigned char strLen);
void sendStr(char* str, unsigned char strLen);
void myOut8(unsigned char port, unsigned char dat);
void setPos(unsigned char x, unsigned char y);
void writeUlaReg(unsigned char reg, val);

char bgdColor[] = "[33;44m";
char wndColor[] = "[33;41m";
char textColor[] = "[30;46m";
char msg[] = "Hello, world!\n";

void main()
{
unsigned char i, j, x, y;

	writeUlaReg(UR_VMODEB, MODETXT);

//	printf("1.1\n");

// clear the screen
	sendCmd(bgdColor, 7);
	sendTextChar(12);
//

// set the screen location to 20, 15
	y = 8;
	setPos(20, y);

// set the color for the window
	sendCmd(wndColor, 7);
// write the top border
	sendTextChar(0xC9);
	for (i = 0; i < 30; i++){
		sendTextChar(0xCD);
	}
	sendTextChar(0xBB);


	for (i = 0; i < 10; i++){
		y++;
		setPos(20, y);
	// set the color for the window
		sendCmd(wndColor, 7);
		sendTextChar(0xBA);

		sendCmd(textColor, 7);
		for (j = 0; j < 30; j++) sendTextChar(' ');

		sendCmd(wndColor, 7);
		sendTextChar(0xBA);
	}

//		Write the bottom border
	y++;
	setPos(20, y);
// set the color for the window
	sendCmd(wndColor, 7);
// write the top border
	sendTextChar(0xC8);
	for (i = 0; i < 30; i++){
		sendTextChar(0xCD);
	}
	sendTextChar(0xBC);


	setPos(21, 9);
	sendCmd(textColor, 7);
	sendStr(msg, strlen(msg));


}


void clrScr(void){
	sendTextChar(12);
}


void setPos(unsigned char x, unsigned char y){
char outBuf[80];
char sTemp[10];

	outBuf[0] = '[';
	outBuf[1] = '\0';
	sprintf(sTemp, "%d", y);
	strcat(outBuf, sTemp);
	strcat(outBuf, ";");
	sprintf(sTemp, "%d", x);
	strcat(outBuf, sTemp);
	strcat(outBuf, "H");

//	printf("%s   %s    %d\n", outBuf, sTemp, x);

	sendCmd(outBuf, strlen(outBuf));

}



void sendCmd(char* str, unsigned char strLen){
	sendTextChar(27);
	sendStr(str, strLen);
}

void sendStr(char* str, unsigned char strLen){
unsigned char i;

	for (i = 0; i < strLen; i++){
		sendTextChar(*str++);
	}
}

void sendTextChar(unsigned char x){
unsigned char a = 1;


	while(a != 0){
		a = inp(TEXT_PORT);
		a = a & 0x40;
	}

	outp(TEXT_PORT, x);
}

void waitText(void){
unsigned char a = 1;

	while(a != 0){
		a = inp(0xA2);
		a = a & 0x40;
	}
}

void writeUlaReg(unsigned char reg, val){
	outp(ULA_ADR, reg);
	outp(ULA_DAT, val);
}

