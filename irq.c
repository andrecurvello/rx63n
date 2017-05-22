/*
 *�O��������
 */

#include "iodefine.h"

void initIRQ(void);

void initIRQ(void){
	int i;
	ICU.IER[8].BYTE = 0x00;
	
	//�f�W�^���t�B���^�@���r�b�g
	ICU.IRQFLTE0.BYTE = 0x00;
	ICU.IRQFLTC0.WORD = 0xFFFF;//64clock��1��
	//�f�W�^���t�B���^�@���r�b�g�@IRQ0-7
	ICU.IRQFLTE0.BYTE = 0xFF;
	
	PORTD.PDR.BYTE = 0;
	PORTD.PMR.BYTE = 0x00;//�|�[�g���[�h���W�X�^(0:�ėp���o�̓|�[�g, 1:���Ӌ@�\)
	
	//�}���`�t�@���N�V�����s���R���g���[��
	MPC.PWPR.BYTE &= ~0x80;//PFS�v���e�N�g����
	MPC.PWPR.BYTE |= 0x40;
	
	MPC.PD0PFS.BYTE = 0x40;
	MPC.PD1PFS.BYTE = 0x40;
	MPC.PD2PFS.BYTE = 0x40;
	MPC.PD3PFS.BYTE = 0x40;
	MPC.PD4PFS.BYTE = 0x40;
	MPC.PD5PFS.BYTE = 0x40;
	MPC.PD6PFS.BYTE = 0x40;
	MPC.PD7PFS.BYTE = 0x40;
	
	MPC.PWPR.BYTE &= ~0x40;
	MPC.PWPR.BYTE |= 0x80;//PFS�v���e�N�g�ݒ�
	
	for(i=0; i<8; i++){
		ICU.IRQCR[i].BIT.IRQMD = 1;
	}
	
	for(i=64; i<72; i++){
		ICU.IPR[i].BYTE = i-63;
		ICU.IR[i].BYTE = 0x00;
	}
	
	ICU.IER[8].BYTE = 0xFF;
}