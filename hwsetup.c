/************************************************************************
*
* Device     : RX/RX600/RX63N,RX631
*
* File Name  : hwsetup.c
*
* Abstract   : Hardware Setup file.
*
* History    : 0.10  (2011-02-21)  [Hardware Manual Revision : 0.01]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2011 Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
************************************************************************/

#include "iodefine.h"
#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

void HardwareSetup(void)
{
	int i;
	
	//�v���e�N�g����
	SYSTEM.PRCR.WORD = 0xA503;
	//�N���b�N�p�[�q�ݒ�
	PORT3.PDR.BYTE = 0x00;
	PORT3.PMR.BYTE = 0xC0;
	//���U��E�F�C�g�R���g���[��
	SYSTEM.MOSCWTCR.BYTE = 0x0E;
	SYSTEM.SOSCWTCR.BYTE = 0x0E;
	
	//�֎~��Ԋm�F
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
