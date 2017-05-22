#include "iodefine.h"

int initRIIC0(void)
{
	//I2Cモジュールストップ状態の解除
	SYSTEM.PRCR.WORD = 0xA502;//モジュールストップレジスタのプロテクト解除
	SYSTEM.MSTPCRB.BIT.MSTPB21 = 0;//I2Cモジュールストップ状態の解除
	SYSTEM.PRCR.WORD = 0xA500;//モジュールストップレジスタのプロテクト

	// set port direction to input
	PORT1.PDR.BIT.B3 = 0;// SDA0
	PORT1.PDR.BIT.B2 = 0;// SCL0
	// set port mode as peripheral device
	PORT1.PMR.BIT.B3 = 1;// SDA0
	PORT1.PMR.BIT.B2 = 1;// SCL0
	
	// select pin function as RIIC
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P13PFS.BIT.PSEL = 0x0F;// SDA0
	MPC.P12PFS.BIT.PSEL = 0x0F;// SCL0
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	// オープンドレイン設定　外部プルアップ抵抗で出力決めるなら必要
	PORT1.ODR0.BIT.B6 = 1;
	PORT1.ODR0.BIT.B4 = 1;
	//入力プルアップ有効
	//PORT1.PCR.BIT.B3 = 1;
	//PORT1.PCR.BIT.B2 = 1;


	/* RIIC disable for RIIC initial */
	RIIC0.ICCR1.BIT.ICE = 0;
	RIIC0.ICCR1.BIT.IICRST = 1;// リセットパルス発生
	RIIC0.ICCR1.BIT.ICE = 1;
	    
	/* Set baud rate */
	RIIC0.ICMR1.BIT.CKS = 3;// IIC phi = PCLK/8 clock
	RIIC0.ICBRH.BIT.BRH = 26;// 100kbps (PCLK = 48MHz)
	RIIC0.ICBRL.BIT.BRL = 26;
	/* disable all address detection. Master mode only. */
	RIIC0.ICSER.BYTE = 0x00;
	/* ACKWP is protect bit for ACKBT */
	RIIC0.ICMR3.BIT.ACKWP = 1;// disable protect for ACKBT
 
	RIIC0.ICCR1.BIT.IICRST = 0;//リセット解除
	
	return 0;
}

int i2cWrite(unsigned char slv_adr7b, unsigned char sub_adr, unsigned char data)
{
	int i;
	unsigned char addres8b;
	
	addres8b = slv_adr7b<<1 + 0;

	while(RIIC0.ICCR2.BIT.BBSY != 0);//使用中
	RIIC0.ICCR2.BIT.ST = 1;
	while(RIIC0.ICSR2.BIT.NACKF == 0)
	{
		while(RIIC0.ICSR2.BIT.TDRE == 0);
		RIIC0.ICDRT = addres8b;
		
		for(i=0; i<1000; i++);
		if(RIIC0.ICSR2.BIT.NACKF != 0)
		{
			break;
		}
		while(RIIC0.ICSR2.BIT.TDRE == 0);
		RIIC0.ICDRT = sub_adr;	
		
		for(i=0; i<1000; i++);
		if(RIIC0.ICSR2.BIT.NACKF != 0)
		{
			break;
		}
		while(RIIC0.ICSR2.BIT.TDRE == 0);
		RIIC0.ICDRT = data;
		
		while(RIIC0.ICSR2.BIT.TEND == 0);
		break;
	}
	
	RIIC0.ICSR2.BIT.STOP = 0;
	RIIC0.ICCR2.BIT.SP = 1;
	
	while(RIIC0.ICSR2.BIT.STOP == 0);
	
	RIIC0.ICSR2.BIT.NACKF=0;
	RIIC0.ICSR2.BIT.STOP = 0;
	
	return 0;
}