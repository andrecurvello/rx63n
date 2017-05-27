/*
 * 概要
 * モーター制御
 * 電流方向による回転方向の制御とPWMによる回転数の制御
 * 使用ポートP44・P45・P46・P47（回転方向）
 * 同上    PB1・PB3（回転数）
 * 動作未確認
 */

#include "iodefine.h"

#ifndef FREQUENCY
#define FREQUENCY 3000000//TGR(short型),PWM:64分周:46875count(1秒間)
#define MAXTCNT 46875
#endif

#ifndef MOTORSPEEDMAX
#define MOTORSPEEDMAX 10
#endif

void initMotorController(void);
void runMotorController(void);
void mcSetSpeed(unsigned short);
void mcSpeedUp(void);
void mcSpeedDown(void);
void mcGoStraight(void);
void mcGoToBack(void);
void mcTurnRight(void);
void mcTrunLeft(void);

//global value
unsigned char motorSpeed;//モーターの回転速度の段階数

void initMotorController(void){
	/* 使用ポート初期化 */
	//1:VCC(設定なし）
	//2:モーター１用出力A1
	//使用ポート	P44(CN2-53)
	PORT4.PDR.BIT.B4 = 1;
	PORT4.PODR.BIT.B4 = 0;
	
	//3:モーター１用パルス出力
	//MTIOC0A(CN1-14)
	PORTB.PDR.BIT.B3 = 1;
	PORTB.PMR.BIT.B3 = 1;
	
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PB3PFS.BIT.PSEL = 1;
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	ICU.IPR[142].BIT.IPR = 10;
	ICU.IR[142].BIT.IR = 0;
	ICU.IER[0x11].BIT.IEN6 = 1;
	ICU.IER[0x11].BIT.IEN7 = 1;
	
	MTU0.TCR.BYTE = 0x4B;//TGRBのコンペアマッチでTCNT=0, 立下りエッジでカウント PCLK/64
	MTU0.TMDR.BYTE = 0x02;//PWMモード１
	MTU0.TIORH.BYTE = 0x12;//A:初期low - コンペアマッチでhigh, B:初期low - コンペアマッチでlow
	
	MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
	//90%
	MTU0.TGRA = 1000;//コンペアマッチでHIGH(duty)
	MTU0.TGRB = 46875;//コンペアマッチでLOW(cycle)
	MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
	
	//MTU.TSTR.BIT.CST0 = 1;//MTU0-TCNTカウンタ開始
	
	//4:モーター１用出力A2
	//使用ポート	P45(CN2-54)
	PORT4.PDR.BIT.B5 = 1;
	PORT4.PODR.BIT.B5 = 0;
	
	//5:モーター２用出力B1
	//使用ポート	P46(CN2-55)
	PORT4.PDR.BIT.B6 = 1;
	PORT4.PODR.BIT.B6 = 0;
	
	//6:モーター２用パルス出力
	//MTIOC0C(CN1-12)：MTU0のため上記と一部設定が同じ
	PORTB.PDR.BIT.B1 = 1;
	PORTB.PMR.BIT.B1 = 1;
	
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PB1PFS.BIT.PSEL = 1;
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	//TGRBをcycleとして利用のため、TCR,TMDRの設定は省略
	//TGRC,TGRDを設定
	MTU0.TIORL.BYTE = 0x12;//C:初期low - コンペアマッチでhigh, D:初期low - コンペアマッチでlow
	
	MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
	//90%
	MTU0.TGRC = 1000;//コンペアマッチでHIGH(duty)
	MTU0.TGRD = 46875;//コンペアマッチでLOW(cycle)
	MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
	
	ICU.IER[0x12].BIT.IEN0 = 1;
	ICU.IER[0x12].BIT.IEN1 = 1;
	
	//MTU.TSTR.BIT.CST0 = 1;//MTU0-TCNTカウンタ開始（TGRA,TGRB,TGRC,TGRDを同時に）
	
	//7:モーター２用出力B2
	//使用ポート	P47(CN2-56)
	PORT4.PDR.BIT.B7 = 1;
	PORT4.PODR.BIT.B7 = 0;
	//8:GND（設定なし）
}

//
void runMotorController(void){
	//MTU.TSTR.BIT.CST0 = 1;//MTU0-TCNTカウンタ開始
	//MTU0.TGRA,MTU0.TGRCを変更で速度制御(最大はTGRBの設定値46875)
}

//速度設定
void mcSetSpeed(unsigned short speed){
	//TGRA, TGRCを設定
	if(0 <= speed && speed <= MOTORSPEEDMAX){
		MTU.TSTR.BIT.CST0 = 0;
		MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
		MTU0.TGRA = MTU0.TGRC = (MOTORSPEEDMAX - motorSpeed) * MAXTCNT + 1;
		MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
		MTU.TSTR.BIT.CST0 = 1;
	}
}

//速度上昇
void mcSpeedUp(void){
	if(motorSpeed < MOTORSPEEDMAX){
		motorSpeed++;
		MTU.TSTR.BIT.CST0 = 0;
		MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
		MTU0.TGRA = MTU0.TGRC = (MOTORSPEEDMAX - motorSpeed) * MAXTCNT + 1;
		MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
		MTU.TSTR.BIT.CST0 = 1;
	}
}

//速度低下
void mcSpeedDown(void){
	if(0 < motorSpeed){
		motorSpeed--;
		MTU.TSTR.BIT.CST0 = 0;
		MTU0.TGRA = MTU0.TGRC = (MOTORSPEEDMAX - motorSpeed) * MAXTCNT + 1;
		MTU.TSTR.BIT.CST0 = 1;
	}
}

//順送
void mcGoStraight(void){
	//
	PORT4.PODR.BIT.B4 = 1;
	PORT4.PODR.BIT.B5 = 0;
	PORT4.PODR.BIT.B6 = 0;
	PORT4.PODR.BIT.B7 = 1;
}

//逆走
void mcGoToBack(void){
	PORT4.PODR.BIT.B4 = 0;
	PORT4.PODR.BIT.B5 = 1;
	PORT4.PODR.BIT.B6 = 1;
	PORT4.PODR.BIT.B7 = 0;
}

//右回転
void mcTurnRight(void){
	PORT4.PODR.BIT.B4 = 1;
	PORT4.PODR.BIT.B5 = 0;
	PORT4.PODR.BIT.B6 = 1;
	PORT4.PODR.BIT.B7 = 0;
}

//左回転
void mcTurnLeft(void){
	PORT4.PODR.BIT.B4 = 0;
	PORT4.PODR.BIT.B5 = 1;
	PORT4.PODR.BIT.B6 = 0;
	PORT4.PODR.BIT.B7 = 1;
}