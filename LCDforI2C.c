/*
 * Ｉ２Ｃ接続小型キャラクタＬＣＤモジュール　８ｘ２行
 * [AQM0802A-RN-GBW]
 */

//初期化
int initLCDmini(void)
{
	//以下のディレイはfor文で。

	unsigned short time;

	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x38);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x39);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x14);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x70);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x56);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x6C);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x38);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x0C);
	
	for(time = 0; time < 1000 ; time++);
	i2cWrite(0x3E, 0x00, 0x01);
	
	for(time = 0; time < 1000 ; time++);
	
	return 0;
}

//位置決め
int LCDminiloc(char row, char column)
{
	int err = 0;
	unsigned char data = 0;	

	if(row==1)
	{
		data = 0x80 + (column - 1);
	}
	else if (row==2)
	{
		data = 0xC0 +  (column - 1);
	}
	else
	{
		data = 0x80;
	}
	
	err = i2cWrite(0x3E, 0x00, data);	
	
	return(err);
}

//表示
int LCDministr(char *string)
{
	int	err = 0;
	while(*string != '\0')
	{
		err = i2cWrite(0x3E, 0x40, *string);
		//delay(1);
		string++;
	}
	return(err);
}