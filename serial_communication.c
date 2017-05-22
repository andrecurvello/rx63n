#include <machine.h>//setpsw_i()
#include "iodefine.h"
#include "vect.h"

unsigned char SCI0_RX_FLAG = 0;
unsigned char SCI1_RX_FLAG = 0;

//setting for SCI0
void runSCI0(void);
int initSCI0(void);
void enableSCI0(void);
void clearSCI0ErrorFlag(void);
void Excep_SCI0_RXI0(void);

//setting for SCI1
void runSCI1(void);
int initSCI1(void);
void enableSCI1(void);
void clearSCI1ErrorFlag(void);
void Excep_SCI1_RXI1(void);

//setting for SCI6
void runSCI6(void);
int initSCI6(void);
void enableSCI6(void);
void clearSCI6ErrorFlag(void);
void Excep_SCI6_RXI6(void);

/* 割り込み関数 */
void Excep_SCI0_RXI0(void){
	SCI0_RX_FLAG = 1;
}

void Excep_SCI1_RXI1(void){
	SCI1_RX_FLAG = 1;
}

void runSCI0(void){
	int i=0;
	char str;
	unsigned char Start_Message[] = "\n\rSCI Star\n\r";
	
	initIO();
	initSCI();
	setpsw_i();
	
	while(Start_Message[i] != '\0'){
		while(SCI0.SSR.BIT.TEND == 0) ;
		SCI0.TDR = Start_Message[i];
		i++;
	}
	
	while(1){
		if(SCI0_RX_FLAG == 1){
			str = SCI0.RDR;
			PORTA.PODR.BYTE = ~str;
			SCI0_RX_FLAG = 0;
		}
	}
}

/*
 * setting for SCI0
 */

int initSCI0(void){
	SYSTEM.PRCR.WORD = 0xA502;
	SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	SCI0.SCR.BIT.TE = 0;
	SCI0.SCR.BIT.RE = 0;
	SCI0.SCR.BIT.TIE = 0;
	SCI0.SCR.BIT.RIE = 0;
	SCI0.SCR.BIT.TEIE = 0;
	
	//IOポートからモジュールポートへ機能変更
	PORT2.PMR.BIT.B0 = 1;
	PORT2.PMR.BIT.B1 = 1;
	
	//ピン機能選択レジスタ
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.P20PFS.BYTE = 0x0A;
	MPC.P21PFS.BYTE = 0x0A;
	
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	//まで
	
	SCI0.SCR.BIT.CKE = 0;//内蔵ボーレートジェネレータ（SCK0端子）
	
	SCI0.SIMR1.BIT.IICM = 0;//12C Mode Select
	SCI0.SPMR.BIT.CKPH = 0;//
	SCI0.SPMR.BIT.CKPOL = 0;
	
	/* Communication Data Format Setting */
	SCI0.SMR.BIT.CKS = 0x01;//Clock PCLK/4 (n=1)
	SCI0.SMR.BIT.MP = 0;//1対多通信機能を使うなら「１」
	SCI0.SMR.BIT.STOP = 0;//Stop Bit : 1bit
	SCI0.SMR.BIT.PM = 0;//Parity Mode : Even Parity
	SCI0.SMR.BIT.PE = 0;//Parity Enable : No Parity
	SCI0.SMR.BIT.CHR = 0;//Character Length : 8bit
	SCI0.SMR.BIT.CM = 0;//調歩同期式モード
	
	SCI0.SCMR.BIT.SMIF = 0;
	SCI0.SCMR.BIT.SINV = 0;
	SCI0.SCMR.BIT.SDIR = 0;
	
	SCI0.SEMR.BIT.ACS0 = 0;
	SCI0.SEMR.BIT.ABCS = 0;
	SCI0.SEMR.BIT.NFEN = 0;
	
	/* Bit Rate Setting */
	//SCI0.BRR = 38;//48MHz
	//SCI0.BRR = 78;//96MHz
	SCI0.BRR = 39;//96MHz, 19200bps
	
	
	//enableSCI();
	SCI0.SCR.BIT.TEIE = 0;
	SCI0.SCR.BIT.MPIE = 0;
	SCI0.SCR.BIT.RE = 1;
	SCI0.SCR.BIT.TE = 1;
	SCI0.SCR.BIT.RIE = 1;
	SCI0.SCR.BIT.TIE = 0;
	
	ICU.IPR[214].BIT.IPR = 10;
	ICU.IR[214].BIT.IR = 0;
	ICU.IER[0x1A].BIT.IEN6 = 1;
	
	return 0;
}

void clearSCI0ErrorFlag(void){
	char errorValue;
	SCI0.SCR.BIT.TE = 0;
	while(SCI0.SCR.BIT.TE == 1) ;
	errorValue = SCI0.SSR.BIT.ORER;
	SCI0.SSR.BIT.ORER = 0;
	while(SCI0.SSR.BIT.ORER == 1) ;
	errorValue = SCI0.SSR.BIT.FER;
	SCI0.SSR.BIT.FER = 0;
	while(SCI0.SSR.BIT.FER == 1) ;
	errorValue = SCI0.SSR.BIT.PER;
	SCI0.SSR.BIT.PER = 0;
	while(SCI0.SSR.BIT.PER == 1);
	
	errorValue = SCI0.RDR;
}

void enableSCI0(void){
	SCI0.SCR.BIT.TEIE = 0;
	SCI0.SCR.BIT.MPIE = 0;
	SCI0.SCR.BIT.RE = 1;
	SCI0.SCR.BIT.TE = 1;
	SCI0.SCR.BIT.RIE = 1;
	SCI0.SCR.BIT.TIE = 0;
}

/*
 * setting for SCI1
 */

int initSCI1(void){
	SYSTEM.PRCR.WORD = 0xA502;
	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	SCI1.SCR.BIT.TE = 0;
	SCI1.SCR.BIT.RE = 0;
	SCI1.SCR.BIT.TIE = 0;
	SCI1.SCR.BIT.RIE = 0;
	SCI1.SCR.BIT.TEIE = 0;
	
	//IOポートからモジュールポートへ機能変更
	PORT1.PMR.BIT.B5 = 1;
	PORT1.PMR.BIT.B6 = 1;
	
	//ピン機能選択レジスタ
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.P15PFS.BYTE = 0x0A;
	MPC.P16PFS.BYTE = 0x0A;
	
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	//まで
	
	SCI1.SCR.BIT.CKE = 0;//内蔵ボーレートジェネレータ（SCK0端子）
	
	SCI1.SIMR1.BIT.IICM = 0;//12C Mode Select
	SCI1.SPMR.BIT.CKPH = 0;//
	SCI1.SPMR.BIT.CKPOL = 0;
	
	/* Communication Data Format Setting */
	SCI1.SMR.BIT.CKS = 0x01;//Clock PCLK/4 (n=1)
	SCI1.SMR.BIT.MP = 0;//1対多通信機能を使うなら「１」
	SCI1.SMR.BIT.STOP = 0;//Stop Bit : 1bit
	SCI1.SMR.BIT.PM = 0;//Parity Mode : Even Parity
	SCI1.SMR.BIT.PE = 0;//Parity Enable : No Parity
	SCI1.SMR.BIT.CHR = 0;//Character Length : 8bit
	SCI1.SMR.BIT.CM = 0;//調歩同期式モード
	
	SCI1.SCMR.BIT.SMIF = 0;
	SCI1.SCMR.BIT.SINV = 0;
	SCI1.SCMR.BIT.SDIR = 0;
	
	SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = 0;
	SCI1.SEMR.BIT.NFEN = 0;
	
	/* Bit Rate Setting */
	//SCI1.BRR = 38;//48MHz 9600bps
	SCI1.BRR = 38;//96MHz 19200bps
	
	//enableSCI();
	SCI1.SCR.BIT.TEIE = 0;
	SCI1.SCR.BIT.MPIE = 0;
	SCI1.SCR.BIT.RE = 1;
	SCI1.SCR.BIT.TE = 1;
	SCI1.SCR.BIT.RIE = 1;
	SCI1.SCR.BIT.TIE = 0;
	
	ICU.IPR[217].BIT.IPR = 0xF;
	ICU.IR[217].BIT.IR = 0;
	ICU.IER[0x1B].BIT.IEN1 = 1;
	
	return 0;
}

void clearSCI1ErrorFlag(void){
	SCI1.SCR.BIT.TE = 0;
	SCI1.SSR.BIT.ORER = 0;
	SCI1.SSR.BIT.FER = 0;
	SCI1.SSR.BIT.PER = 0;
}

void enableSCI1(void){
	SCI1.SCR.BIT.TEIE = 0;
	SCI1.SCR.BIT.MPIE = 0;
	SCI1.SCR.BIT.RE = 1;
	SCI1.SCR.BIT.TE = 1;
	SCI1.SCR.BIT.RIE = 1;
	SCI1.SCR.BIT.TIE = 0;
}

/*
 * setting for SCI6
 */
int initSCI6(void){
	SYSTEM.PRCR.WORD = 0xA502;
	SYSTEM.MSTPCRB.BIT.MSTPB25 = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	SCI6.SCR.BIT.TE = 0;
	SCI6.SCR.BIT.RE = 0;
	SCI6.SCR.BIT.TIE = 0;
	SCI6.SCR.BIT.RIE = 0;
	SCI6.SCR.BIT.TEIE = 0;
	
	//IOポートからモジュールポートへ機能変更
	PORTB.PMR.BIT.B0 = 1;
	PORTB.PMR.BIT.B1 = 1;
	
	//ピン機能選択レジスタ
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.PB0PFS.BYTE = 0x0B;
	MPC.PB1PFS.BYTE = 0x0B;
	
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	//まで
	
	SCI6.SCR.BIT.CKE = 0;//内蔵ボーレートジェネレータ（SCK0端子）
	
	SCI6.SIMR1.BIT.IICM = 0;//12C Mode Select
	SCI6.SPMR.BIT.CKPH = 0;//
	SCI6.SPMR.BIT.CKPOL = 0;
	
	/* Communication Data Format Setting */
	SCI6.SMR.BIT.CKS = 0x01;//Clock PCLK/4 (n=1)
	SCI6.SMR.BIT.MP = 0;//1対多通信機能を使うなら「１」
	SCI6.SMR.BIT.STOP = 0;//Stop Bit : 1bit
	SCI6.SMR.BIT.PM = 0;//Parity Mode : Even Parity
	SCI6.SMR.BIT.PE = 0;//Parity Enable : No Parity
	SCI6.SMR.BIT.CHR = 0;//Character Length : 8bit
	SCI6.SMR.BIT.CM = 0;//調歩同期式モード
	
	SCI6.SCMR.BIT.SMIF = 0;
	SCI6.SCMR.BIT.SINV = 0;
	SCI6.SCMR.BIT.SDIR = 0;
	
	SCI6.SEMR.BIT.ACS0 = 0;
	SCI6.SEMR.BIT.ABCS = 0;
	SCI6.SEMR.BIT.NFEN = 0;
	
	/* Bit Rate Setting */
	//SCI0.BRR = 38;//48MHz
	//SCI0.BRR = 78;//96MHz
	SCI6.BRR = 39;//96MHz, 19200bps
	
	
	//enableSCI();
	SCI6.SCR.BIT.TEIE = 0;
	SCI6.SCR.BIT.MPIE = 0;
	SCI6.SCR.BIT.RE = 1;
	SCI6.SCR.BIT.TE = 1;
	SCI6.SCR.BIT.RIE = 1;
	SCI6.SCR.BIT.TIE = 0;
	
	ICU.IPR[232].BIT.IPR = 10;
	ICU.IR[232].BIT.IR = 0;
	ICU.IER[0x1D].BIT.IEN0 = 1;
	
	return 0;
}

void clearSCI6ErrorFlag(void){
	SCI6.SCR.BIT.TE = 0;
	SCI6.SSR.BIT.ORER = 0;
	SCI6.SSR.BIT.FER = 0;
	SCI6.SSR.BIT.PER = 0;
}

void enableSCI6(void){
	SCI6.SCR.BIT.TEIE = 0;
	SCI6.SCR.BIT.MPIE = 0;
	SCI6.SCR.BIT.RE = 1;
	SCI6.SCR.BIT.TE = 1;
	SCI6.SCR.BIT.RIE = 1;
	SCI6.SCR.BIT.TIE = 0;
}

