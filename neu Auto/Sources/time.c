#define __TIME_C_
#include "includes.h"


void init_I2C(void)
{
	I2C.IBFD.R = 0xbf;	//设置时钟
	I2C.IBAD.R = 0xa0;	//从机地址
	I2C.IBCR.B.MDIS = 0;	//使能I2C
	
	SIU.PCR[10].R = 0x0b10;	//SDA PA10
	SIU.PCR[11].R = 0x0b10;	//SCL PA11
}


void disable_I2C(void)
{
	I2C.IBCR.B.MDIS = 1;
}


int I2C_read_byte_from_time_module(BYTE address, BYTE data[], BYTE cnt)
{
	int i;
	volatile DWORD ret;
	
	//START
	I2C.IBCR.B.NOACK = 0;
	for (ret=0; I2C.IBSR.B.IBB; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 1;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 1;
	I2C.IBCR.B.MS = 1;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 2;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 3;
		}
		delay_ms(1);
	}
	I2C.IBDR.R = address;	//写寄存器地址
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 4;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	I2C.IBCR.B.RSTA = 1;	//重启动
	
	
	//写芯片地址 /读
	I2C.IBDR.R = 0xa3;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 5;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 6;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 0;
	I2C.IBDR.R;
	
	
	for (i = 0; i<cnt; i++)
	{
		for (ret=0; !I2C.IBSR.B.IBIF; ret++)
		{
			if (ret >= I2C_WAIT_TIME)
			{
				I2C.IBCR.B.MS = 0;
				return 7;
			}
			delay_ms(1);
		}
		I2C.IBSR.B.IBIF = 1;
		if (i+2 < cnt)
		{
			data[i] = I2C.IBDR.R;
		}
		else if (i+2 == cnt)	//正在读倒数第二个字节
		{
			I2C.IBCR.B.NOACK = 1;
			data[i] = I2C.IBDR.R;
		}
		else	//正在读最后一个字节
		{
			I2C.IBCR.B.MS = 0;
			data[i] = I2C.IBDR.R;
		}
	}
	
	return 0;
}


int I2C_write_byte_to_time_module(BYTE address, BYTE data[], BYTE cnt)
{
	int i;
	volatile DWORD ret;
	
	//START
	I2C.IBCR.B.NOACK = 0;
	for (ret=0; I2C.IBSR.B.IBB; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 1;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 1;
	I2C.IBCR.B.MS = 1;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 2;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 3;
		}
		delay_ms(1);
	}
	I2C.IBDR.R = address;	//写寄存器地址
	
	for (i=0; i<cnt; i++)
	{
		for (ret=0; !I2C.IBSR.B.IBIF; ret++)
		{
			if (ret >= I2C_WAIT_TIME)
			{
				I2C.IBCR.B.MS = 0;
				return 5;
			}
			delay_ms(1);
		}
		I2C.IBSR.B.IBIF = 1;
		if (i<cnt-1)
		{
			if (!I2C.IBSR.B.RXAK)
			{
				I2C.IBDR.R = data[i];
			}
			else
			{
				I2C.IBCR.B.MS = 0;
				return 6;
			}
		}
		else
		{
			I2C.IBCR.B.MS = 0;
		}
	}
	
	return 0;
}


int I2C_write_byte_to_time_module_2(BYTE address, BYTE data[], BYTE cnt)
{
	int i;
	volatile DWORD ret;
	
	//START
	I2C.IBCR.B.NOACK = 0;
	for (ret=0; I2C.IBSR.B.IBB; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 1;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 1;
	I2C.IBCR.B.MS = 1;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 2;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 3;
		}
		delay_ms(1);
	}
	I2C.IBDR.R = address;	//写寄存器地址
	
	for (i=0; ; i++)
	{
		for (ret=0; !I2C.IBSR.B.IBIF; ret++)
		{
			if (ret >= I2C_WAIT_TIME)
			{
				I2C.IBCR.B.MS = 0;
				return 5;
			}
			delay_ms(1);
		}
		I2C.IBSR.B.IBIF = 1;
		if (i<cnt)
		{
			if (!I2C.IBSR.B.RXAK)
			{
				I2C.IBDR.R = data[i];
			}
			else
			{
				I2C.IBCR.B.MS = 0;
				return 6;
			}
		}
		else
		{
			I2C.IBCR.B.MS = 0;
			break;
		}
	}
	
	return 0;
}


int I2C_read_byte_from_time_module_2(BYTE address, BYTE data[], BYTE cnt)
{
	int i, read_one_byte;
	volatile DWORD ret;
	
	//START
	I2C.IBCR.B.NOACK = 0;
	for (ret=0; I2C.IBSR.B.IBB; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 1;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 1;
	I2C.IBCR.B.MS = 1;
	
	//写芯片地址 /写
	I2C.IBDR.R = 0xa2;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 2;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 3;
		}
		delay_ms(1);
	}
	I2C.IBDR.R = address;	//写寄存器地址
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 4;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	I2C.IBCR.B.RSTA = 1;	//重启动
	
	
	//写芯片地址 /读
	I2C.IBDR.R = 0xa3;
	
	
	for (ret=0; !I2C.IBSR.B.IBIF; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 5;
		}
		delay_ms(1);
	}
	I2C.IBSR.B.IBIF = 1;
	for (ret=0; I2C.IBSR.B.RXAK; ret++)
	{
		if (ret >= I2C_WAIT_TIME)
		{
			I2C.IBCR.B.MS = 0;
			return 6;
		}
		delay_ms(1);
	}
	I2C.IBCR.B.TX = 0;
	I2C.IBDR.R;
	
	if (cnt==1)
	{
		cnt = 2;
		read_one_byte = 1;
	}
	else
	{
		read_one_byte = 0;
	}
	for (i = 0; i<cnt; i++)
	{
		for (ret=0; !I2C.IBSR.B.IBIF; ret++)
		{
			if (ret >= I2C_WAIT_TIME)
			{
				I2C.IBCR.B.MS = 0;
				return 7;
			}
			delay_ms(1);
		}
		I2C.IBSR.B.IBIF = 1;
		if (i+2 < cnt)
		{
			data[i] = I2C.IBDR.R;
		}
		else if (i+2 == cnt)	//正在读倒数第二个字节
		{
			I2C.IBCR.B.NOACK = 1;
			data[i] = I2C.IBDR.R;
		}
		else	//正在读最后一个字节
		{
			I2C.IBCR.B.NOACK = 1;
			I2C.IBCR.B.MS = 0;
			if (read_one_byte)
			{
				I2C.IBDR.R;
			}
			else
			{
				data[i] = I2C.IBDR.R;
			}
		}
	}
	
	
	return 0;
}