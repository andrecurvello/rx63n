#include "iodefine.h"

#define XBEE_SCI SCI12

int xbee_send_type17(unsigned char* addr, unsigned char* command, unsigned char param);
int xbee_send_type10(unsigned char* addr, unsigned char* data);
int xbee_checksum(unsigned char* data, unsigned short length);

/* 接続先パラメータ取得 */
int xbee_get_param(unsigned char* addr){
	return xbee_send_type17(addr, "IS", 2);
}
/* 接続先出力設定 */
//ON
int xbee_output_high(unsigned char* addr, unsigned char* port){
	return xbee_send_type17(addr, port, 1);
}
//OFF
int xbee_output_low(unsigned char* addr, unsigned char* port){
	return xbee_send_type17(addr, port, 0);
}
/* フレームタイプ１７送信用関数 */
int xbee_send_type17(unsigned char* addr, unsigned char* command, unsigned char param){
	unsigned char uc = 0;
	unsigned char lnum = 0;
	unsigned char comp_data[256];
	comp_data[0] = 0x7E;
	comp_data[1] = 0x00;
	comp_data[2] = 0x10;
	comp_data[3] = 0x17;
	comp_data[4] = 0x01;
	for(uc=0; uc<8; uc++){
		comp_data[5+uc] = addr[uc];
	}
	comp_data[13] = 0xFF;
	comp_data[14] = 0xFE;
	comp_data[15] = 0x02;
	comp_data[16] = command[0];
	comp_data[17] = command[1];
	if(param==0){
		comp_data[18] = 0x04;
		lnum = 19;
	}else if(param==1){
		comp_data[18] = 0x05;
		lnum = 19;
	}else{
		lnum = 18;
	}
	comp_data[lnum] = xbee_checksum(comp_data, 16);
	
	for(uc=0; uc<=lnum; uc++){
		while(XBEE_SCI.SSR.BIT.TEND == 0);//とりあえずSCI12（#defineで置き換えて変更しやすくする)
		XBEE_SCI.TDR = comp_data[uc];
	}
	
	return 0;
}

//データ送信用関数
int xbee_send_data(unsigned char* addr, unsigned char* data){
	return xbee_send_type10(addr, data);
}
//フレームタイプ１０送信用関数
int xbee_send_type10(unsigned char* addr, unsigned char* data){
	unsigned char uc = 0;
	unsigned char comp_data[256];
	unsigned short data_length = xbee_strlen(data);
	comp_data[0] = 0x7E;
	comp_data[1] = (data_length+18-4)>>8;
	comp_data[2] = data_length+18-4;
	comp_data[3] = 0x10;
	comp_data[4] = 0x01;
	for(uc=0; uc<8; uc++){
		comp_data[5+uc] = addr[uc];
	}
	comp_data[13] = 0xFF;
	comp_data[14] = 0xFE;
	comp_data[15] = 0x00;
	comp_data[16] = 0x00;
	for(uc=0; uc<data_length; uc++){
		comp_data[17+uc] = data[uc];
	}
	comp_data[17+data_length] = xbee_checksum(comp_data, 17 + data_length);
	
	for(uc=0; uc<=17+data_length; uc++){
		while(XBEE_SCI.SSR.BIT.TEND == 0);
		XBEE_SCI.TDR = comp_data[uc];
	}
	//返答パケットはフレームタイプ8B
	
	return 0;
}

/* 文字列の長さを返す */
int xbee_strlen(unsigned char* chrs){
	unsigned short length = 0;
	while(*chrs++ != '\0'){
		length++;
	}
	return length;
}

/* チェックサム生成 */
int xbee_checksum(unsigned char* data, unsigned short length){
	unsigned char checksum = 0;
	unsigned char uc;
	
	for(uc=3; uc<length; uc++){
		checksum += data[uc];
	}
	checksum &= 0xFF;
	checksum = 0xFF - checksum;
	
	return checksum;
}