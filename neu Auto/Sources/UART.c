#define __UART_C_
#include "includes.h"

extern BYTE haha;
extern BYTE rev_ch;
extern WORD helm_use;
extern int16_t motor_use;
extern int direction;

int lenth=0;

BYTE status;

int g_serial_port_0_f = 0;
BYTE g_serial_port_0_data;

int g_serial_port_1_f = 0;
BYTE g_serial_port_1_data;

void rev_remote_frame_44(BYTE rev);
/*-----------------------------------------------------------------------*/
/* 串口0初始化                                                           */
/* 分配给WiFi模块使用                                                    */
/*-----------------------------------------------------------------------*/
void init_serial_port_0(void)
{
	LINFLEX_0.LINCR1.B.INIT=1;  //进入初始化模式
	LINFLEX_0.LINCR1.R=0x00000015; 
	LINFLEX_0.LINIER.B.DRIE=1; //允许接收中断
	/*波特率算法baud=Fperiph_clk/(16*LFDIV)
	DIV_M=LFDIV整数部分
	DIV_F=LFDIV小数部分*16  */ 	
//#ifndef PERIPH_SET_1_CLK_16M	//80M
	
	LINFLEX_0.LINFBRR.B.DIV_F = 6;		//38400:64M-104+3
	LINFLEX_0.LINIBRR.B.DIV_M= 17;  //115200	//波特率设置38400:80M-130+3 57600:80M-86+13 115200:80M-43+6  9600:80M-520+13
   
//#else
//	LINFLEX_0.LINIBRR.B.DIV_M= 17;	//波特率设置 2400:416+11, 9600:104+3, 10417:96+0, 19200:52+1, 57600:17+6
//    LINFLEX_0.LINFBRR.B.DIV_F = 6;	//波特率设置 115200:8+11, 230400:4+5, 460800:2+3, 921600:1+1
//#endif
    LINFLEX_0.UARTCR.B.UART=1;
	LINFLEX_0.UARTCR.R=0x00000033;//8-bit data、UART mode
	LINFLEX_0.LINCR1.B.INIT=0; //退出初始化模式
	
	SIU.PCR[18].R = 0x0400;    /* MPC56xxB: Configure port B2 as LIN0TX */
    SIU.PCR[19].R = 0x0103;    /* MPC56xxB: Configure port B3 as LIN0RX */
  	INTC_InstallINTCInterruptHandler(intc_serial_port_0_RX,79,5); 
}


void serial_port_0_TX(BYTE data)
{
	int i = 0;
	
	LINFLEX_0.BDRL.B.DATA0 = data;	//发送语句
	while(!LINFLEX_0.UARTSR.B.DTF)
	{
		if (i++ >= 10000)
		{
			break;	/* 防止DTF置位失败 */
		}
	}
	LINFLEX_0.UARTSR.B.DTF=1;
}


void serial_port_0_TX_array(const BYTE data[], WORD n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_0_TX(data[i]);
	}
}


void intc_serial_port_0_RX(void)
{
	BYTE rev_ch;
	D3=~D3;
	while(!LINFLEX_0.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_0.BDRM.B.DATA4;
	g_serial_port_0_f = 1;
	g_serial_port_0_data = rev_ch;
	LINFLEX_0.UARTSR.B.DRF=1;      //清空标志位
	rev_remote_frame_2(rev_ch);
}


/*-----------------------------------------------------------------------*/
/* 串口1初始化                                                           */
/* 分配给Bluetooth模块使用                                               */
/*-----------------------------------------------------------------------*/
void init_serial_port_1(void) 
{
	LINFLEX_1.LINCR1.B.INIT=1;	//进入初始化模式
	LINFLEX_1.LINCR1.R=0x00000015;
//#ifndef PERIPH_SET_1_CLK_16M
//	LINFLEX_1.LINIBRR.B.DIV_M= 520; //57600:86&13  9600:520&13
//    LINFLEX_1.LINFBRR.B.DIV_F =13;
//#else
    LINFLEX_1.LINFBRR.B.DIV_F = 1;
    LINFLEX_1.LINIBRR.B.DIV_M= 26; //38400

//#endif

    LINFLEX_1.UARTCR.B.UART=1;
	LINFLEX_1.UARTCR.R=0x00000033;
	
	LINFLEX_1.LINIER.B.DRIE=1;
	
	LINFLEX_1.LINCR1.B.INIT=0;	//退出初始化模式
	
	SIU.PCR[38].R = 0x0400;	/* MPC56xxB: Configure port C6 as LIN1TX */
    SIU.PCR[39].R = 0x0103;	/* MPC56xxB: Configure port C7 as LIN1RX */
    
    INTC_InstallINTCInterruptHandler(intc_serial_port_1_RX, 99, 5);
}

#if 1
void serial_port_1_TX(unsigned char data)
{
	LINFLEX_1.BDRL.B.DATA0=data;	//发送语句
	while(!LINFLEX_1.UARTSR.B.DTF){}	//等待数据发送完成
	LINFLEX_1.UARTSR.B.DTF=1;	//清空标志位
}
#endif

void serial_port_1_TX_array(const BYTE data[], WORD n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_1_TX(data[i]);
	}
}


void intc_serial_port_1_RX()
{
	BYTE rev_ch;
	
	while(!LINFLEX_1.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_1.BDRM.B.DATA4;
	g_serial_port_1_f = 1;
	g_serial_port_1_data = rev_ch;
	LINFLEX_1.UARTSR.B.DRF=1;
	rev_remote_frame_44(rev_ch);
}

void rev_remote_frame_44(BYTE rev)
{
    static int shuzi ;
    static WORD high=0;
    static WORD low=0;
	shuzi=shuzi+1;
	
	if (shuzi==1)
	 {
		 if (rev=='Z')
		{
								 //serial_port_1_TX('a');
		}	
		 else 
	   {	
		   shuzi = 0;
	   }
	} 	
	 else if (shuzi==2)
     {
		if (rev=='Z')
		{
			// serial_port_1_TX('a');
		}
		else 
		{	
			shuzi = 0;
			 
		} 
	 }
			    				
	  else if (shuzi==3)
	{
		direction=rev;
		//serial_port_1_TX('a');
					
	}
	 else if (shuzi==4)
	{
			lenth=rev;
					// serial_port_1_TX('a');
	}
	  else if (shuzi==5)
	{
			high=((WORD)(rev)<<8);
			// serial_port_1_TX('a');
					
	}
	 else if (shuzi==6)
	{
		low=(WORD)(rev);
			if(direction==1)
			{
				helm_use=(high|low);
			}
			 else if (direction==5)
			{
				motor_use=(int16_t)(high|low);
			}
			 else
			 {
				 shuzi = 0;
			 }
					
	
		// serial_port_1_TX('c');
		//set_steer_helm_basement(use);
				
	}
	 else if (shuzi==7)
	{
		shuzi=0;
		haha=1;
	}

        
}



/*-----------------------------------------------------------------------*/
/* 串口2初始化                                                           */
/* 分配给RFID读卡器模块使用                                              */
/*-----------------------------------------------------------------------*/
void init_serial_port_2(void)
{
	LINFLEX_2.LINCR1.B.INIT=1;
	LINFLEX_2.LINCR1.R=0x00000011; 
	LINFLEX_2.LINIER.B.DRIE=1;
#ifndef PERIPH_SET_1_CLK_16M
	LINFLEX_2.LINIBRR.B.DIV_M= 520;
    LINFLEX_2.LINFBRR.B.DIV_F = 83;
#else
	LINFLEX_2.LINFBRR.B.DIV_F = 1;
	LINFLEX_2.LINIBRR.B.DIV_M= 52;

#endif
    LINFLEX_2.UARTCR.B.UART=1;
	LINFLEX_2.UARTCR.R=0x00000033;
	LINFLEX_2.LINCR1.B.INIT=0;
	
	SIU.PCR[40].R = 0x0400;	/* MPC56xxB: Configure port C8 as LIN0TX */
    SIU.PCR[41].R = 0x0103;	/* MPC56xxB: Configure port C9 as LIN0RX */
  	INTC_InstallINTCInterruptHandler(intc_serial_port_2_RX, 119, 5); 
}


void serial_port_2_TX(BYTE data)
{
	LINFLEX_2.BDRL.B.DATA0=data;
	while(!LINFLEX_2.UARTSR.B.DTF){}
	LINFLEX_2.UARTSR.B.DTF=1;
}


void serial_port_2_TX_array(const BYTE data[], WORD n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_2_TX(data[i]);
	}
}


void intc_serial_port_2_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_2.UARTSR.B.DRF){}
	rev_ch=(BYTE)LINFLEX_2.BDRM.B.DATA4;
	LINFLEX_2.UARTSR.B.DRF=1;
	
	rev_RFID_frame(rev_ch);
	
//	EMIOS_0.CH[3].CCR.B.FEN=0;//关摄像头场中断
//	EMIOS_0.CH[7].CCR.B.FEN=0;//关摄像头行中断
}


/*-----------------------------------------------------------------------*/
/* 串口3初始化                                                           */
/* 未分配                                                                */
/*-----------------------------------------------------------------------*/
void init_serial_port_3(void)
{
	LINFLEX_3.LINCR1.B.INIT=1;
	LINFLEX_3.LINCR1.R=0x00000011; 
	LINFLEX_3.LINIER.B.DRIE=1;
#ifndef PERIPH_SET_1_CLK_16M
	LINFLEX_3.LINIBRR.B.DIV_M= 520;
    LINFLEX_3.LINFBRR.B.DIV_F = 83;
#else
    LINFLEX_3.LINFBRR.B.DIV_F = 3;
	LINFLEX_3.LINIBRR.B.DIV_M= 104;
	
#endif
    LINFLEX_3.UARTCR.B.UART=1;
	LINFLEX_3.UARTCR.R=0x00000033;
	LINFLEX_3.LINCR1.B.INIT=0;
	
	SIU.PCR[74].R = 0x0400;	//LINFLEX_3 TX PE10
    SIU.PCR[75].R = 0x0103;	//LINFLEX_3 RX PE11 涉及到引脚复用
  	SIU.PSMI[31].R = 1;
  	INTC_InstallINTCInterruptHandler(intc_serial_port_3_RX, 122, 5); 
}


void serial_port_3_TX(BYTE data)
{
	LINFLEX_3.BDRL.B.DATA0 = data;       //发送语句
	while(!LINFLEX_3.UARTSR.B.DTF){}
	LINFLEX_3.UARTSR.B.DTF = 1;
}


void serial_port_3_TX_array(const BYTE data[], WORD n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_3_TX(data[i]);
	}
}


void intc_serial_port_3_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_3.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_3.BDRM.B.DATA4;
	LINFLEX_3.UARTSR.B.DRF = 1;
}
