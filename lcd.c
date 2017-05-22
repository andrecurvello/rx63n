#include <stdio.h>
#include "iodefine.h"

#define RS PORT4.PODR.BIT.B4
#define E  PORT4.PODR.BIT.B5
#define RW PORT4.PODR.BIT.B6

#define LCDINITWAIT 768000
#define LCDWAIT 76800

void initLCD(void);
void displayOn(unsigned char on);
void displayClr(void);
void displayLocate(unsigned char);
void reverseString(unsigned char*);
void displayLocateCG(unsigned char);
void shiftCursor(unsigned char length, unsigned char direction);
void displayChar(unsigned char);
void displayString(unsigned char*);

void reverseString(unsigned char* string){
	unsigned char* cp = string;
	unsigned char* p = (unsigned char*)malloc(sizeof(char)*(strlen(string)+1));
	int len = strlen(string);
	strcpy(p, string);
	while(len > 0){
		*string++ = *(p + len-1);
		len--;
	}
	string = cp;
}

void displayString(unsigned char* data){
	while(*data!='\0'){
		displayChar(*data);
		data++;
	}
}

//文字表示
void displayChar(unsigned char data){
	unsigned long time;
	
	RS = 1;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	PORT6.PODR.BYTE = data >> 4;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	PORT6.PODR.BYTE = data;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
}

//表示ON/OFF関数
void displayOn(unsigned char on){
	unsigned long time;
	RS = 0;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	E = 1;
	PORT6.PODR.BYTE = 0x00;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	E = 1;
	if(on == 1){
		PORT6.PODR.BYTE = 0x0E;
	}else{
		PORT6.PODR.BYTE = 0x0A;
	}
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
}

//表示クリア＋カーソルホーム
void displayClr(void){
	unsigned long time;
	RS = 0;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	PORT6.PODR.BYTE = 0x00;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	PORT6.PODR.BYTE = 0x01;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
}

void shiftCursor(unsigned char length, unsigned char direction){
	unsigned char len;
	unsigned long time;
	RS = 0;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	if(direction==0){
		for(len=0; len<length; len++){
			E = 1;
			PORT6.PODR.BYTE = 0x01;
			for(time=0; time<LCDWAIT; time++) ;
			E = 0;
			for(time=0; time<LCDWAIT; time++) ;
			
			E = 1;
			PORT6.PODR.BYTE = 0x04;
			for(time=0; time<LCDWAIT; time++) ;
			E = 0;
			for(time=0; time<LCDWAIT; time++) ;
		}
	}else{
		for(len=0; len<length; len++){
			E = 1;
			PORT6.PODR.BYTE = 0x01;
			for(time=0; time<LCDWAIT; time++) ;
			E = 0;
			for(time=0; time<LCDWAIT; time++) ;
			
			E = 1;
			PORT6.PODR.BYTE = 0x00;
			for(time=0; time<LCDWAIT; time++) ;
			E = 0;
			for(time=0; time<LCDWAIT; time++) ;
		}
	}
}

//表示開始位置の決定(DD RAMアドレスセット)
void displayLocate(unsigned char location){
	unsigned long time;
	unsigned char loc;
	RS = 0;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	loc = location >> 4;
	loc = loc | 0x08;
	PORT6.PODR.BYTE = loc;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	loc = location & 0x0F;
	PORT6.PODR.BYTE = loc;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
}

//CG RAM アドレスセット
void displayLocateCG(unsigned char location){
	unsigned long time;
	unsigned char loc;
	RS = 0;
	RW = 0;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	loc = location >> 4;
	loc = (loc | 0x04) & 0x07;
	PORT6.PODR.BYTE = loc;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
	
	E = 1;
	loc = location & 0x0F;
	PORT6.PODR.BYTE = loc;
	for(time=0; time<LCDWAIT; time++) ;
	E = 0;
	for(time=0; time<LCDWAIT; time++) ;
}

//SC2004キャラクタ液晶ディスプレーの初期設定関数の本体
void initLCD(void)
{
	unsigned long time;
	for(time = 0;time < 3000;++time);//15ms以上待つ//125ms
	RS = 0;
	RW = 0;
	E = 0;
	
	for(time = 0;time < LCDINITWAIT; ++time);//4.1ms以上待つ
	E = 1;
	PORT6.PODR.BIT.B3 = 0;
	PORT6.PODR.BIT.B2 = 0;
	PORT6.PODR.BIT.B1 = 1;
	PORT6.PODR.BIT.B0 = 1;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time < LCDINITWAIT; ++time);
	E = 1;
	PORT6.PODR.BIT.B3 = 0;
	PORT6.PODR.BIT.B2 = 0;
	PORT6.PODR.BIT.B1 = 1;
	PORT6.PODR.BIT.B0 = 1;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time < LCDINITWAIT; ++time);
	E = 1;
	PORT6.PODR.BYTE = 0x03;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time < LCDINITWAIT; ++time);
	E = 1;
	PORT6.PODR.BYTE = 0x02;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x02;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x08;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x00;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x08;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x00;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x01;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x00;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	
	for(time = 0;time<LCDWAIT;++time);
	E = 1;
	PORT6.PODR.BYTE = 0x06;
	for(time = 0;time<LCDWAIT;++time);
	E = 0;
	for(time = 0;time<LCDWAIT;++time);
}