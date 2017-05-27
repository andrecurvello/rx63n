/*
 * 超音波センサー制御
 * 0.2m - 4.0m
 * 超音波センサモジュールの仕様に準拠
 * 送信から受信までの時間計測CMTタイマ（CMT0使用）
 */
#include "iodefine.h"

#ifndef FREQUENCY
#define FREQUENCY 3000000//TGR(short型),PWM:64分周:46875count(1秒間)
#define MAXTCMCOR 46875
#endif

void initSensorController(void);
void runSensorController(void);
void initCMTforUltrasonicSensor();

void initSensorController(void){
	initCMTforUltrasonicSensor();
}

void runSensorController(void){
	
}

//超音波センサ計測用CMTタイマ設定初期化
//CMT0使用
void initCMTforUltrasonicSensor(void){
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;//CMT0,1
	SYSTEM.PRCR.WORD = 0xA500;
	
	//CMT0の動作停止
	CMT.CMSTR0.BIT.STR0 = 0;
	
	CMT0.CMCR.BIT.CMIE = 0x01;
	CMT0.CMCR.BIT.CKS = 0x01;//8分周・32分周・128分周・512分周
	
	CMT0.CMCNT = 0;
	
	//192MHz/64/64 = 46875-1
	CMT0.CMCOR = 46875 - 1;// 1s
	
	ICU.IPR[4].BYTE = 10;
	ICU.IER[3].BIT.IEN4 = 1;
}

//CMT割込み関数内に配置して使用
void runCMTforUltrasonicSensor(void){
	
}