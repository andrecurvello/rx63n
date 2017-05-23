#include "iodefine.h"
extern void HardwareSetup(void);

void HardwareSetup(void)
{
	int i;
	
	//プロテクト解除
	SYSTEM.PRCR.WORD = 0xA503;
	//クロック用端子設定
	PORT3.PDR.BYTE = 0x00;
	PORT3.PMR.BYTE = 0xC0;
	//発振器ウェイトコントロール
	SYSTEM.MOSCWTCR.BYTE = 0x0E;
	SYSTEM.SOSCWTCR.BYTE = 0x0E;
	
	//禁止状態確認
	while(SYSTEM.OPCCR.BIT.OPCMTSF == 1);
	while(FLASH.FENTRYR.BIT.FENTRY0 == 1);
	while(FLASH.FENTRYR.BIT.FENTRY1 == 1);
	while(FLASH.FENTRYR.BIT.FENTRY2 == 1);
	while(FLASH.FENTRYR.BIT.FENTRY3 == 1);
	
	SYSTEM.MOSCCR.BYTE = 0x00;
	while(SYSTEM.MOSCCR.BYTE == 0x01);
	
	SYSTEM.PLLCR.WORD = 0x0F00;
	SYSTEM.PLLCR2.BYTE = 0x00;
	
	SYSTEM.PLLWTCR.BYTE = 0x0F;
	
	for(i=0; i<600; i++);
	
	SYSTEM.SCKCR.LONG = 0x21021211;
	SYSTEM.SCKCR2.WORD = 0x0032;
	SYSTEM.BCKCR.BYTE = 0x01;
	
	SYSTEM.SCKCR3.WORD = 0x0400;
}
