#include "iodefine.h"

/*
 * �}���`�t�@���N�V�����^�C�}�p���X���j�b�g�ł�PWM�o��
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
	
	//TGRA �J�E���^�N���b�N�̑I��
	MTU3.TCR.BYTE = 0x28;//TGRA�̃R���y�A�}�b�`�E�C���v�b�g�L���v�`����TCNT�N���A + ����������G�b�W�ŃJ�E���g
	MTU3.TMDR.BYTE = 0x02;//PWM���[�h�P
	MTU3.TIORH.BYTE = 0x37;//����LOW�R���y�A�}�b�`�Ńg�O��(TRGB�̐ݒ�)�@�{�@����High�R���y�}�b�`�Ńg�O���iTRGA�̐ݒ�)
	MTU3.TIER.BYTE = 0x01;//TGRA���荞�݂�����
	
	MTU.TRWER.BIT.RWE = 0x1;//�v���e�N�g����
	MTU3.TGRA = 48000;
	MTU3.TGRB = 24000;
	MTU.TRWER.BIT.RWE = 0x0;//�v���e�N�g�ݒ�
	
	MTU.TSTR.BIT.CST3 = 1;//TCNT�J�E���^�̊J�n
}

