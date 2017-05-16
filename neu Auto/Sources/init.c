#define __INIT_C_
#include "includes.h"
int Lcounter=0;
int sending_waiter=0;
int WIFICHEKER=0;
int DoorC=0;
int sending_test;
FATFS fatfs1;	/* 会被文件系统引用，不得释放 */
int mode=0;
extern int Game_over;
extern int out;
extern int End;
extern int up;
/*-----------------------------------------------------------------------*/
/* 设置单片机的模式和时钟                                                */
/*-----------------------------------------------------------------------*/
void init_modes_and_clock(void) 
{
    ME.MER.R = 0x0000001D;	/* Enable DRUN, RUN0, SAFE, RESET modes */
	/* 设置sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;	/* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;	/* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;	/* 8 MHz xtal: Set PLL0 to 120 MHz */ 
    ME.RUN[0].R = 0x001F0064;	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟 */
    ME.RUNPC[0].R = 0x00000010;	/* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式 */
  
	/* PCTL[?] 选择需要时钟模块(默认即可，不用设置) */
	//ME.PCTL[32].R = 0x00;	/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
	/* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0;	/* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;	/* Enter RUN0 Mode & Inverted Key */
    
    while (ME.GS.B.S_MTRANS) {}	/* Wait for mode transition to complete 等待模式转换完成 */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode 等待选择RUN0模式 */
  
	/* 开peri0、1、2 */
	/* 外设时钟总线 可用于分频 */
	CGM.SC_DC[0].R = 0x84;	/* LIN */
	CGM.SC_DC[1].R = 0x80;	/* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;	/* eMIOS,CTU,ADC */
}


/*-----------------------------------------------------------------------*/
/* 禁用看门狗                                                            */
/* 据徐博说应该首先运行                                                  */
/*-----------------------------------------------------------------------*/
void disable_watchdog(void)
{
	SWT.SR.R = 0x0000c520;	/* rite keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A;	/* Clear watchdog enable (WEN) */
}


/*-----------------------------------------------------------------------*/
/* 初始化all灯                                                         */
/*-----------------------------------------------------------------------*/
void init_led(void)
{
	//2015第一版载LED
#if 0	
 	SIU.PCR[40].R = 0x0203;	/* PC8  */
  	SIU.PCR[45].R = 0x0203; /* PC13 */
 	SIU.PCR[44].R = 0x0203; /* PC12 */
	SIU.PCR[71].R = 0x0203;	/* PE7  */
#endif

#if 1
	//第二版车灯
 	SIU.PCR[16].R = 0x0203;	/* PB0  */
  	SIU.PCR[17].R = 0x0203; /* PB1 */
 	SIU.PCR[72].R = 0x0203; /* PE8 */
	SIU.PCR[73].R = 0x0203;	/* PE9  */	

	//第二版板载LED
	SIU.PCR[12].R = 0x0203;/* PA12  */
	SIU.PCR[13].R = 0x0203;/* PA13  */
	SIU.PCR[14].R = 0x0203;/* PA14  */
	SIU.PCR[15].R = 0x0203;/* PA15  */
#endif
	D0 = 1;	/* 1=熄灭 */
	D1 = 1;
	D2 = 1;
	D3 = 1;
	D5 = 1;
	D6 = 1;
	D7 = 1;
	D8 = 1;

//车灯全亮
	LeftL = 1;	/* 0=熄灭 */
	RightL = 1;
	StopL = 1;
	RunL = 1;
}


/*-----------------------------------------------------------------------*/
/* 初始化拨码开关                                                         */
/*-----------------------------------------------------------------------*/
void init_DIP(void)
{
	SIU.PCR[53].R=0x0100;/*PD5 as input*/
	SIU.PCR[54].R=0x0100;
	SIU.PCR[55].R=0x0100;
	SIU.PCR[56].R=0x0100;
}
/*-----------------------------------------------------------------------*/
/* 初始化PIT中断                                                         */
/* 10ms                                                                  */
/*-----------------------------------------------------------------------*/
void init_pit(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[1].LDVAL.R = 800000;	/* 800000==10ms */
	PIT.CH[1].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(PitISR,60,1);	/* PIT 1 interrupt vector with priority 1 */
}


/*-----------------------------------------------------------------------*/
/* 初始化eMIOS0                                                          */
/* 初始化电机和舵机                                                      */
/*-----------------------------------------------------------------------*/
void initEMIOS_0MotorAndSteer(void)
{
	/* eMIOS0初始化80MHz分为10MHz */
	EMIOS_0.MCR.B.GPRE= 7;	/* GPRE+1=分频系数；/* Divide 80 MHz sysclk by 7+1 = 8 for 10MHz eMIOS clk */
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
    /* Modulus Up Counter 5kHZ */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 2000;	/* 设置周期200us 5KHZ */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
	 /* 前进输出 OPWMB PE5 输出0-2000 */
	EMIOS_0.CH[18].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[18].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
	EMIOS_0.CH[18].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[18].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[18].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[66].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* 前进输出 OPWMB PE6 输出0-2000 */
	EMIOS_0.CH[20].CCR.B.BSL = 0x1;
	EMIOS_0.CH[20].CCR.B.MODE = 0x60;
	EMIOS_0.CH[20].CCR.B.EDPOL = 1;
	EMIOS_0.CH[20].CADR.R = 0;
	EMIOS_0.CH[20].CBDR.R = 0;
	SIU.PCR[68].R = 0x0600;
    /* 前进输出 OPWMB PE5 输出0-2000 */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[21].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[69].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* 前进输出 OPWMB PE6 输出0-2000 */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;
	EMIOS_0.CH[22].CCR.B.MODE = 0x60;
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;
	EMIOS_0.CH[22].CADR.R = 0;
	EMIOS_0.CH[22].CBDR.R = 0;
	SIU.PCR[70].R = 0x0600;
	
    /* Modulus Up Counter 50HZ */
    EMIOS_0.CH[8].CCR.B.UCPRE=3;	/* Set channel prescaler to divide by 4 */
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 4 */
	EMIOS_0.CH[8].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[8].CADR.R = 50000;	/* 设置周期0.02s  50HZ */
	EMIOS_0.CH[8].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;	/* Use internal counter */
    /* 方向舵机 PWM PA9 输出0-50000 */
	EMIOS_0.CH[9].CCR.B.BSL = 0x1;	/* Use counter bus C (default) */
	EMIOS_0.CH[9].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */  
    EMIOS_0.CH[9].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[9].CADR.R = 1;	/* Leading edge when channel counter bus=250*/
//	EMIOS_0.CH[9].CBDR.R = data_steer_helm_basement.center;	/* Trailing edge when channel counter bus=500*/
	SIU.PCR[9].R = 0x0600;	/* [11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
#if 0
	/* 信号舵机 PWM PA12 输出0-50000 */
	EMIOS_0.CH[12].CCR.B.BSL = 0x1;
	EMIOS_0.CH[12].CCR.B.MODE = 0x60;  
    EMIOS_0.CH[12].CCR.B.EDPOL = 1;
	EMIOS_0.CH[12].CADR.R = 1;
	EMIOS_0.CH[12].CBDR.R = SINGLE_HELM_CENTER;
	SIU.PCR[44].R = 0x0600;
#endif
}

/*----------------------------------------------------------------------*/
/*视频信号场行中断初始                                             	 */
/*----------------------------------------------------------------------*/
void initEMIOS_0Image(void) 
{ 
	//PA3场中断捕捉上升沿及下降沿
	EMIOS_0.CH[3].CCR.B.MODE = 0x02; // Mode is SAIC, continuous 
	EMIOS_0.CH[3].CCR.B.BSL = 0x01; /* Use counter bus B (default) */
	EMIOS_0.CH[3].CCR.B.EDSEL = 1;  //Both edges
//	EMIOS_0.CH[3].CCR.B.EDPOL=1; //Edge Select falling edge
//	EMIOS_0.CH[3].CCR.B.FEN=1;  //interupt enbale
	SIU.PCR[3].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input 
	INTC_InstallINTCInterruptHandler(FieldInputCapture,142,2);  
	
	//PA7行中断捕捉上升沿
	EMIOS_0.CH[7].CCR.B.MODE = 0x02; // Mode is SAIC, continuous 
	EMIOS_0.CH[7].CCR.B.BSL = 0x01; /* Use counter bus B (default) */
	EMIOS_0.CH[7].CCR.B.EDSEL = 0;
	EMIOS_0.CH[7].CCR.B.EDPOL=1; //Edge Select rising edge
//	EMIOS_0.CH[7].CCR.B.FEN=1;  //interupt enbale
	SIU.PCR[7].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input 
	INTC_InstallINTCInterruptHandler(RowInputCapture,144,3); 
	
	//C10口二值化入口
	SIU.PCR[42].R = 0x0102;  // C9口二值化入口
}

/*-----------------------------------------------------------------------*/
/* 拨码开关模式选择                                                */
/*                                                              */
/*-----------------------------------------------------------------------*/
void init_choose_mode(void)
{
	//mode=3;
	//mode=2;//尝试
	mode=switch1*2+switch4;
}

/*-----------------------------------------------------------------------*/
/* 使能外部中断                                                          */
/* 总开关                                                                */
/*-----------------------------------------------------------------------*/
void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;	/* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	/* Enable external interrupts */
}


/*-----------------------------------------------------------------------*/
/* 判断大端                                                              */
/* MPC5604大端 高位在低                                                  */
/*-----------------------------------------------------------------------*/
int is_big_endian()
{
	WORD a = 0x1234;
	BYTE b = *(BYTE *)&a;	/*通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分 */
    
    if(b == 0x12)
	{
    	return 1;
	}
    
	return 0;
}


/*-----------------------------------------------------------------------*/
/* 初始化光电编码器          已测试                                            */
/*-----------------------------------------------------------------------*/
void init_optical_encoder(void)	//PD12模数计数器入口，上升沿
{
	/* 计数部分 PD12 */
	EMIOS_0.CH[24].CCR.B.MODE = 0x51;	/* Mode is MCB */
	EMIOS_0.CH[24].CCR.B.BSL = 0x3;	/* Use internal counter */
	EMIOS_0.CH[24].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[24].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[24].CCR.B.FREN = 0;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[24].CCR.B.EDPOL=1;	/* Edge Select rising edge */
	EMIOS_0.CH[24].CADR.R=0xffff;
	/* (WORD)EMIOS_0.CH[15].CCNTR.R 数据寄存器 */
	SIU.PCR[60].R = 0x0500;	/* Initialize pad for eMIOS channel Initialize pad for input */

	
	/* 方向部分 PC14 */
	SIU.PCR[46].R = 0x0100;
	/* SIU.GPDI[28].B.PDI 数据寄存器 */
}


/*-----------------------------------------------------------------------*/
/* 延时 xus                                                              */
/* 依赖总线80M                                                           */
/*-----------------------------------------------------------------------*/
void delay_us(DWORD us)
{
	volatile int i, j;
	
	for (i = 0; i < us; i++)
	{
		for (j = 0; j < 9; j++) {}
	}
}


/*-----------------------------------------------------------------------*/
/* 延时 xms                                                              */
/* 依赖delay_us()                                                        */
/*-----------------------------------------------------------------------*/
void delay_ms(DWORD ms)
{
	int i;
	
	for (i = 0; i < ms; i++)
	{
		delay_us(1000);
		if(RFID_site_data.is_new_site==1 && Game_over==1)
		{
			RFID_site_data.is_new_site = 0;
			RFID_site_data.roadnum=RFID_Num_Exp(RFID_site_data.site);
			if((RFID_site_data.roadnum>>8)==0x23)
			{
				set_speed_pwm(-500);
				break;
			}
			Game_over=0;
		}
//		if(RFID_site_data.is_new_site && (RFID_site_data.roadnum>>0)==0x1102)//读到红绿灯
//		{
//			set_speed_pwm(0);
//			Car_Stop=1;
//			break;
//		}
		if(out==1)
		{
			if(RFID_site_data.is_new_site==1)//读到出库卡
			{
				RFID_site_data.is_new_site = 0;
				RFID_site_data.roadnum=RFID_Num_Exp(RFID_site_data.site);
				if((RFID_site_data.roadnum>>8)==0x24)
				{
					out=0;
					break;
				}
			}
		}
	}
}


/*-----------------------------------------------------------------------*/
/* 初始化并自检                                                          */
/*-----------------------------------------------------------------------*/
void init_all_and_POST(void)
{
	int i = 0;
	/* TF卡 */
	TCHAR *path = "0:";
	
	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
//	initEMIOS_0Image();/* 摄像头输入中断初始化 */
	
	/* 初始化SPI总线 */
	init_DSPI_1();
//	init_pit();
	init_led();
	init_DIP();
	init_serial_port_1();//Wifi_ouyang
//	init_serial_port_2();//rfid_ouyang
	init_serial_port_0();
	//init_ADC();
	//init_serial_port_3();
	init_supersonic_trigger_0();	
	init_supersonic_receive_0();
//	init_supersonic_receive_1();
//	init_supersonic_trigger_1();		
    init_supersonic_receive_2();
	init_supersonic_trigger_2();
//	init_supersonic_trigger_3();
//	init_supersonic_receive_3();
	init_optical_encoder();

	//init_DSPI_2();
	//init_I2C();
	init_choose_mode();
	init_pit_1s_L();
	
	/* 初始化SPI总线 */
	//init_DSPI_1();
	
	/* 开启外部总中断 */
	enable_irq();

	/* 初始化显示屏 */
	initLCD();

	//LCD_DISPLAY();
	LCD_Fill(0xFF);	/* 亮屏 */
	delay_ms(50);
	LCD_Fill(0x00);	/* 黑屏 */
	delay_ms(50);
	
#if 0	
	/* 初始化TF卡 */

	LCD_P8x16Str(0,0, (BYTE*)"TF..");
	if (!SD_init())
	{
		/* 挂载TF卡文件系统 */
		if (FR_OK == f_mount(&fatfs1, path, 1))
		{
			/* 文件读写测试 */
			if (!test_file_system())
			{
				g_devices_init_status.TFCard_is_OK = 1;
			}
		}
	}
	if (g_devices_init_status.TFCard_is_OK)
	{
		LCD_P8x16Str(0,0, (BYTE*)"TF..OK");
	}
	else
	{
		LCD_P8x16Str(0,0, (BYTE*)"TF..NOK");
		suicide();
	}
	
	/* 读取设备号 */

	LCD_P8x16Str(0, 4, (BYTE*)"DeviceNo=");
	if (!read_device_no_from_TF())
	{
		if (WIFI_ADDRESS_WITHOUT_INIT != g_device_NO)
		{
			LCD_PrintoutInt(72, 4, g_device_NO);
		}
		else
		{
			suicide();
		}
	}
	else
	{
		suicide();
	}
	device_Num_change();
	/* 开启RFID读卡器主动模式 */

	//ouyang
	if (!init_RFID_modul_type())
	{
		g_devices_init_status.RFIDCard_energetic_mode_enable_is_OK = 1;
		LCD_P8x16Str(0, 6, (BYTE*)"RFID..OK");
	}
	else
	{
		g_devices_init_status.RFIDCard_energetic_mode_enable_is_OK = 0;
		LCD_P8x16Str(0, 6, (BYTE*)"RFID..NOK");
		suicide();
	}
	delay_ms(1000);
	/* 换屏 */
	LCD_Fill(0x00);



	
	/* 读取舵机参数 */
	LCD_P8x16Str(0, 0, (BYTE*)"StH.L=");
	if (read_steer_helm_data_from_TF())
	{
		suicide();
	}
	update_steer_helm_basement_to_steer_helm();
	LCD_PrintoutInt(48, 0, data_steer_helm_basement.left_limit);
	set_steer_helm_basement(data_steer_helm_basement.left_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 2, (BYTE*)"StH.R=");
	LCD_PrintoutInt(48, 2, data_steer_helm_basement.right_limit);
	set_steer_helm_basement(data_steer_helm_basement.right_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 4, (BYTE*)"StH.C=");
	LCD_PrintoutInt(48, 4, data_steer_helm_basement.center);
	set_steer_helm_basement(data_steer_helm_basement.center);
#endif
	g_device_NO=1;
	update_steer_helm_basement_to_steer_helm();
	LCD_PrintoutInt(48, 0, data_steer_helm_basement.left_limit);
	set_steer_helm_basement(data_steer_helm_basement.left_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 2, (BYTE*)"StH.R=");
	LCD_PrintoutInt(48, 2, data_steer_helm_basement.right_limit);
	set_steer_helm_basement(data_steer_helm_basement.right_limit);
	delay_ms(500);
	LCD_P8x16Str(0, 4, (BYTE*)"StH.C=");
	LCD_PrintoutInt(48, 4, data_steer_helm_basement.center);
	set_steer_helm_basement(data_steer_helm_basement.center);
	
	/* 读取mode号 */
	LCD_P8x16Str(0, 6, (BYTE*)"MODE=");
	LCD_PrintoutInt(40, 6, mode);
	//set_pos_target();
	delay_ms(1000);


	/* 换屏 */
	LCD_Fill(0x00);
//
//	/* 速度闭环测试 */	
//	g_f_enable_speed_control = 1;
//	LCD_P8x16Str(0, 4, (BYTE*)"S.T=0");
//	set_speed_target(0);
//	delay_ms(2000);
	
	/* 换屏 */
	LCD_Fill(0x00);


}
//

/*-----------------------------------------------------------------------*/
/* 自杀                                                                                  */
/* 在系统初始化出错时一直卡住                                                 */
/* 阻止系统在外设不正确的情况下启动                                        */
/*-----------------------------------------------------------------------*/
void suicide(void)
{
	while (1) { }
}


void init_pit_1s_L(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[1].LDVAL.R = 800000;	/* 800000==10ms */
	PIT.CH[1].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(Pit_1s_L,60,1);	/* PIT 1 interrupt vector with priority 1 */
}
void Pit_1s_L(void)//10ms
{
	static int time_counter;
	time_counter++;
	Lcounter++;
	if(Lcounter==80)
	{
		Lcounter=0;
		WIFICHEKER=1;
	}
	if(Lcounter==10)
	{
		sending_waiter++;    //用来发送完等待一段时间
	}
	if(time_counter==1000)
	{
		time_counter=0;
	    sending_test=1;
	}
	if(Door_Status==1)
	{
		DoorC++;
	}
	if(DoorC==300)
	{
		Door_Status=0;
		Door_Stop=1;
		DoorC=0;
	}
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}






