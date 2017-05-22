#include "vect.h"
#include "iodefine.h"

/*
	「メモ」
	96MHz / 512 = 187500 cycle/s
	96MHz/8/440/256　ラの音
	CMT0.CMCOR = 106;//5860, 46876 - 1;//23438-1;//10000ms //2929-1(1000ms)
	「使用目的」
	CMT0
	CMT1
	CMT2
	CMT3 時間計測
*/

unsigned char cmt_counter;

void initCMT0(void);
void initCMT1(void);
void initCMT2(void);
void initCMT3(void);
void initCMT(void);
void timer10m(unsigned char);

void initCMT(void){
	initCMT0();
	initCMT1();
	initCMT2();
	initCMT3();
}

void initCMT0(void){
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;//CMT0,1
	SYSTEM.PRCR.WORD = 0xA500;
	
	CMT.CMSTR0.BIT.STR0 = 0;
	
	CMT0.CMCR.BIT.CMIE = 0x01;
	CMT0.CMCR.BIT.CKS = 0x01;//8分周・32分周・128分周・512分周
	
	CMT0.CMCNT = 0;
	
	//192MHz/2/512 = 187500 cycles
	CMT0.CMCOR = 18750;// 100ms
	
	ICU.IPR[4].BYTE = 0x06;
	ICU.IER[3].BIT.IEN4 = 1;
}

extern unsigned char sinNow;
void Excep_CMT0_CMI0(void){ 
	if(sinNow < 256) sinNow++;
	else sinNow = 0;
}

void initCMT1(void){
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;//CMT0,1
	SYSTEM.PRCR.WORD = 0xA500;
	
	CMT.CMSTR0.BIT.STR1 = 0;
	
	CMT1.CMCR.BIT.CMIE = 0x01;
	CMT1.CMCR.BIT.CKS = 0x03;//8分周・32分周・128分周・512分周
	
	CMT1.CMCNT = 0;
	
	//192MHz/2/512 = 187500 cycles
	CMT1.CMCOR = 18750;// 100ms
	
	ICU.IPR[5].BYTE = 0x06;
	ICU.IER[3].BIT.IEN5 = 1;
}

void Excep_CMT1_CMI1(void){ 
	
}

void initCMT2(void){
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA14 = 0;//CMT0,1
	SYSTEM.PRCR.WORD = 0xA500;
	
	CMT.CMSTR1.BIT.STR2 = 0;
	
	CMT2.CMCR.BIT.CMIE = 0x01;
	CMT2.CMCR.BIT.CKS = 0x00;//8分周・32分周・128分周・512分周
	
	CMT2.CMCNT = 0;
	
	//192MHz/2/8 = 12000000 cycles/s
	//1cycle 83.333ns
	//1μs -> 12
	CMT2.CMCOR = 18750;// 100ms
	
	ICU.IPR[6].BYTE = 0x06;
	ICU.IER[3].BIT.IEN6 = 1;
}

void Excep_CMT2_CMI2(void){ 
	
}

void initCMT3(void){
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA14 = 0;//CMT0,1
	SYSTEM.PRCR.WORD = 0xA500;
	
	CMT.CMSTR1.BIT.STR3 = 0;
	
	CMT3.CMCR.BIT.CMIE = 0x01;
	CMT3.CMCR.BIT.CKS = 0x03;//8分周・32分周・128分周・512分周
	
	CMT3.CMCNT = 0;
	
	//192MHz/2/512 = 187500 cycles
	CMT3.CMCOR = 1875;// 10ms
	
	ICU.IPR[7].BYTE = 0x06;
	ICU.IER[3].BIT.IEN7 = 1;
}

void Excep_CMT3_CMI3(void){ 
	cmt_counter++;
}

void timer10m(unsigned char time){
	CMT3.CMCNT = 0;
	cmt_counter = 0;
	CMT.CMSTR1.BIT.STR3 = 1;
	while(cmt_counter < time);
	CMT3.CMCNT = 0;
	cmt_counter = 0;
	CMT.CMSTR1.BIT.STR3 = 0;
}