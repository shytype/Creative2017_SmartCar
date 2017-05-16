#define __SDCARD_C_
#include "includes.h"


static void SD_SPI_to_4M(void);
static void DSPI_send_8_clocks(void);
static BYTE DSPI_read_write_byte(BYTE byte_write);
static BYTE SD_reset(void);
static BYTE SD_send_cmd(BYTE cmd, DWORD var);
byte text[2][4]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};
byte (*p)[4]=text;
volatile byte (*camera)[COLUMNS]=pic;
byte RowCol1[3]={0xAA,0xBB,0xFF};
byte RowCol2[3]={0xFF,0xBB,0xAA};
byte Camera1[3]={0x55,0xAA,0xFF};
byte Camera2[3]={0xFF,0xAA,0x55};
byte row=ROWS;
byte column=COLUMNS;

void init_DSPI_1(void)
{
	DSPI_1.MCR.R = 0x803f0001;     /* Configure DSPI_1 as master，空闲时为高，HALT MODE */
	DSPI_1.CTAR[0].R = 0x3E0A7729;	//未使用 用于发送8bits 调整极性为1，相位为1，调整波特率为低速31.25kbit/s
	DSPI_1.CTAR[1].R = 0x38087726;  //TF 发送8bits 极性为0，相位为0，baud rate=625k/s
	DSPI_1.CTAR[2].R = 0x3E0A7724;  //L3G4200D 发送8bits 极性为1，相位为1，baud rate=1m/s
	DSPI_1.CTAR[3].R = 0x380A7720;	//OLED 发送8bits 极性为0，相位为0，baud rate=8m/s
	DSPI_1.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
	SIU.PCR[34].R = 0x0604;	//PC2 SCK_1
	//SIU.PSMI[7].R = 0;	//SCK_1 PCR[34]
	SIU.PCR[35].R = 0x0503;	//PC3 CS0_1 TF
	//SIU.PSMI[9].R = 0;	//CS0_1 PCR[35]
	SIU.PCR[36].R = 0x0104;	//PC4 SIN_1
	//SIU.PSMI[8].R = 0;	//SIN_1 PCR[36]
	SIU.PCR[62].R = 0x0604;	//PD14 CS1_1 OLED
	SIU.PCR[63].R = 0x0604;	//PD15 CS2_1
	SIU.PCR[67].R = 0x0A04;	//PE3 SOUT_1
	SIU.PCR[74].R = 0x0A04;	//PE10 CS3_1 L3G
	SIU.PCR[75].R = 0x0A04;	//PE11 CS4_1
	DSPI_1.RSER.B.TCFRE = 0;	//关闭传输完成中断
}


static void SD_SPI_to_4M(void)
{
	DSPI_1.CTAR[1].R = 0x3E0A7721;	//用于发送8bits 调整极性为1，相位为1，调整波特率为4M
}


static BYTE DSPI_read_write_byte(BYTE byte_write)
{
	DWORD tmp_tx = 0x00000000;
	WORD tmp_rx;
	int i = 0;
	
	tmp_tx |= 0x98010000;//CS1
	tmp_tx |= (DWORD)byte_write;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF) { }
	tmp_rx = (WORD)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return (BYTE)tmp_rx;
}


//----------------by-JJ----------------//
//----------------此函数与发送0xff不同，片选拉高----------------//
static void DSPI_send_8_clocks(void)
{
	DWORD tmp_tx = 0x980000FF;
	WORD tmp_rx;
	
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (WORD)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
}


/*-----------------------------------------------------------------------*/
/* 初始化TF卡                                                                         */
/* 正常返回0                                                               */
/* 不成功卡着一直试                                                                */
/*-----------------------------------------------------------------------*/
SWORD SD_init(void)
{
	while (SD_reset()) { }
	SD_SPI_to_4M();
	return 0;
}


static BYTE SD_send_cmd(BYTE cmd, DWORD var)
{
	BYTE rev, retry;
	
	DSPI_send_8_clocks(); //TF卡规定的头
	DSPI_read_write_byte(cmd | 0x40);	//分别写入命令;第1、2位=01
	DSPI_read_write_byte((BYTE)(var>>24));	//将字节地址写入到cmd字节序列
	DSPI_read_write_byte((BYTE)(var>>16));
	DSPI_read_write_byte((BYTE)(var>>8));
	DSPI_read_write_byte((BYTE)(var));
	DSPI_read_write_byte(0x95);//TF卡规定的尾
	retry = 0;
	while((rev = DSPI_read_write_byte(0xFF)) == 0xFF)	//等待响应【？】
	{
		if(retry++>100)
		{
			break;	//超时退出
		}
	}
	
	return rev;
}


static BYTE SD_reset(void)
{
	BYTE i;
	BYTE retry;
	BYTE rev = 0x00;
	
	for(i=0; i<10; i++)
	{
		DSPI_send_8_clocks();	//至少74个时钟，必须的!!!
	}
	//SD卡复位
	//发送CMD0，正常跳出表示成功进入idle状态
	for(retry=0; rev!=0x01; retry++)
	{
		rev = SD_send_cmd(0, 0);	//发idle命令【？】
		if(retry>100)
		{
			return 1;
		}
	}
	//SD卡初始化(CMD1/CMD55+ACMD41两种方法)
	for(retry=0; rev!=0x00; retry++)
	{
		rev = SD_send_cmd(1, 0);	//发idle命令
		if(retry>100)
		{
			return 2;
		}
	}
	rev = SD_send_cmd(59, 0);	//关crc
	rev = SD_send_cmd(16, 512);	//设扇区大小512
	
	return 0;	//正常返回
}


//----------------by-徐博----------------//
BYTE SD_read_block(DWORD sector, BYTE *buffer)	//sector=address,buffer=数据缓冲区
{
	BYTE rev;          
	WORD i;
	
	rev = SD_send_cmd(17, sector<<9);  //读命令,发送CMD17,收到0x00表示成功 	
	if(rev != 0x00)
	{
		return rev;
	}
	
	//等数据的开始
	while(DSPI_read_write_byte(0xFF) != 0xFE){}//连续读直到读到开始字节0xfe
	for(i=0; i<SD_SECTOR_SIZE; i++)	//读512个数据，写入数据缓冲区
	{
		*buffer++ = DSPI_read_write_byte(0xFF);
	}
	DSPI_read_write_byte(0xFF);	//读两个CRC字节
	DSPI_read_write_byte(0xFF);  
	DSPI_send_8_clocks();	//按时序补8个时钟
	
	return 0;	//读取操作成功
}


//----------------by-JJ----------------//
BYTE SD_read_multiple_block(DWORD sector, DWORD n, BYTE buffer[][SD_SECTOR_SIZE])
{
	BYTE rev;          
	WORD i;
	DWORD j;
	
	rev = SD_send_cmd(18, sector<<9);
	if(rev != 0x00)
	{
		return rev;
	}
	for (j=0; j<n; j++)
	{
		while(DSPI_read_write_byte(0xFF) != 0xFE){}
		for(i=0; i<SD_SECTOR_SIZE; i++)
		{
			buffer[j][i] = DSPI_read_write_byte(0xFF);
		}
		DSPI_read_write_byte(0xFF);
		DSPI_read_write_byte(0xFF);  
	}
	SD_send_cmd(12, 0);	//发送STOP_TRANSMISSION
	DSPI_send_8_clocks();
	
	return 0;
}


//----------------by-徐博----------------//
BYTE SD_write_block(DWORD sector, const BYTE *buffer)	//sector=address,buffer=数据缓存区
{
	BYTE rev;
	WORD i;
	
	rev = SD_send_cmd(24, sector<<9);	//写命令,sector<<9:将地址左移9位,address=address*512,将块地址（扇区地址）转为字节地址
	if(rev != 0x00)
	{
		return rev;	//收到0x00表示成功
	}
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xfe);	//发开始符

	for(i=0; i<SD_SECTOR_SIZE; i++)	//送512字节数据
	{
		DSPI_read_write_byte(*buffer++);
	}
	
	DSPI_read_write_byte(0xFF);	//写入2个字节的CRC校验码，Don't care
	DSPI_read_write_byte(0xFF);

	rev = DSPI_read_write_byte(0xFF);	//读取返回值
	if((rev&0x1f) != 0x05)	//若返回值为XXX00101,说明数据已经被SD卡接受
	{
		return rev;	//写入失败
	}
	//等待操作完
	while(!DSPI_read_write_byte(0xFF)){}	//等待SD卡不忙
	DSPI_send_8_clocks();	//按SD卡操作时序补8个时钟
	
	return 0;	//说明写扇区操作成功
}


//----------------by-JJ----------------//
BYTE SD_write_multiple_block(DWORD sector, DWORD n, const BYTE buffer[][SD_SECTOR_SIZE])
{
	BYTE rev;
	WORD i;
	DWORD j;
	
	rev = SD_send_cmd(25, sector<<9);
	if(rev != 0x00)
	{
		return rev;
	}
	DSPI_read_write_byte(0xff);
	DSPI_read_write_byte(0xff);
	for(i=0; i<n; i++)
	{
		DSPI_read_write_byte(0xfc);
		for(j=0; j<SD_SECTOR_SIZE; j++)
		{
			DSPI_read_write_byte(buffer[i][j]);
		}
		DSPI_read_write_byte(0xFF);
		DSPI_read_write_byte(0xFF);
		rev = DSPI_read_write_byte(0xFF);
		if((rev&0x1f) != 0x05)
		{
			return rev;
		}
		while(!DSPI_read_write_byte(0xFF)){}
	}
	DSPI_read_write_byte(0xfb);
	while(!DSPI_read_write_byte(0xFF)){}
	//SD_send_cmd(12, 0);	//这句话导致后续的多扇区读出错
	DSPI_send_8_clocks();
	
	return 0;
}

/*************************************************************/
/*                        清空缓冲区                         */
/*************************************************************/
void clear_sd_buffer(BYTE buffer[][SD_SECTOR_SIZE])
{
	int i, j;
	
	for (i=0; i<SD_BUFFER_SECTOR_MAX; i++)
	{
		for (j=0; j<SD_SECTOR_SIZE; j++)
		{
			buffer[i][j] = 0;
		}
	}
}


/*-----------------------------------------------------------------------*/
/* 读取方向舵机数据从TF卡                                                      */
/* 正常返回0                                                                          */
/*-----------------------------------------------------------------------*/
SWORD read_steer_helm_data_from_TF()
{
	FIL fil;
	TCHAR *tchar = "STEHEL";
	UINT br;
	
	if (FR_OK == f_open(&fil, tchar, FA_READ))
	{
		if (FR_OK == f_read(&fil, (void *)&data_steer_helm_basement, sizeof(data_steer_helm_basement), &br))
		{
			if (FR_OK == f_close(&fil))
			{
				
			}
			else
			{
				return 3;
			}
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}
	
	return 0;
}


/*-----------------------------------------------------------------------*/
/* 写入方向舵机数据到TF卡                                                       */
/* 正常返回0                                                                          */
/*-----------------------------------------------------------------------*/
SWORD write_steer_helm_data_to_TF()
{
	FIL fil1, fil2;
	TCHAR *tchar = "STEHEL";
	UINT wr;
	
	if (FR_OK == f_open(&fil1, tchar, FA_CREATE_ALWAYS))
	{
		if (FR_OK == f_close(&fil1))
		{
			
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}

	if (FR_OK == f_open(&fil2, tchar, FA_WRITE))
	{
		if (FR_OK == f_write(&fil2, (const void *)&data_steer_helm_basement, sizeof(data_steer_helm_basement), &wr))
		{
			if (FR_OK == f_close(&fil2))
			{
				return 0;
			}
			else
			{
				return 5;
			}
		}
		else
		{
			return 4;
		}
	}
	else
	{
		return 3;
	}
}


/*-----------------------------------------------------------------------*/
/* 读取设备号从TF卡                                                                */
/* 正常返回0                                                                          */
/*-----------------------------------------------------------------------*/
int read_device_no_from_TF()
{
	FIL fil;
	TCHAR *tchar = "DEVICE";
	UINT br;
	
	if (FR_OK == f_open(&fil, tchar, FA_READ))
	{
		if (FR_OK == f_read(&fil, (void *)&g_device_NO, sizeof(g_device_NO), &br))
		{
			if (FR_OK == f_close(&fil))
			{
				
			}
			else
			{
				return 3;
			}
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}
	
	return 0;
}


/*-----------------------------------------------------------------------*/
/* 测试文件系统                                                                      */
/* 正常返回0                                                                          */
/*-----------------------------------------------------------------------*/
SWORD test_file_system()
{
	FIL fil1, fil2, fil3;
	TCHAR *tchar = "TEST";
	UINT br;
	UINT wr;
	DWORD test_write_to_TFCard = 0x0A1B2C3D;
	DWORD test_read_from_TFCard = 0x00000000;

	if (FR_OK == f_open(&fil1, tchar, FA_CREATE_ALWAYS))
	{
		if (FR_OK == f_close(&fil1))
		{
			
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}
	
	if (FR_OK == f_open(&fil2, tchar, FA_WRITE))
	{
		if (FR_OK == f_write(&fil2, (const void *)&test_write_to_TFCard, sizeof(test_write_to_TFCard), &wr))
		{
			if (FR_OK == f_close(&fil2))
			{
				
			}
			else
			{
				return 5;
			}
		}
		else
		{
			return 4;
		}
	}
	else
	{
		return 3;
	}

	if (FR_OK == f_open(&fil3, tchar, FA_READ))
	{
		if (FR_OK == f_read(&fil3, (void *)&test_read_from_TFCard, sizeof(test_read_from_TFCard), &br))
		{
			if (FR_OK == f_close(&fil3))
			{
				
			}
			else
			{
				return 8;
			}
		}
		else
		{
			return 7;
		}
	}
	else
	{
		return 6;
	}
	
	if (test_write_to_TFCard == test_read_from_TFCard)
	{
		return 0;
	}
	else
	{
		return 7;
	}
}

/*-----------------------------------------------------------------------*/
/* 写入摄像头图像数据到TF卡                                                       */
/* 正常返回0                                                                          */
/*-----------------------------------------------------------------------*/
SWORD write_camera_data_to_TF()
{
	static FIL Cfil1, Cfil2;
	TCHAR *tchar = "CAMERA";
	UINT wr;
	int i=0;
	TF_Image();
	if (FR_OK == f_open(&Cfil1, tchar, FA_CREATE_ALWAYS))
	{
		if (FR_OK == f_close(&Cfil1))
		{
			
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}

	if (FR_OK == f_open(&Cfil2, tchar, FA_WRITE))
	{
		//写入行列
		f_write(&Cfil2, (const void *)&RowCol1, sizeof(RowCol1), &wr);
		f_lseek(&Cfil2,f_size(&Cfil2));
		f_write(&Cfil2, (const void *)&row, sizeof(row), &wr);
		f_lseek(&Cfil2,f_size(&Cfil2));
		f_write(&Cfil2, (const void *)&column, sizeof(column), &wr);
		f_lseek(&Cfil2,f_size(&Cfil2));
		f_write(&Cfil2, (const void *)&RowCol2, sizeof(RowCol2), &wr);
		f_lseek(&Cfil2,f_size(&Cfil2));
		//写入摄像头数据
		f_write(&Cfil2, (const void *)&Camera1, sizeof(Camera1), &wr);
		f_lseek(&Cfil2,f_size(&Cfil2));
		for(i=0;i<ROWS;i++)
		{
			f_write(&Cfil2, (const void *)&(camera[i]), sizeof(camera[i]), &wr);
			f_lseek(&Cfil2,f_size(&Cfil2));
		}
		f_write(&Cfil2, (const void *)&Camera2, sizeof(Camera2), &wr);
			if (FR_OK == f_close(&Cfil2))
			{
				return 0;
			}
			else
			{
				return 5;
			}
		

	}
	else
	{
		return 3;
	}
}

