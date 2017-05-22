#include "iodefine.h"

void initS12ADC(void);

//RX63N������12�r�b�gA/D�ϊ���̏����ݒ�֐��{��
void initS12ADC(void)
{
	//12�r�b�gAD�ϊ���̃��W���[���X�g�b�v��Ԃ�����
	SYSTEM.PRCR.WORD = 0xA502;
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;
	SYSTEM.PRCR.WORD = 0xA500;//PMR���W�X�^��ėp���o�͂�
	PORT4.PDR.BIT.B0 = 0;
	PORT4.PDR.BIT.B1 = 0;
	PORT4.PDR.BIT.B2 = 0;
	PORT4.PMR.BIT.B0 = 0;
	PORT4.PMR.BIT.B1 = 0;
	PORT4.PMR.BIT.B2 = 0;
	
	MPC.PWPR.BYTE &= ~0x80;
	MPC.PWPR.BYTE |= 0x40;
	MPC.P40PFS.BIT.ASEL = 1;//�A�i���O�[�q�ɐݒ�iASEL�r�b�g��1�Ɂj
	MPC.P41PFS.BIT.ASEL = 1;
	MPC.P42PFS.BIT.ASEL = 1;
	//MPC.P40PFS.BYTE = 0x80;
	MPC.PWPR.BYTE &= 0x40;
	MPC.PWPR.BYTE |= ~0x80;
	
	S12AD.ADCSR.BIT.ADST = 0;//stop ADC�R���o�[�^
	S12AD.ADCSR.BYTE = 0x10;//AD�R���g���[�����W�X�^�̐ݒ�
	
	S12AD.ADANS0.WORD = 0x07;//AN000�EAN001�EAN002������
	//S12AD.ADANS1.WORD = 0x00;
	
	S12AD.ADCER.BIT.ADRFMT = 0;//�E�Â�
	S12AD.ADCSR.BIT.ADCS = 1;
	
	ICU.IPR[102].BIT.IPR = 8;//�D��
	ICU.IR[102].BIT.IR = 0;//�t���OOFF
	ICU.IER[0x0C].BIT.IEN6 = 1;//���荞��ON(�R���g���[��)
}