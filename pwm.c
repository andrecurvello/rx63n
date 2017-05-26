#include "iodefine.h"

/*
 * �}���`�t�@���N�V�����^�C�}�p���X���j�b�g�ł�PWM�o��
 */
void initPWM(void){
	//MTIOC3A�p�|�[�g�ݒ�
	PORT1.PDR.BIT.B7 = 1;
	PORT1.PMR.BIT.B7 = 1;
	//MTIOC1A�p�|�[�g�ݒ�
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
	
	//TGRA �J�E���^�N���b�N�̑I��
	//MTIOC1A
	MTU1.TCR.BYTE = 0x48;//TGRB�̃R���y�A�}�b�`��TCNT=0, ������G�b�W�ŃJ�E���g
	MTU1.TMDR.BYTE = 0x02;//PWM���[�h�P
	MTU1.TIOR.BYTE = 0x12;//A:����low - �R���y�A�}�b�`��high, B:����low - �R���y�A�}�b�`��low
	
	MTU3.TCR.BYTE = 0x48;//TGRB�̃R���y�A�}�b�`�E�C���v�b�g�L���v�`����TCNT�N���A + ����������G�b�W�ŃJ�E���g
	MTU3.TMDR.BYTE = 0x02;//PWM���[�h�P
	MTU3.TIORH.BYTE = 0x12;//A:low-high, B:low-low
	MTU3.TIER.BYTE = 0x01;//TGRA���荞�݂�����
	
	MTU.TRWER.BIT.RWE = 0x1;//�v���e�N�g����
	MTU1.TGRA = 1000;//�R���y�A�}�b�`��HIGH(duty)
	MTU1.TGRB = 10000;//�R���y�A�}�b�`��LOW(cycle)
	MTU3.TGRA = 1000;//�R���y�A�}�b�`��HIGH(duty)
	MTU3.TGRB = 10000;//�R���y�A�}�b�`��LOW(cycle)
	MTU.TRWER.BIT.RWE = 0x0;//�v���e�N�g�ݒ�
	
	MTU.TSTR.BIT.CST1 = 1;//MTU0-TCNT�J�E���^�J�n
	MTU.TSTR.BIT.CST3 = 1;//MTU3-TCNT�J�E���^�̊J�n
}

/* �����i��������j

PWM���[�h1�i�ő�8����PWM�o�́j
TGRA��TGRB�ATGRC��TGRD���y�A�Ŏg�p�B���ꂼ��AMTIOCnA�AMTIOCnC�[�q����PWM�o�͂𐶐�����B
���@TGRA,C�̃R���y�A�}�b�` -> TIOR��IOA�AIOC�r�b�g�Ŏw�肵���o��
���@TGRB,D�̃R���y�A�}�b�` -> TIOR��IOB�AIOD�r�b�g�Ŏw�肵���o��

MTIOC0A - �g�p���W�X�^ - MTU0.TGRA + MTU0.TRGB - 
MTIOC0C - �g�p���W�X�^ - MTU0.TGRC + MTU0.TGRD - P32
MTIOC1A - �g�p���W�X�^ - MTU1.TGRA + MTU1.TRGB - P20
MTIOC2A - �g�p���W�X�^ - MTU2.TGRA + MTU2.TGRB - P26
MTIOC3A - �g�p���W�X�^ - MTU3.TGRA + MTU3.TRGB - P14*P17
MTIOC3C - �g�p���W�X�^ - MTU3.TGRC + MTU3.TGRD - P16
MTIOC4A - �g�p���W�X�^ - MTU4.TGRA + MTU4.TRGB - 
MTIOC4C - �g�p���W�X�^ - MTU4.TGRC + MTU4.TGRD - 

PWM���[�h�Q�i��������ƕ��p���邱�Ƃɂ��ő�8����PWM�o�́j
����������́A������TCNT�̒l�𓯎��ɏ��������邱�Ƃ��ł���B
��TCR�̐ݒ�ɂ�蕡����TCNT�𓯎��ɃN���A���邱�Ƃ��ł���i�����N���A�j
��TGR��1�{���������W�X�^�A����TGR���f���[�e�B���W�X�^�Ɏg�p�B

MTIOC0A - �g�p���W�X�^ - MTU0.TGRA
MTIOC0B - �g�p���W�X�^ - MTU0.TGRB
MTIOC0C - �g�p���W�X�^ - MTU0.TGRC
MTIOC0D - �g�p���W�X�^ - MTU0.TGRD
MTIOC1A - �g�p���W�X�^ - MTU1.TGRA
MTIOC1B - �g�p���W�X�^ - MTU1.TGRB
MTIOC2A - �g�p���W�X�^ - MTU2.TGRA
MTIOC2B - �g�p���W�X�^ - MTU2.TGRB

��TCNT�i�^�C�}�J�E���^�j
16�r�b�g�̓ǂݏ����\�ȃJ�E���^�B���Z�b�g��0000h�ɏ�����

���g�O���o��
0�E1�̓���ւ�

�i�����܂Łj */