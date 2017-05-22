#include "iodefine.h"

/*
 * マルチファンクションタイマパルスユニットでのPWM出力
 */
void InitPWM(void){
	PORT1.PDR.BIT.B7 = 1;
	PORT1.PMR.BIT.B7 = 1;
	
	/*
	 * MTIOC3A
	 */
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P17PFS.BIT.PSEL = 0x1;
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	ICU.IPR[152].BIT.IPR = 10;
	ICU.IR[152].BIT.IR = 0;
	ICU.IER[0x13].BIT.IEN0 = 1;// MTU3 TGIA3
	ICU.IER[0x13].BIT.IEN1 = 1;// MTU3 TGIB3
	
	//TGRA カウンタクロックの選択
	MTU3.TCR.BYTE = 0x28;//TGRAのコンペアマッチ・インプットキャプチャでTCNTクリア + 立ち下がりエッジでカウント
	MTU3.TMDR.BYTE = 0x02;//PWMモード１
	MTU3.TIORH.BYTE = 0x37;//初期LOWコンペアマッチでトグル(TRGBの設定)　＋　初期Highコンペマッチでトグル（TRGAの設定)
	MTU3.TIER.BYTE = 0x01;//TGRA割り込みを許可
	
	MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
	MTU3.TGRA = 48000;
	MTU3.TGRB = 24000;
	MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
	
	MTU.TSTR.BIT.CST3 = 1;//TCNTカウンタの開始
}

