#include <stdio.h>
#include "iodefine.h"

#define MUSICNUMBERMAX 7
#define MUSIC_COST 2

void playSound(void);
void stopSound(void);

unsigned char ryzhmCnt = 0;
unsigned char nextRyzhmCntFlag = 0;

unsigned char nextMusicIntervalFlag = 0;
unsigned char sinNow  = 0;
extern float volume;
extern unsigned short musicFreq;
extern unsigned char displayFlag;
extern char currentMI[4];
/* 256 c */
unsigned char sounds[] = {
	127,130,133,136,139,142,145,148,
	151,154,157,161,164,166,169,172,
	175,178,181,184,187,189,192,195,
	197,200,202,205,207,210,212,214,
	217,219,221,223,225,227,229,231,
	232,234,236,237,239,240,242,243,
	244,245,246,247,248,249,250,251,
	251,252,252,253,253,253,253,253,
	253,253,253,253,253,252,252,251,
	251,250,249,249,248,247,246,245,
	243,242,241,239,238,236,235,233,
	231,230,228,226,224,222,220,218,
	215,213,211,209,206,204,201,199,
	196,193,191,188,185,182,180,177,
	174,171,168,165,162,159,156,153,
	150,147,144,141,137,134,131,128,
	126,123,120,117,113,110,107,104,
	101,98,95,92,89,86,83,80,
	77,74,72,69,66,63,61,58,
	55,53,50,48,45,43,41,39,
	36,34,32,30,28,26,24,23,
	21,19,18,16,15,13,12,11,
	9,8,7,6,5,5,4,3,
	3,2,2,1,1,1,1,1,
	1,1,1,1,1,2,2,3,
	3,4,5,6,7,8,9,10,
	11,12,14,15,17,18,20,22,
	23,25,27,29,31,33,35,37,
	40,42,44,47,49,52,54,57,
	59,62,65,67,70,73,76,79,
	82,85,88,90,93,97,100,103,
	106,109,112,115,118,121,124,127
};

unsigned char soundInterval[] = {
	191,//シ
	180,//ド
	159,//レ
	142,//ミ
	134,//ファ
	120,//ソ
	89,//ラ
	95,//シ
	90//ド
};

unsigned char musicMap[] = {
	180, 159, 142, 134, 142, 159, 180, 142, 134, 120, 89, 120, 134, 142, 180, 180, 180, 180, 180,
	180, 159, 159, 142, 142, 134, 134, 142, 159, 180
};

unsigned char songs[MUSICNUMBERMAX][100] =  {
	"ﾄﾞﾚﾐﾌｧﾐﾚﾄﾞﾐﾌｧｿﾗｿﾌｧﾐﾄﾞﾄﾞﾄﾞﾄﾞﾄﾞﾄﾞﾚﾚﾐﾐﾌｧﾌｧﾐﾚﾄﾞ",
	"ﾄﾞﾄﾞﾚﾐﾐｿﾐﾐﾚﾚﾄﾞﾐﾐﾌｧｿﾄﾞﾗﾄﾞｿﾄﾞﾄﾞｼﾗｿﾗﾗﾗﾗｿﾄﾞﾄﾞﾚﾐﾐｿﾐﾐﾚﾚﾄﾞ",
	"ﾗﾗﾌｧﾚﾚﾐﾌｧﾚｿﾗｿﾌｧﾐﾚﾄﾞﾗﾗﾌｧﾚﾚﾐﾌｧﾚｿﾗｿﾐﾌｧ",
	"ﾄﾞﾐﾌｧｿﾄﾞﾐﾌｧｿﾄﾞﾐﾌｧｿﾐﾄﾞﾓﾚﾐﾐﾚﾄﾞﾄﾞﾐｿｿﾌｧﾐﾌｧｿﾐﾄﾞﾚﾄﾞ",
	"ｿﾌｧﾐﾚﾐﾌｧﾚﾄﾞﾐﾌｧｿﾐﾚﾐﾌｧﾚﾐﾌｧｿﾐﾚﾐﾌｧﾚｿﾌｧﾐﾚﾐﾌｧﾚﾄﾞ",
	"ﾐｿﾗｿﾐｿﾄﾞﾄﾞﾗﾗｿﾐﾄﾞﾚﾐﾐｿﾗｿﾐｿﾄﾞﾄﾞﾗﾗｿﾐﾚﾐﾄﾞﾚﾐﾚｿﾗﾗｿﾗﾄﾞﾄﾞｼﾗｿｿﾐﾐｿﾗｿﾐｿﾄﾞﾄﾞﾗﾗｿﾐﾚﾐﾄﾞ",
	"ﾄﾞﾚﾐﾄﾞミドミﾐﾚﾚﾐﾌｧﾌｧﾐﾚﾌｧﾐﾌｧｿﾐｿﾐｿﾌｧｿﾗﾗｿﾌｧﾗｿﾄﾞﾚﾐﾌｧｿﾗﾗﾚﾐﾌｧｿﾗｼｼﾐﾌｧｿﾗｼﾄﾞﾄﾞｼﾗﾌｧｼｿﾄﾞﾄﾞｼﾗｿﾌｧﾐﾚ"
};

unsigned char ryzhm[26] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1, 0
};

/* playSoundState = 1 */
void playSound(void){
	unsigned char cc;
	if(nextRyzhmCntFlag > 5){
		if(ryzhmCnt < 25) ryzhmCnt++;
		else ryzhmCnt = 0;
		nextRyzhmCntFlag = 0;
	}
	cc = sounds[sinNow];
	DA.DADR0 = sounds[sinNow];// * volume * ryzhm[ryzhmCnt];
	//displayFlag = 1;
}

/* playSoundState = 0 */
void stopSound(void){
	ryzhmCnt = 0;
	nextRyzhmCntFlag = 0;
	DA.DADR0 = 1023;
}

void playMusic(void){
	//ドレミファミレドミファソラソファミドドドドドドレレミミファファミレド
	static unsigned char ch = 0;
	if(nextMusicIntervalFlag > 3){
		if(ch < 28) ch++;
		else ch = 0;
		nextMusicIntervalFlag = 0;
	}
	CMT0.CMCOR = musicMap[ch] / 2; // / MUSIC_COST;
	musicFreq = musicMap[ch];// 仮
	playSound();
}

void playMusicX(unsigned char ns){
	const unsigned char chf = (unsigned char)strlen(songs[ns]);
	static unsigned char ch = 0;
	if(nextMusicIntervalFlag > 2){
		switch(songs[ns][ch]){
			case 'ﾄ':
			CMT0.CMCOR = 180 / MUSIC_COST;
			musicFreq = 261;
			sprintf(currentMI, "ﾄﾞ");
			ch++;
			break;
			case 'ﾚ':
			CMT0.CMCOR = 159 / MUSIC_COST;
			musicFreq = 294;
			sprintf(currentMI, "ﾚ  ");
			break;
			case 'ﾐ':
			CMT0.CMCOR = 142 / MUSIC_COST;
			musicFreq = 330;
			sprintf(currentMI, "ﾐ  ");
			break;
			case 'ﾌ':
			CMT0.CMCOR = 134 / MUSIC_COST;
			musicFreq = 350;
			sprintf(currentMI, "ﾌｧ");
			ch++;
			break;
			case 'ｿ':
			CMT0.CMCOR = 120 / MUSIC_COST;
			musicFreq = 392;
			sprintf(currentMI, "ｿ  ");
			break;
			case 'ﾗ':
			CMT0.CMCOR = 89 / MUSIC_COST;
			musicFreq = 440;
			sprintf(currentMI, "ﾗ  ");
			break;
			case 'ｼ':
			CMT0.CMCOR = 95 / MUSIC_COST;
			musicFreq = 494;
			sprintf(currentMI, "ｼ  ");
			break;
		}
		if(ch < chf) ch++;
		else ch = 0;
		nextMusicIntervalFlag = 0;
	}
	playSound();
}