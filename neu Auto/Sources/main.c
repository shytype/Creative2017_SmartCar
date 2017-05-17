//本车现在是1号车程序
#include "includes.h"
BYTE rev_ch;
WORD helm_use=0;
int16_t motor_use=0;
int direction;
BYTE haha;
short high2;
short low2;
short high3;
short low3;
short device_number2;
short data_number;
int supersonic_on_off=1;
int biaoji=0;
int jishu=0;
int zhangai=1;
int i;
int j;
extern int bz;
extern int right;
int velocity;
int sum=0;
BYTE video_sender;
extern int Hold_a;
extern int Hold_b;
extern int right_turn;
extern int target_near;
extern get_ss;

void Mode0_DebugCamera(void);
void Mode1_SendVideo(void);
void Mode2_GO(void);
void Mode3_Andriod(void);

void main(void)
	{
	init_all_and_POST();
//	set_steer_helm_basement(3000);
//	set_speed_pwm(300);
//	delay_ms(12000);
//	sending_service_package(0x55,0x0066,0x01F4);
//	delay_ms(2000);
//	sending_service_package(0x55,0x0067,0x01F4);
//	delay_ms(2000);
//	sending_service_package(0x55,0x0068,0x01F4);
//	delay_ms(2000);
//	sending_service_package(0x55,0x0069,0x01F4);
	if(mode==0)
		Mode0_DebugCamera();//图像显示屏显示，车速20，显示offset RoadType，舵机打角，wifi_car_action不激活
	else if(mode==1)
		Mode1_SendVideo();//推车录图像，仅摄像头图像发上位机
	else if(mode==2)
		Mode2_GO();//速度20，WIFI读卡循迹超声全开，图像不显示不发送
	else if(mode==3)
		Mode3_Andriod();//远程模式，上位机遥控车
}
void Mode0_DebugCamera(void)
{
//	if(g_device_NO!=1)
//	{
//		Car_Waitfororder=1; /////////////////////////
//		EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断
//		Start_one();
//	}
//	else
//	{
////		EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断
//		velocity=310;///////////////////////////////////////////
//	}
	for (;;)
	{
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			Wifi_Ctrl();
		}
//		if(!target_near)//在距离小于5的时候不进超声
//		{
//		
//		supersonic();
//		}
//		if(!get_ss)
//		{
			
			control_car_action();			// 全场动作控制
//		}
//		get_ss=0;
//		EMIOS_0.CH[3].CSR.B.FLAG = 1;
//		EMIOS_0.CH[3].CCR.B.FEN=1;
//		if(WIFICHEKER==1 && Car_Psg==1)            // 有一个时间间隔为了 保证在没有收到的时候不会发疯一样发
//		{
//			WIFICHEKER=0;
//			wifi_sender_checker();//每次检查一次是否收到回复  注意：子函数在被设计为发送完一定时间内不会工作，防止对方还没回答这里不停发
//		}
//		if(order_received==1)
//		{
//			order_received=0;
//			generate_remote_frame_2(g_device_NO_Hex, 0x33, 0x0000, 2, (const BYTE *)(&response_data));
//		}
//		if(fieldover==1&&Car_Stop==0&&zhangai==1&&Car_Waitfororder==0)
//		{
//			car_default();
//			fieldover=0; 
//			FindBlackLine();   //寻迹处理                        jqy
//			if(RoadType==88 || RoadType==2)
//				set_speed_pwm(velocity-80); 
//			else
//			    set_speed_pwm(velocity);
//			CenterLineWithVideo();        //摄像头数据处理              jqy     
//	     	Video_Show();                 //显示屏显示                     jqy
//	     	Typejudge();                  //赛道避障类型判断         jqy
//			if(target_offset<0)           //显示偏差值及赛道类型      jqy
//				LCD_write_english_string(96,1,"-");
//			else LCD_write_english_string(96,1,"+");
//			LCD_Write_Num(105,1,ABS(target_offset),2);
//			LCD_Write_Num(105,2,RoadType,2);
//			SteerControl();          //舵机控制              jqy
//			if(Hold_a==1 && Hold_b==1)
//			{
//				set_speed_pwm(0);
//				Car_Waitfororder=1;
//			}
//			EMIOS_0.CH[3].CSR.B.FLAG = 1;
//			EMIOS_0.CH[3].CCR.B.FEN=1;
//		}
//		zhangai_run();                          //避障参数
	}
}

void Mode1_SendVideo(void)
{
	//SetupCCD();	
	g_f_enable_speed_control = 0;
	EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断

	for (;;)
	{

		if(fieldover)
		{
			fieldover=0;
			FindBlackLine();
		//	Send_CCD_Video();
			
			EMIOS_0.CH[3].CSR.B.FLAG = 1;
			EMIOS_0.CH[3].CCR.B.FEN=1;
		}
		
	}

}

void Mode2_GO(void)
{
	//set_steer_helm_basement(3800);
	set_speed_pwm(240);
	EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断

#if 0
	for (;;)
	{
		trigger_supersonic_0();
        get_supersonic_time_0();
	    trigger_supersonic_2();
		get_supersonic_time_2();
#if 0
		if((ABS((WORD)(tmp_time.R))/100)<100)
		{
			trigger_supersonic_2();
			get_supersonic_time_2();
			LCD_Write_Num(96,6,(ABS((WORD)(tmp_time.R))/100),5);
			set_speed_pwm(0);
		}
#endif
		if((ABS((WORD)(tmp_time2.R))/100)<70 && (ABS((WORD)(tmp_time.R))/100)<100)
			jishu++;
		else if((ABS((WORD)(tmp_time2.R))/100)>70 && (ABS((WORD)(tmp_time.R))/100)>100)
			jishu=0;
		if((ABS((WORD)(tmp_time2.R))/100)<270 && (ABS((WORD)(tmp_time.R))/100)<270)
		{
			biaoji++;
			trigger_supersonic_0();
			get_supersonic_time_0();
			trigger_supersonic_2();
			get_supersonic_time_2();
			LCD_Write_Num(96,6,(ABS((WORD)(tmp_time2.R))/100),5);
			LCD_Write_Num(96,5,(ABS((WORD)(tmp_time.R))/100),5);
		}
		else if((ABS((WORD)(tmp_time2.R))/100)>270 && (ABS((WORD)(tmp_time.R))/100)>270)
		{
			biaoji=0;
			trigger_supersonic_0();
			get_supersonic_time_0();
			trigger_supersonic_2();
			get_supersonic_time_2();
			LCD_Write_Num(96,6,(ABS((WORD)(tmp_time2.R))/100),5);
			LCD_Write_Num(96,5,(ABS((WORD)(tmp_time.R))/100),5);
		}

	//	else if((ABS((WORD)(tmp_time.R))/100)<100)
	//	{
	//		set_speed_pwm(0);
	//		delay_ms(3000);
	//	}
		if(biaoji>=300)
		{
		//	trigger_supersonic_0();
		//	get_supersonic_time_0();
		//	trigger_supersonic_2();
		//	get_supersonic_time_2();
		//	LCD_Write_Num(96,6,(ABS((WORD)(tmp_time.R))/100),5);	
			//for (i = 0; i < 500; i++)
			//	{
				//	for (j = 0; j < 9; j++)
				///	{			
						set_steer_helm_basement(4800);
						set_speed_pwm(210);
						delay_ms(1000);
					//	trigger_supersonic_2();
					//	get_supersonic_time_2();
					//	LCD_Write_Num(96,6,(ABS((WORD)(tmp_time.R))/100),5);
					//}
			//	}
			//for (i = 0; i < 500; i++)
				//{
				//	for (j = 0; j < 9; j++)
				//	{			
						set_steer_helm_basement(2700);
						set_speed_pwm(320);
						delay_ms(1300);
					//	trigger_supersonic_2();
					//	get_supersonic_time_2();
					//	LCD_Write_Num(96,6,(ABS((WORD)(tmp_time.R))/100),5);
				//	}
				//}
			set_steer_helm_basement(STEER_HELM_CENTER);
			set_speed_pwm(240);
		//	delay_ms(2000);//////////////
			biaoji=0;
			jishu=0;
			//jishu++;
			//LCD_Write_Num(0,0,(ABS((WORD)(jishu))/100),5);
			//biaoji=0;
		//	set_speed_pwm(0); ////////////////
			//suicide();////////////
		}
		trigger_supersonic_0();
		get_supersonic_time_0();
		trigger_supersonic_2();
		get_supersonic_time_2();
		if(jishu>=100)
		{
			set_speed_pwm(0);
			suicide();
		}
	}
#endif

	for (;;)
	{
	    trigger_supersonic_2();
		get_supersonic_time_2();
		if((ABS((WORD)(tmp_time2.R))/100)<270)
		{
			biaoji++;
			trigger_supersonic_2();
			get_supersonic_time_2();
			LCD_Write_Num(96,6,(ABS((WORD)(tmp_time2.R))/100),5);
		}
		else if((ABS((WORD)(tmp_time2.R))/100)>270)
		{
			biaoji=0;
			trigger_supersonic_2();
			get_supersonic_time_2();
			LCD_Write_Num(96,6,(ABS((WORD)(tmp_time2.R))/100),5);
		}
		if(biaoji>=300)
		{	
			set_steer_helm_basement(4800);
			set_speed_pwm(210);
			delay_ms(1000);
			set_steer_helm_basement(2700);
			set_speed_pwm(210);
			delay_ms(1300);
			set_steer_helm_basement(STEER_HELM_CENTER);
			set_speed_pwm(240);
			biaoji=0;
		}
		EMIOS_0.CH[3].CSR.B.FLAG = 1;
		EMIOS_0.CH[3].CCR.B.FEN=1;
	}

	
#if 0
		if(biaoji==0)
		{
			while((jishu-jia)>0)
			{
				for (i = 0; i < 10; i++)
				{
					for (j = 0; j < 9; j++)
					{			
						set_steer_helm_basement(2802);
						set_speed_pwm(300);
						trigger_supersonic_2();
						get_supersonic_time_2();
						LCD_Write_Num(96,6,(ABS((WORD)(tmp_time.R))/100),5);
					}
				}
				jishu--;
			}
			biaoji=1;
			
		}
#endif
		LCD_Fill(0x00);
for (;;)
{
		//set_speed_pwm(300);
	//	set_steer_helm_basement(3875);
		/* 执行远程命令 */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}

		/* 整车动作控制 */
		//control_car_action();
		//fieldover=1;//尝试
		if(fieldover)
		{
			fieldover=0;
			
			FindBlackLine();
		//	if(g_f_red==1&&g_f_stopline==1)
		//	{
				//set_speed_target(0);
			//	set_speed_pwm(0);
			//	D6=~D6;
			//}
			//else
				set_speed_pwm(300);
				//set_speed_target(20);
			Display_Video();



						if(target_offset<0)
							LCD_write_english_string(96,1,"-");
						else LCD_write_english_string(96,1,"+");
						LCD_Write_Num(105,1,ABS(target_offset),2);
						LCD_Write_Num(105,2,RoadType,2);
			SteerControl();
			
			EMIOS_0.CH[3].CSR.B.FLAG = 1;
			EMIOS_0.CH[3].CCR.B.FEN=1;
		}
}
}


void Mode3_Andriod(void)
{
	for(;;)
    {
		
#if 1
			LCD_PrintoutInt(0, 0, (int)Light_Status);
			
			high2=((WORD)(remote_frame_data[5])<<8);
			low2=(WORD)(remote_frame_data[6]);
			device_number2=(high2|low2);
			LCD_PrintoutInt(0, 4, (int)device_number2);
		
			high3=((WORD)(remote_frame_data[7])<<8);
			low3=(WORD)(remote_frame_data[8]);
			data_number=(high3|low3);
			LCD_PrintoutInt(0, 6, (int)data_number);
			
			
			if ( device_number2==1)
				{
					set_steer_helm_basement(data_number);
				}
			else if (device_number2==5)
				{
					set_speed_pwm(data_number);
				}
	   }
#endif
//		if(sending_test==1)          
//		{
//			sending_test=0;
//			sending_service_package(0x33,0x0001,0x000a);// BYTE 目标地址 WORD 命令字 WORD 数据字
//		}
//		
//		if(WIFICHEKER==1)            // 有一个时间间隔为了 保证在没有收到的时候不会发疯一样发
//		{
//			WIFICHEKER=0;
//			wifi_sender_checker();//每次检查一次是否收到回复  注意：子函数在被设计为发送完一定时间内不会工作，防止对方还没回答这里不停发
//		}
//    }
	
}
