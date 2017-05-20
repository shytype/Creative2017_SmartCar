#define __ACTION_C_
#include "includes.h"
int flag_5_2=0;
int flag_c_2_2=0;
int flag_c_4_1=0;
int Road_No=1;
int flag_c_4_2=0;
byte Road_num=0;
int Car_Stop=0;
int Door_Status=0;
int Door_Stop=0;
int Car_Psg=0;
int Door_Open=0;
int Light_2_Ctrl=0;
int Door_Delay=0;
int Door_Close=0;
int Door_Close_Run=0;
int g_f_red=0;//信号灯标志位
int car_direction=1;//车身绝对方向：1234北东南西
int old_car_direction=1;//车身上一次绝对方向：1234北东南西

//********************2016赛季参数******************************************
extern int velocity;
extern int zhangai;
int bz=-1;//关键标志位  有问题问jqy
int time=1;
extern int jishu;
int right=0;//改变2号车PID参数标志位
int place[4]={0,0,0,0};//不同接客区标志   0：未启用   1：起点   2：终点   3：已使用
int current_place=0;
int used=0;//标志已经过起点
int Leftturn=0;//左转位
int Rightturn=0;//右转位
int Had_turn=0;//转过后不再转
int up=0;
extern int Traffic_Jam;
extern int End;
int Game_over=0;
int Emergency=0;
int Hold_a=0;
int Hold_b=0;
int out=0;
//********************2017赛季参数******************************************
extern int right_turn;
extern int left_turn;
extern int straight_drive;
extern int car_stop;
extern int car_go_back;//倒车参数
extern int speed_change;
extern int car_turn_around;//调头参数
extern int uturn;
extern int target_lost;
extern int target_access;
extern int message_received;
extern int barrier_left_detected;
extern int barrier_right_detected;
extern int stuck;
extern int delay_count;
extern int barrier_offset;
extern short steer_rate;
extern short speed_number;
extern short angle_rate;
int dasspeed=420;//识别时速度原pwm300，李秋键更改
int last1,last2,last3,last4,last5=0;

/*------------------------------*/
/* 车灯控制程序    掉头                                         */  
/*------------------------------*/
void Car_UTurn(void)//U型弯，用于灯在后部范围内
{
	g_f_enable_mag_steer_control=0;
	set_speed_target(0);
	set_steer_helm_basement(3700);
	set_speed_pwm(420);//原pwm300，dby更改
	delay_ms(2000);//原本delayms800，dby更改
	set_steer_helm_basement(data_steer_helm_basement.center);
	set_car_direction(UTURN);
	EMIOS_0.CH[3].CCR.B.FEN=0;
	delay_ms(100);//李秋键增加
	set_speed_pwm(0);//李秋键增加
	
}
/*-----------------------------------------------------------------------*/
/* 设置车身方向        act为左右转、掉头                       */
/*-----------------------------------------------------------------------*/
void set_car_direction(SBYTE act)
{
	return;
}
#if 1
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_1_action(WORD site)
{
	return;
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_2_action(DWORD site)
{
	return;
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_3_action(DWORD site)
{
	return;
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车4                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_4_action(DWORD site)
{
	return;
}
#endif
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_1_action(WORD cmd)
{
	return;
}

/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_2_action(WORD cmd)
{

}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_3_action(WORD cmd)
{
	return;
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车4                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_4_action(WORD cmd)
{
	return;
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* 负责分配                                                              */
/*-----------------------------------------------------------------------*/
void control_car_action(void)
{
	if(WIFI_ADDRESS_CAR_1 == g_device_NO)
	{
//		int delay_count1=0,delay_count2=0;
//		delay_count=0;
//		delay_count2=delay_count1;
//		delay_count1=delay_count;
//		if(delay_count2-delay_count1<5)
//		{
//			stuck++;		
//		}
		//supersonic();
		if(speed_change==1)
		{
			 speed_change=0;
			 velocity=speed_number;
		     if(last1==1)
		     {
		    	 last1=0;
		         right_turn=1;
		     }
		     else if(last2==1)
		     {
		    	 last2=0;
		         straight_drive=1;
		     }
		    	 
		     else if(last3==1)
		     {
		    	 last3=0;
		    	 car_stop=1;
		     }
		     else if(last4==1)
		     {
		    	 last4=0;
		    	 left_turn=1;
		     }
		}
		if(right_turn==1)
        {
        	right_turn=0;
        	last1=1;
        	set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*steer_rate/42+data_steer_helm_basement.center);
        	//set_speed_pwm(dasspeed);
        	set_speed_target(velocity);
        	
        }
        if(straight_drive==1)
        {
        	straight_drive=0;
        	last2=1;
        	set_steer_helm_basement(data_steer_helm_basement.center);
        	//set_speed_pwm(dasspeed);	
        	set_speed_target(velocity);	
        }
        if(stuck>20)//倒车
        {
        	stuck=0;
        	last5=1;
        	set_steer_helm_basement(data_steer_helm_basement.center);
        	set_speed_target(-20);
        	delay_ms(1000);
        }
        if(car_stop==1)
        {
        	car_stop=0;
        	last3=1;
            set_steer_helm_basement(data_steer_helm_basement.center);
            set_speed_target(0);	
        }
        if(car_turn_around==1)//调头
        {
        	car_turn_around=0;
        	Car_UTurn();//函数定义在action.c
        }
        if(left_turn==1)
        {
            left_turn=0;
            last4=1;
            set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*steer_rate/42);
            //set_speed_pwm(dasspeed);
            set_speed_target(velocity);	
        }
        if(uturn==1)
        {
        	uturn=0;
        	Car_UTurn();
        	
        }
//        if(target_access)
//        {
//        	target_access=0;
//        	set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*steer_rate/42+data_steer_helm_basement.center+100);
//        	set_speed_pwm(dasspeed);
//        }
        if(message_received==1)
        {
        	message_received=0;
        	target_lost=0;
        }
        if(target_lost>20)
        {
        	target_lost=0;
        	set_steer_helm_basement(data_steer_helm_basement.center);
        	set_speed_target(0);
        }
        if(barrier_left_detected)
        {
        	barrier_left_detected=0;
        	set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*angle_rate/60+data_steer_helm_basement.center+250);
        	//set_steer_helm_basement(3600);
        	//LCD_Write_Num(105,1,10010,2);
        	//set_speed_pwm(dasspeed);
        	set_speed_target(velocity);
        	delay_ms(50);
        }
        if(barrier_right_detected)
        {
            barrier_right_detected=0;
        	set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*steer_rate/60-250);
        	//set_steer_helm_basement(3000);
        	//LCD_Write_Num(105,1,10086,2);
        	//set_speed_pwm(dasspeed);
        	set_speed_target(velocity);
            delay_ms(50);
        }
        
	}
}
void device_Num_change(void)//把设备号换成16进制（好像没啥用）
{
	if(g_device_NO==1)
		g_device_NO_Hex=0x01;
	if(g_device_NO==2)
		g_device_NO_Hex=0x02;
	if(g_device_NO==3)
		g_device_NO_Hex=0x03;
	if(g_device_NO==4)
		g_device_NO_Hex=0x04;
}
void car_default()//每次开车将宝马所有标志位置为默认状态，防止起冲突
{
	return;
}
void BMW_Taxi()//宝马开关门开车控制
{
	return;
}

void Start_one()
{
	if(g_device_NO==1)    //电压8.7V
	{
//		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
//		{
//			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
//			Wifi_Ctrl();
//		}	
		velocity=16;
		set_speed_target(velocity);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_count=0;
		while(delay_count<1500)
		{
			LCD_Write_Num(105,2,delay_count,4);
		}
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_count=0;
		while(delay_count<5000)
		{
			LCD_Write_Num(105,3,delay_count,4);
		}
	}
	
}
