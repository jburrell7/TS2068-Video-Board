
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>


#include "fpga.h"
#include "patterns.h"


void vdp_writeReg(unsigned char reg, val);
unsigned char vdp_getStat(void);
void vdp_writeMemBlk(unsigned char *data, unsigned int len, unsigned int adr);
void vdp_getMemBlk(unsigned int adr, unsigned int len, unsigned char *data);
void vdp_setTextMode(void);
void writeUlaReg(unsigned char reg, val);


unsigned char textModeInit[8] = {0x00, 0xD0, 0x02, 0x00,
								0x00, 0x20, 0x00, 0xF5};

unsigned char testMsg[80];


main(){
// set display B for VDP output
	writeUlaReg(UR_VMODEB, MODEVDP);
// set the 9918 core for text mode
	vdp_setTextMode();
// write the character bit maps starting at
//	vdp memory address 0x0000
	vdp_writeMemBlk(ASCII, sizeof(ASCII), 0);

	strcpy(testMsg, "Hello, world!");
	vdp_writeMemBlk(testMsg, strlen(testMsg), 0x801);
}


void vdp_setTextMode(void){
unsigned char i;

	for(i = i; i < 8; i++){
		vdp_writeReg(i, textModeInit[i]);
	}
}

void vdp_writeReg(unsigned char reg, val){

// defensive programming
	reg = (reg & 7) | 0x80;
	outp(VDP_REG_ADR, reg);
	outp(VDP_REG_ADR, val);
}

unsigned char vdp_getStat(void){
	return inp(VDP_REG_ADR);
}

void vdp_writeMemBlk(unsigned char *data, unsigned int len, unsigned int adr){
unsigned char tmp;
unsigned int i;

	printf("writing %u bytes ", len);
	printf("from %u to %u\n", data, adr);

	adr = (adr & 0x3FFF) |0x4000;
	tmp = adr & 0xFF;
	vdp_writeReg(VDP_REG_ADR, tmp);
	tmp = adr >> 8;
	vdp_writeReg(VDP_REG_ADR, tmp);

	for (i = 0; i < len; i++){
		outp(VDP_MEM_ADR, *data);
		data++;
	}
}


void vdp_getMemBlk(unsigned int adr, unsigned int len, unsigned char *data){
unsigned char tmp;
unsigned int i;

	adr = (adr & 0x3FFF) |0x4000;
	tmp = adr & 0xFF;
	vdp_writeReg(VDP_REG_ADR, tmp);
	tmp = adr >> 8;
	vdp_writeReg(VDP_REG_ADR, tmp);

	for (i = 0; i < len; i++){
		*data = inp(VDP_MEM_ADR);
		data++;
	}
}


void writeUlaReg(unsigned char reg, val){
	outp(ULA_ADR, reg);
	outp(ULA_DAT, val);
}

