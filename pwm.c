#include "iodefine.h"

/*
 * マルチファンクションタイマパルスユニットでのPWM出力
 */
void initPWM(void){
	//MTIOC3A用ポート設定
	PORT1.PDR.BIT.B7 = 1;
	PORT1.PMR.BIT.B7 = 1;
	//MTIOC1A用ポート設定
	PORT2.PDR.BIT.B0 = 1;
	PORT2.PMR.BIT.B0 = 1;
	
	/*
	 * MTIOC3A(P17)
	 * MTIOC1A(P20)
	 */
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P17PFS.BIT.PSEL = 0x1;
	MPC.P20PFS.BIT.PSEL = 0x1;
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	SYSTEM.PRCR.WORD = 0xA503;
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	//MTU1
	ICU.IPR[148].BIT.IPR = 10;
	ICU.IR[148].BIT.IR = 0;
	ICU.IER[0x12].BIT.IEN4 = 1;
	ICU.IER[0x12].BIT.IEN5 = 1;
	
	//MTU3
	ICU.IPR[152].BIT.IPR = 10;
	ICU.IR[152].BIT.IR = 0;
	ICU.IER[0x13].BIT.IEN0 = 1;// MTU3 TGIA3
	ICU.IER[0x13].BIT.IEN1 = 1;// MTU3 TGIB3
	
	//TGRA カウンタクロックの選択
	//MTIOC1A
	MTU1.TCR.BYTE = 0x48;//TGRBのコンペアマッチでTCNT=0, 立下りエッジでカウント
	MTU1.TMDR.BYTE = 0x02;//PWMモード１
	MTU1.TIOR.BYTE = 0x12;//A:初期low - コンペアマッチでhigh, B:初期low - コンペアマッチでlow
	
	MTU3.TCR.BYTE = 0x48;//TGRBのコンペアマッチ・インプットキャプチャでTCNTクリア + 立ち下がりエッジでカウント
	MTU3.TMDR.BYTE = 0x02;//PWMモード１
	MTU3.TIORH.BYTE = 0x12;//A:low-high, B:low-low
	MTU3.TIER.BYTE = 0x01;//TGRA割り込みを許可
	
	MTU.TRWER.BIT.RWE = 0x1;//プロテクト解除
	MTU1.TGRA = 1000;//コンペアマッチでHIGH(duty)
	MTU1.TGRB = 10000;//コンペアマッチでLOW(cycle)
	MTU3.TGRA = 1000;//コンペアマッチでHIGH(duty)
	MTU3.TGRB = 10000;//コンペアマッチでLOW(cycle)
	MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
	
	MTU.TSTR.BIT.CST1 = 1;//MTU0-TCNTカウンタ開始
	MTU.TSTR.BIT.CST3 = 1;//MTU3-TCNTカウンタの開始
}

/* メモ（ここから）

PWMモード1（最大8相のPWM出力）
TGRAとTGRB、TGRCとTGRDをペアで使用。それぞれ、MTIOCnA、MTIOCnC端子からPWM出力を生成する。
●　TGRA,Cのコンペアマッチ -> TIORのIOA、IOCビットで指定した出力
●　TGRB,Dのコンペアマッチ -> TIORのIOB、IODビットで指定した出力

MTIOC0A - 使用レジスタ - MTU0.TGRA + MTU0.TRGB - 
MTIOC0C - 使用レジスタ - MTU0.TGRC + MTU0.TGRD - P32
MTIOC1A - 使用レジスタ - MTU1.TGRA + MTU1.TRGB - P20
MTIOC2A - 使用レジスタ - MTU2.TGRA + MTU2.TGRB - P26
MTIOC3A - 使用レジスタ - MTU3.TGRA + MTU3.TRGB - P14*P17
MTIOC3C - 使用レジスタ - MTU3.TGRC + MTU3.TGRD - P16
MTIOC4A - 使用レジスタ - MTU4.TGRA + MTU4.TRGB - 
MTIOC4C - 使用レジスタ - MTU4.TGRC + MTU4.TGRD - 

PWMモード２（同期動作と併用することにより最大8相のPWM出力）
●同期動作は、複数のTCNTの値を同時に書き換えることができる。
●TCRの設定により複数のTCNTを同時にクリアすることができる（同期クリア）
●TGRの1本を周期レジスタ、他のTGRをデューティレジスタに使用。

MTIOC0A - 使用レジスタ - MTU0.TGRA
MTIOC0B - 使用レジスタ - MTU0.TGRB
MTIOC0C - 使用レジスタ - MTU0.TGRC
MTIOC0D - 使用レジスタ - MTU0.TGRD
MTIOC1A - 使用レジスタ - MTU1.TGRA
MTIOC1B - 使用レジスタ - MTU1.TGRB
MTIOC2A - 使用レジスタ - MTU2.TGRA
MTIOC2B - 使用レジスタ - MTU2.TGRB

●TCNT（タイマカウンタ）
16ビットの読み書き可能なカウンタ。リセットで0000hに初期化

●トグル出力
0・1の入れ替え

（ここまで） */