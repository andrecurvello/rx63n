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
	MTU3.TGRA = 48000;//コンペアマッチでトグル
	MTU3.TGRB = 24000;//コンペアマッチでトグル
	MTU.TRWER.BIT.RWE = 0x0;//プロテクト設定
	
	MTU.TSTR.BIT.CST3 = 1;//TCNTカウンタの開始
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

（ここまで）*/