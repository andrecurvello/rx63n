/*
 * 概要
 * LED点滅 PORTA
 * ソフトウェアタイマで点滅
 */

#include "iodefine.h"
#define WAIT_TIME 1000

void InitIO(void);
void InitLedOut(void);
void RunLedOut(void);

unsigned char led_data = 0xFF;

void InitIO(){
	PORTA.PDR.BYTE = 0xFF;//PORTA出力
	PORTA.PODR.BYTE = 0xFF;//負論理で消灯
}

void RunLedOut(){
	unsigned short i;
	while(1){
		led_data = ~led_data;
		PORTA.PODR.BYTE = led_data;
		for(i=0; i<WAIT_TIME; i++);
	}
}
