#define __MAG_C_
#include "includes.h"


WORD mag_left=0;
WORD mag_right=0;
WORD mag_right_old=0;
WORD mag_left_old=0;
WORD ab_s=0;


/*-----------------------------------------------------------------------*/
/* 初始化AD                                                              */
/*-----------------------------------------------------------------------*/
void init_ADC(void)
{
	ADC.MCR.R = 0x20000100;     //Conversion times for 80MHz ADClock  连续模式
	ADC.NCMR[1].R = 0x000000FF; //Select ANS0 ANS1 ANS2 ANS3 inputs for conversion 
								//channel 32~39 sampling enable
	ADC.CTR[1].R = 0x00008606;  //Conversion times for 32MHz ADClock？？ 
	ADC.MCR.B.NSTART=1;       //Trigger normal conversions for ADC0
	SIU.PCR[24].R = 0x2100;     //MPC56xxB: Initialize PB[8] as ANS0 CDR32
	SIU.PCR[25].R = 0x2100;     //MPC56xxB: Initialize PB[9] as ANS1 CDR33
	SIU.PCR[26].R = 0x2100;     //MPC56xxB: Initialize PB[10] as ANS2 34
	SIU.PCR[27].R = 0x2100;     //MPC56xxB: Initialize PB[11] as ANS3 35
	SIU.PCR[60].R = 0x2100;     //MPC56xxB: Initialize PD[12] as ANS4 36
	SIU.PCR[61].R = 0x2100;     //MPC56xxB: Initialize PD[13] as ANS5 37
	SIU.PCR[62].R = 0x2100;     //MPC56xxB: Initialize PD[14] as ANS6 38
	SIU.PCR[63].R = 0x2100;     //MPC56xxB: Initialize PD[15] as ANS7 39
}


/*-----------------------------------------------------------------------*/
/* 电磁读值函数                                                          */
/*-----------------------------------------------------------------------*/
void mag_read(void)
{
	mag_right_old=mag_right;
	mag_right=(WORD)ADC.CDR[33].B.CDATA;//PB9
	mag_left_old=mag_left;
	mag_left=(WORD)ADC.CDR[32].B.CDATA;//PB8
}


/*-----------------------------------------------------------------------*/
/* 判断是否在磁线上                                                      */
/*-----------------------------------------------------------------------*/
int is_on_mag_line(void)
{
	mag_read();
	if (mag_left != 0 && mag_right != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*-----------------------------------------------------------------------*/
/* 电磁发送函数                                                          */
/*-----------------------------------------------------------------------*/
void mag_TX(void)
{
	BYTE a,b,m,n,p,q,s,t;
	WORD c = 0x0000;
	WORD d = 0x0000;
	WORD e = 0x0000;
	WORD f = 0x0000;
	
	c=0x03ff & mag_left_old;
	a=(BYTE)c;
	b=(BYTE)(c>>8);
	serial_port_1_TX(b);
	serial_port_1_TX(a);
	
	d=0x03ff & mag_left;
	m=(BYTE)d;
	n=(BYTE)(d>>8);
	serial_port_1_TX(n);
	serial_port_1_TX(m);
	
	e=0x03ff & mag_right_old;
	p=(BYTE)d;
	q=(BYTE)(d>>8);
	serial_port_1_TX(p);
	serial_port_1_TX(q);
	
	f=0x03ff & mag_right;
	s=(BYTE)d;
	t=(BYTE)(d>>8);
	serial_port_1_TX(s);
	serial_port_1_TX(t);
	
}


/*-----------------------------------------------------------------------*/
/* 舵机参数控制                                                          */
/*-----------------------------------------------------------------------*/
void control_steer_helm(void)
{	
	int error = 0,error_count = 0;
	int kp=2,kd=1;
	int pos=0;
       static	int steer_rate = 0;
	static int last_error=0;
	static int steer_pwm =0;	/* 由全局变量改为局部静态变量 */
	
	/* 过飞桥 */
	if(g_f_enable_fly_bridge)
	{
		kp=5;
		kd=1;
		if(rad.y<-15)
		{
			kp = 2;
			kd = 4;
		}
	}
	/* 过钢丝桥 */
	if(g_f_enable_steer_bridge)
	{
		kp=2;
		kd=2;
	}
	/* 过单边桥 */
	if(g_f_enable_single_bridge_control)
	{
		if(rev.y>0)
		{
			kp = 1;
			kd = 2;
		}
	}
	pos = mag_left - mag_right;
	error = -pos;
	if(abs(error)>20)
	{
	error_count = (error-last_error);
	steer_rate = (kp*(error)+kd*error_count);
	/*
	LCD_PrintoutInt(0, 0,(steer_rate));
	LCD_PrintoutInt(0, 2,(mag_right));
	LCD_PrintoutInt(0, 4,(mag_left));
	*/
	last_error = error;
	if(mag_left<=30)steer_rate=data_steer_helm.right_limit;
	if(mag_right<=30)steer_rate=data_steer_helm.left_limit;
	steer_pwm = steer_rate;
	}
	else
	{
		steer_pwm = 0;
	}
	
	/* 过单边桥 */
	if(g_f_enable_single_bridge_control)
	{
		steer_pwm=steer_rate+rad.x;
		if(rad.x>=230&&steer_pwm<-50)
		{
			steer_pwm = -50;
		}
		if(rev.y>5)
		{
			if(steer_pwm>100)steer_pwm = 100;
			else if(steer_pwm<-100)steer_pwm = -100;
		}
	}
		/* 过飞桥 */
	if(g_f_enable_fly_bridge)
	{
		
		if(g_f_enable_fly_bridge==2)
		{
			if(steer_pwm>100)steer_pwm = 100;
			else if(steer_pwm<-100)steer_pwm = -100;
		}
	}
	if(g_f_big_U)
	{
		if(rad.z>20&&steer_pwm<0)
		steer_pwm=data_steer_helm.left_limit;		
	}
	if(g_f_big_U_2)
	{
		if(rad.z<-20&&steer_pwm>0)
		steer_pwm=data_steer_helm.right_limit;		
	}
	set_steer_helm((WORD)(steer_pwm));	/* 躲警告 */
}
