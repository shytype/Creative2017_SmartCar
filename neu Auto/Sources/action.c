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
	set_steer_helm_basement(3600);
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
	old_car_direction=car_direction;
	if(act==TURNRIGHT)
	{
		if(car_direction!=WEST)
			car_direction+=1;
		else
			car_direction=1;
	}
	else if(act==TURNLEFT)
	{
		if(car_direction!=NORTH)
			car_direction-=1;
		else
			car_direction=4;
	}
	else if(act==UTURN)
	{
		if(car_direction==NORTH||car_direction==EAST)
			car_direction+=2;
		else
			car_direction-=2;
	}
}
#if 1
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车1                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_1_action(WORD site)
{
	if((site>>8)!=0x25 && (site>>8)!=0x23)
		sending_service_package(0x33,0x00CD,site);
	if((site>>8)==0x11)//在红绿灯路口1
	{
		Road_No=1;
		if(Light_Status==0)
		{
			set_speed_pwm(-700);
			delay_ms(70);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>8)==0x12)//在红绿灯路口2
	{
		Road_No=2;
		if(Light_Status==1)
		{
			set_speed_pwm(-700);
			delay_ms(70);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>0)==0x2103 && Traffic_Jam==1)//在桥下路口且拥堵信号为1
	{
		delay_ms(100);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		up=1;
		set_speed_pwm(350);
		delay_ms(1300);
		set_speed_pwm(800);
		velocity=900;
	}
	if((site>>8)==0x21 && (site>>0)!=0x2103)//在左打死路口         并起道路动作切换功能，日后需更改               jqy
	{
		LCD_Fill(0x00);
		bz=-1;
		jishu=0;////////标志位，防止在其他种类道路上行驶时计数累加            jqy
		delay_ms(100);
		set_speed_pwm(320);
	//	delay_ms(100);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(950);
	//	set_steer_helm_basement(data_steer_helm_basement.center);
	//	delay_ms(300);
	//	fieldover=1;
	}
	if((site>>8)==0x22)//在右打死路口                          仅起测试作用，可更改        jqy
	{
	//	LCD_Fill(0x00);
	//	set_speed_pwm(300);
	//	delay_ms(100);
	//	set_steer_helm_basement(data_steer_helm_basement.right_limit);
	//	delay_ms(700);
		bz=0;
		delay_ms(300);
		fieldover=1;
	}
#if 0   //仅停车
	if((site>>8)==0x25 && bz==-1)//停车     1号车停车参数       jqy
	{
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.1+data_steer_helm_basement.center);
		delay_ms(150);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_ms(450);
		set_speed_pwm(0);
	//	Car_Stop=1;
		delay_ms(8000);
		bz=-2;
	//	LCD_Fill(0x00);
	//	fieldover=1;
	//	bz=-1;
	}
#endif
#if 0    //停车和开车配合版本，适用于1号车
	if((site>>8)==0x25 && time==1)//停车     1号车停车参数    
	{
		set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*0.2);
		delay_ms(100);
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.2+data_steer_helm_basement.center);
		delay_ms(100);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_ms(300);
		set_speed_pwm(0);
	//	Car_Stop=1;
		delay_ms(4000);
		fieldover=0;
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.1+data_steer_helm_basement.center);
		set_speed_pwm(-320); 
		time++;
	}
	else if((site>>8)==0x25 && time==2)//停车     1号车停车参数
	{
		set_speed_pwm(1000);
		delay_ms(80);
		set_speed_pwm(0);
		delay_ms(1500);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		set_speed_pwm(320);
		delay_ms(2300);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_ms(500);
		fieldover=1;
		time++;
	}
	else if((site>>8)==0x25 && time==3)//停车     1号车停车参数
	{
		set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*0.96);
		set_speed_pwm(300);
		delay_ms(1700);
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(1700);
		set_speed_pwm(-320);
		delay_ms(1700);
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.center);
		Car_Stop=1;
	}
#endif
#if 0
	else if((site>>8)==0x25 && bz==-2)//停车    2号车停车参数
	{
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(600);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(650);
		set_steer_helm_basement(data_steer_helm_basement.center);
		set_speed_pwm(0);
		delay_ms(8000);
	//	Car_Stop=1;
	//	delay_ms(5000);
	//	LCD_Fill(0x00);
	//	fieldover=1;
		bz=-3;
	}
#endif
#if 1
	if((site>>8)==0x25)
	{
#if 1
		sending_service_package(0x44,0xCCDD,0x0001);
		set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*0.88);
		set_speed_pwm(320);
		delay_ms(1500);
		sending_service_package(0x44,0xCCDD,0x0001);
		set_speed_pwm(-500);
		delay_ms(50);
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(1380);
		sending_service_package(0x44,0xCCDD,0x0001);
		set_speed_pwm(-400);
		Game_over=1;
		delay_ms(700);
		sending_service_package(0x44,0x1234,0x0001);
		delay_ms(700);
		set_speed_pwm(500);
		delay_ms(50);
		set_speed_pwm(0);
		sending_service_package(0x44,0x1234,0x0001);
		set_steer_helm_basement(data_steer_helm_basement.center);
		LCD_Fill(0x00);
		set_speed_pwm(0);
		Car_Waitfororder=1;
		Car_Stop=1;
		delay_ms(1000);
		sending_service_package(0xEE,0x0711,0x0);
		delay_ms(500);
		sending_service_package(0xEE,0x0711,0x0);
		delay_ms(500);
		sending_service_package(0xEE,0x0711,0x0);	
#endif
	}
#endif
#if 0
	if((site>>8)==0x25)//停车    初始版本
	{
		set_speed_pwm(0);
		delay_ms(2000);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		set_speed_pwm(-350);
		delay_ms(1500);
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(1500);
		set_speed_pwm(350);
		delay_ms(1200);
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_ms(800);
		set_speed_pwm(350);
		delay_ms(1000);
		set_speed_pwm(0);
		
		Car_Stop=1;		
		//	LCD_Fill(0x00);
		fieldover=1;
	}
#endif
//	if((site>>8)==0x26)//进入停车场
//	{
//		delay_ms(100);
//		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.29+data_steer_helm_basement.center);
//		delay_ms(600);
//	}
	if((site>>8)==0x31)
	{
		up=1;
		velocity=1100;
		set_speed_pwm(velocity);
		fieldover=1;
	}
	if((site>>8)==0x32)
	{
		delay_ms(400);
		velocity=-200;
		set_speed_pwm(velocity);
	//	fieldover=1;
	}
	if((site>>0)==0x3002)
	{
		up=0;
		set_speed_pwm(-400);
		delay_ms(300);
		set_speed_pwm(100);
		set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*0.8);
		delay_ms(500);
		set_speed_pwm(300);
		delay_ms(500);
		velocity=320;
	}
	if((site>>0)==0x3001)
	{
		delay_ms(300);
		velocity=200;
		set_speed_pwm(velocity);
		fieldover=1;
	}
//	if((site>>0)==0x8009)
//	{
//		bz=1;
//	}
	if((site>>0)==0x1101 || (site>>0)==0x1102)
	{
		bz=-1;
	}
	if((site>>0)==0x8001)
	{
		bz=2;
	}
	if((site>>0)==0x1105)
	{
		bz=-1;
	}
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_2_action(DWORD site)
{
//	if (RFID_CARD_ID_1_3 == site)
//	{
//		set_steer_helm(0);
//	}
#if 0
	if (RFID_CARD_ID_2_4 == site&&RFID_site_data.old_site==RFID_CARD_ID_1_3)
	{
		Road_num=WIFI_ADDRESS_Road_Node_1;
		send_net_cmd(Road_num,WIFI_CMD_ASK_ROAD);//请求节点发送路况信息
	}
#endif
	//rfid_ask_road(0x01, 0xFF, 0xCD, site);//CD=CARD
	//road_stop();
	if((site>>8)!=0x25 && (site>>8)!=0x23)
		sending_service_package(0x33,0x00CD,site);
	if((site>>0)==0x1106)//改回PID算法
	{
		right=0;
		Road_No=1;
		if(Light_Status==1)
		{
			delay_ms(650);
			set_steer_helm_basement(data_steer_helm_basement.right_limit);
			delay_ms(850);
		}
		if(Light_Status==0)
		{
			set_speed_pwm(-600);
			delay_ms(60);
			Rightturn=1;
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	else if((site>>8)==0x11)//在红绿灯路口1
	{
		Road_No=1;
		if(Light_Status==0)
		{
			set_speed_pwm(-600);
			delay_ms(60);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>0)==0x1201)
	{
		right=0;
		Road_No=2;
		if(Light_Status==0)
		{
			set_steer_helm_basement(data_steer_helm_basement.center);
			delay_ms(1300);//1350
			set_steer_helm_basement(data_steer_helm_basement.left_limit);
			delay_ms(800);//900
		}
		if(Light_Status==1)
		{
			set_speed_pwm(-600);
			delay_ms(60);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
			Leftturn=1;
		}
	}	
	else if((site>>8)==0x12)//在红绿灯路口2
	{
		Road_No=2;
		if(Light_Status==1)
		{
			set_speed_pwm(-600);
			delay_ms(60);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>8)==0x21 && (site>>0)!=0x2103)//在左打死路口
	{
		delay_ms(100);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(950);
		fieldover=1;
	}
	if((site>>8)==0x25)//入库
	{
		if(bz==99)
		{
			sending_service_package(0x44,0xCCDD,0x0001);
			set_speed_pwm(225);
			set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*1);
			delay_ms(670);
			sending_service_package(0x44,0xCCDD,0x0001);
			set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.96+data_steer_helm_basement.center);
			Game_over=1;
			delay_ms(600);
			set_steer_helm_basement(data_steer_helm_basement.center);
			set_speed_pwm(0);
			LCD_Fill(0x00);
			sending_service_package(0x44,0x1234,0x0001);
			delay_ms(200);
			sending_service_package(0x44,0x1234,0x0001);
			Car_Waitfororder=1;
			Car_Stop=1;
		}
		else
			jishu=1;
	}
	if((site>>0)==0x3003)//改变PID算法
	{
		right=1;
//		set_steer_helm_basement(data_steer_helm_basement.right_limit);
//		delay_ms(1000);
	}
	if((site>>8)==0x26)//进入停车场
	{
		set_speed_pwm(250);
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.42+data_steer_helm_basement.center);
		delay_ms(1000);
		bz=99;
	}
//	if((site>>0)==0x8007)
//	{
//		while(Emergency==1)
//		{
//			set_speed_pwm(0);
//			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
//			{
//				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
//				Wifi_Ctrl();
//			}
//			if(order_received==1)
//			{
//				order_received=0;
//				generate_remote_frame_2(g_device_NO_Hex, 0x33, 0x0000, 2, (const BYTE *)(&response_data));
//			}
//		}
//		set_speed_pwm(velocity);
//	}
	if((site>>0)==0x8009)
	{
	//	delay_ms(200);
		set_speed_pwm(-500);
		delay_ms(50);
		set_speed_pwm(0);
		delay_ms(2000);
		set_speed_pwm(-320);
		delay_ms(1000);
		set_speed_pwm(0);
		delay_ms(1500);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		set_speed_pwm(320);
		delay_ms(1200);/////
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(1000);
		set_speed_pwm(-320);
		delay_ms(800);/////
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(1000);
		set_speed_pwm(320);
		delay_ms(1000);/////
		bz=5;
	}
}
/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* RFID                                                                  */
/* 车3                                                                   */
/*-----------------------------------------------------------------------*/
void RFID_control_car_3_action(DWORD site)
{
	if((site>>8)!=0x25 && (site>>8)!=0x40 && (site>>8)!=0x23 && (site>>8)!=0x11 && (site>>8)!=0x12 && (site>>0)!=0x8014)
		sending_service_package(0x33,0x00CD,site);
	current_place=0;
	if(place[1]==2)
		current_place=1;
	if(place[2]==2)
		current_place=2;
	if(place[3]==2)
		current_place=3;
	if(place[1]==1)
		current_place=1;
	if(place[2]==1)
		current_place=2;
	if(place[3]==1)
		current_place=3;
	if((site>>0)==0x1105)//在1号路口
	{
		if((current_place==1 || current_place==2))
		{
			sending_service_package(0x33,0x00DC,0x01BB);
			Road_No=1;
			if(Light_Status==0)
			{
				set_speed_pwm(-800);
				delay_ms(80);
				Car_Stop=1;
				LCD_Fill(0x00);
				set_speed_pwm(0);
				Leftturn=1;
			}
		}
		else
		{
			sending_service_package(0x33,0x00DC,0x01AA);
			Road_No=1;
			if(Light_Status==0)
			{
				set_speed_pwm(-800);
				delay_ms(80);
				Car_Stop=1;
				LCD_Fill(0x00);
				set_speed_pwm(0);
			}
		}
	}
	else if((site>>0)==0x1101)//在3号路口
	{
		sending_service_package(0x33,0x00DC,0x03AA);
		Road_No=1;
		if(Light_Status==0)
		{
			set_speed_pwm(-800);
			delay_ms(80);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>0)==0x8014)//在1号路口
	{
		set_speed_pwm(320);
		if((current_place==1 || current_place==2))
		{
			set_steer_helm_basement(data_steer_helm_basement.left_limit);////////
			if(Leftturn==1)
			{
			    delay_ms(1200);
			    Leftturn=0;
			}
			else
				delay_ms(1000);
		}
	}
	if((site>>0)==0x1201)
	{
		Road_No=2;
		if(Light_Status==0)
		{
			sending_service_package(0x33,0x00DC,0x05BB);
			set_steer_helm_basement(data_steer_helm_basement.center);
			delay_ms(1700);//1350
			set_steer_helm_basement(data_steer_helm_basement.left_limit);
			delay_ms(950);//900
		}
		if(Light_Status==1)
		{
			set_speed_pwm(-800);
			delay_ms(80);
			Car_Stop=1;
			LCD_Fill(0x00);
			set_speed_pwm(0);
		}
	}
	if((site>>8)==0x21 && (site>>0)!=0x2103)//在左打死路口
	{
		LCD_Fill(0x00);
	//	bz=5;
		delay_ms(100);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(1000);
	}
	if((site>>8)==0x25)//停车
	{
		if(End==1)
		{
			set_speed_pwm(300);
			set_steer_helm_basement(data_steer_helm_basement.center-50);
			delay_ms(100);
			sending_service_package(0x44,0xCCDD,0x0001);
			Game_over=1;
			delay_ms(350);
			sending_service_package(0x44,0xCCDD,0x0001);
			delay_ms(350);
			set_speed_pwm(0);
			Game_over=0;
			LCD_Fill(0x00);
			sending_service_package(0x44,0x1234,0x0001);
			delay_ms(200);
			sending_service_package(0x44,0x1234,0x0001);
			delay_ms(200);
			suicide();
		}
		else 
			jishu=1;
	}
	if((site>>8)==0x26 && End==1)//回停车场
	{
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.66+data_steer_helm_basement.center);
		sending_service_package(0x44,0xCCDD,0x0001);
		delay_ms(300);
		sending_service_package(0x44,0xCCDD,0x0001);
		delay_ms(300);
	}
//	if((site>>8)==0x22)//在右打死路口
//	{
//		LCD_Fill(0x00);
//		set_steer_helm_basement(data_steer_helm_basement.right_limit);
//		delay_ms(1080);
//		fieldover=1;
//	}
//	if((site>>0)==0x3003)//改变PID算法
//	{
//		set_steer_helm_basement(data_steer_helm_basement.right_limit);
//		delay_ms(1400);
//	}
	
	if((site>>0)==0x4001 && place[1]==1)//住宅区起点
	{
		delay_ms(150);
		sending_service_package(0x44,0x0011,site);
		Car_Stop=1;
		set_speed_pwm(-800);
		delay_ms(50);
		LCD_Fill(0x00);
		Car_Psg=1;
		set_speed_pwm(0);
		place[1]=3;
		used=1;
		delay_ms(450);
		sending_service_package(0x44,0x0011,site);
	}	
	if((site>>0)==0x4002 && place[2]==1)//工作区起点
	{
		delay_ms(150);
		sending_service_package(0x44,0x0011,site);
		Car_Stop=1;
		set_speed_pwm(-800);
		delay_ms(50);
		LCD_Fill(0x00);
		Car_Psg=1;
		set_speed_pwm(0);
		place[2]=3;
		used=1;
		if(place[1]==0)
		    Had_turn=1;
		if(place[1]==2)
		    Had_turn=0;
		delay_ms(450);
		sending_service_package(0x44,0x0011,site);
	}
	if((site>>0)==0x4003 && place[3]==1)//警察局起点
	{
		delay_ms(150);
		sending_service_package(0xEE,0xAAAA,0x0);		
		Car_Stop=1;
		set_speed_pwm(-800);
		delay_ms(50);
		LCD_Fill(0x00);
		Car_Psg=1;
		set_speed_pwm(0);
		delay_ms(100);
		sending_service_package(0xEE,0xAAAA,0x0);
		delay_ms(100);
		sending_service_package(0x44,0x0011,site);
		place[3]=3;
		used=1;
		delay_ms(450);
		sending_service_package(0x44,0x0011,site);
	}
	if(used==1)
	{
		if((site>>0)==0x4001 && place[1]==2)//住宅区终点
		{
			delay_ms(150);
			sending_service_package(0x44,0x0022,site);
			Car_Stop=1;
			set_speed_pwm(-800);
			delay_ms(50);
			LCD_Fill(0x00);
			Car_Psg=1;
			set_speed_pwm(0);
			place[1]=3;
			delay_ms(450);
			sending_service_package(0x44,0x0022,site);
		}	
		if((site>>0)==0x4002 && place[2]==2)//工作区终点
		{
			delay_ms(150);
			sending_service_package(0x44,0x0022,site);
			Car_Stop=1;
			set_speed_pwm(-800);
			delay_ms(50);
			LCD_Fill(0x00);
			Car_Psg=1;
			set_speed_pwm(0);
			place[2]=3;
			delay_ms(450);
			sending_service_package(0x44,0x0022,site);
		}
		if((site>>0)==0x4003 && place[3]==2)//警察局终点
		{
			delay_ms(150);
			sending_service_package(0xEE,0xAAAA,0x0);
			Car_Stop=1;
			set_speed_pwm(-800);
			delay_ms(50);
			LCD_Fill(0x00);
			Car_Psg=1;
			set_speed_pwm(0);
			delay_ms(100);
			sending_service_package(0xEE,0xAAAA,0x0);
			delay_ms(100);
			sending_service_package(0x44,0x0022,site);
			place[3]=3;
			delay_ms(450);
			sending_service_package(0x44,0x0022,site);
		}
	}
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
	if (WIFI_CMD_NET_0_2 == cmd)//有障碍
	{
		Car_UTurn();
		set_speed_target(10);
	}
	if (WIFI_CMD_ROADBLOCK_unKNOW == cmd||WIFI_CMD_ROADBLOCK_NO == cmd)//无障碍
	{

		send_net_cmd(Road_num,WIFI_CMD_ASK_LIGHT);//请求节点发送路灯信息
	}
	if (WIFI_CMD_LIGHT_RED == cmd)//红灯不可通行
	{
		g_f_red=1;

	}
	if (WIFI_CMD_LIGHT_YELLOW == cmd||WIFI_CMD_LIGHT_GREEN==cmd)//绿灯可通行包括停车状态与通行状态
	{
		g_f_red=0;
		set_steer_helm(0);
		set_speed_target(10);
//		EMIOS_0.CH[3].CSR.B.FLAG = 1;//清场中断标志位
//		EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断
	}
	return;
}

/*-----------------------------------------------------------------------*/
/* 整车动作控制                                                          */
/* WiFi                                                                  */
/* 车2                                                                   */
/*-----------------------------------------------------------------------*/
void WiFi_control_car_2_action(WORD cmd)
{
	if (WIFI_CMD_NET_0_2 == cmd)//有障碍
	{
		Car_UTurn();
		set_speed_target(10);
	}
	if (WIFI_CMD_NET_3_3 == cmd)//无障碍
	{
		D1=~D1;
		D2=~D2;
		send_net_cmd(Road_num,WIFI_CMD_ASK_LIGHT);//请求节点发送路灯信息
	}
	if (WIFI_CMD_NET_0_1 == cmd)//红灯不可通行
	{
		g_f_red=1;

	}
	if (WIFI_CMD_NET_2_1 == cmd)//绿灯可通行包括停车状态与通行状态
	{
		g_f_red=0;
		set_steer_helm(0);
		set_speed_target(10);
//		EMIOS_0.CH[3].CSR.B.FLAG = 1;//清场中断标志位
//		EMIOS_0.CH[3].CCR.B.FEN=1;//开场中断
	}

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
		//supersonic();
		if(speed_change==1)
		{
			 speed_change=0;
		     dasspeed=speed_number;
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
        	set_speed_pwm(dasspeed);	
        	
        }
        if(straight_drive==1)
        {
        	straight_drive=0;
        	last2=1;
        	set_steer_helm_basement(data_steer_helm_basement.center);
        	set_speed_pwm(dasspeed);	
        }
//        if(stuck>30)//倒车
//        {
//        	stuck=0;
//        	last5=1;
//        	set_steer_helm_basement(data_steer_helm_basement.center);
//        	set_speed_pwm(-1*dasspeed);
//        	delay_ms(1000);
//        }
        if(car_stop==1)
        {
        	car_stop=0;
        	last3=1;
            set_steer_helm_basement(data_steer_helm_basement.center);
            set_speed_pwm(0);	
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
            set_speed_pwm(dasspeed);	
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
        	set_speed_pwm(0);
        }
        if(barrier_left_detected)
        {
        	barrier_left_detected=0;
        	set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*angle_rate/60+data_steer_helm_basement.center+250);
        	//set_steer_helm_basement(3600);
        	LCD_Write_Num(105,1,10010,2);
        	set_speed_pwm(dasspeed);
        	delay_ms(50);
        }
        if(barrier_right_detected)
        {
            barrier_right_detected=0;
        	set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*steer_rate/60-250);
        	//set_steer_helm_basement(3000);
        	LCD_Write_Num(105,1,10086,2);
        	set_speed_pwm(dasspeed);
            delay_ms(50);
        }
//		if(Car_Stop==1 &&  RoadType!=88 && RoadType!=2)
//		{
//			if(Light_Status==1 && Road_No==1)
//			{
//				if(bz==2)
//				{
//					delay_ms(700);
//				}
//				if(RFID_site_data.roadnum==0x1102)
//				{
//					delay_ms(1700);
//				}					
//				delay_ms(1000);
//				fieldover=1;
//				Car_Stop=0;
//			}
//			if(Light_Status==0 && Road_No==2)
//			{
//				delay_ms(1000);
//				fieldover=1;
//				Car_Stop=0;
//			}
//		}
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
	if(WIFI_ADDRESS_CAR_1 == g_device_NO)
	{
		if(Door_Close_Run)
			Door_Close_Run=0;
		if(Door_Delay)
			Door_Delay=0;
		set_door_pwm(0);
		if(Car_Psg)
			Car_Psg=0;
		if(Door_Open)
			Door_Open=0;
		if(Door_Close)
			Door_Close=0;
		if(Door_Status)
			Door_Status=0;
		if(Door_Stop)
			Door_Stop=0;
		if(DoorC!=0)
			DoorC=0;
	}
}
void BMW_Taxi()//宝马开关门开车控制
{
	if(Door_Open)
	{
		Door_Status=1;	
		set_door_pwm(600);
		Door_Open=0;
	}
	if(Door_Close)
	{
		Door_Status=1;
		set_door_pwm(-600);
		Door_Close=0;
	}
	if(Door_Stop)
	{
		set_door_pwm(0);
		Door_Stop=0;
		if(Door_Delay)
		{
			sending_service_package(0x44,0x1111,0x0000);
			Car_Stop=0;
			Door_Delay=0;
			sending_service_package(0xEE,0xBBBB,0x0);
			delay_ms(100);
			sending_service_package(0xEE,0xBBBB,0x0);
		}
	}
	if(Door_Close_Run)
	{
		Door_Status=1;
		set_door_pwm(-600);
		Door_Close_Run=0;
		Door_Delay=1;
		sending_service_package(0x44,0x1111,0x0000);
	}
}

void zhangai_run()
{
	if(zhangai==0 && bz==1 && g_device_NO==2)          //停车处理参数，适用于2号车          jqy
	{
	//	delay_ms(300);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);   
		delay_ms(480);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(530);
		set_steer_helm_basement(data_steer_helm_basement.center);
		delay_ms(50);
		zhangai=1;
		bz=-1;
	}
#if 1
	else if(zhangai==0 && bz==1)          //超车处理参数，适用于1号车          jqy
		{
	    	set_speed_pwm(320);
		 	delay_ms(300);
			set_steer_helm_basement(data_steer_helm_basement.left_limit);   
			delay_ms(550);
			set_steer_helm_basement(data_steer_helm_basement.right_limit);
			delay_ms(630);
			set_steer_helm_basement(data_steer_helm_basement.center);
			delay_ms(100);
			zhangai=1;
			bz=-1;
			if(Light_Status==0)
			{
				set_speed_pwm(-700);
				delay_ms(70);
				Car_Stop=1;
				LCD_Fill(0x00);
				set_speed_pwm(0);
			}
			else delay_ms(300);
		}
#endif


		if(zhangai==0 && bz==0)          //避障停车处理参数，适用于1号车         jqy
		{
		//	set_steer_helm_basement(data_steer_helm_basement.center);
			delay_ms(300);
			set_speed_pwm(0);
			jishu=0;
			delay_ms(1000);
			Car_Stop=1;
			zhangai=1;
			bz=-1;
		}
#if 1		
		while(zhangai==0)                //前车未离开，本车不能前进            jqy
		{
			set_speed_pwm(0);
		    FindBlackLine(); 
	//		supersonic();
		    if(RoadType!=12 && RoadType!=13)
		    	if(RoadType!=66)
		    		jishu++;
		    LCD_Write_Num(105,4,jishu,2);
		    EMIOS_0.CH[3].CSR.B.FLAG = 1;
		    EMIOS_0.CH[3].CCR.B.FEN=1;
		    if(jishu>=50)
		    {
		    	zhangai=1;
		    	jishu=0;
		    	bz=-2;               ///////////////////////
		    	delay_ms(2000);
		    }
		}
#endif
}

void Start_one()
{
//	if(g_device_NO==1 && Car_Waitfororder==1)    //电压8.7V
//	{
//		while(Car_Waitfororder==1)
//		{
//			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
//			{
//				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
//				Wifi_Ctrl();
//			}
//			if(order_received==1)
//			{
//				order_received=0;
//				generate_remote_frame_2(g_device_NO_Hex, 0x33, 0x0000, 2, (const BYTE *)(&response_data));
//			}
//		}
//		sending_service_package(0x33,0x00DC,0xEEEE);
//		delay_ms(200);
//		sending_service_package(0x44,0xDDCC,0x0002);
//		velocity=320;
//		set_speed_pwm(velocity);
//		set_steer_helm_basement(data_steer_helm_basement.right_limit);
//		delay_ms(800);
//		sending_service_package(0x44,0xDDCC,0x0002);
//		delay_ms(1000);
//		sending_service_package(0x44,0xAABB,0x0001);
//	}
	if(g_device_NO==2 && Car_Waitfororder==1) 
	{
		while(Car_Waitfororder==1)
		{
			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
			{
				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
				Wifi_Ctrl();
			}
		}
		set_speed_pwm(270);
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.72+data_steer_helm_basement.center);
		sending_service_package(0x44,0xDDCC,0x0002);
		delay_ms(500);
		sending_service_package(0x44,0xDDCC,0x0002);
		delay_ms(500);
		sending_service_package(0x44,0xDDCC,0x0002);
		delay_ms(800);
		sending_service_package(0x44,0xDDCC,0x0002);
		delay_ms(500);
		sending_service_package(0x44,0x1234,0x0001);
		delay_ms(500);
		sending_service_package(0x44,0x1234,0x0001);
		velocity=280;
		jishu=0;
	}
	if(g_device_NO==3 && Car_Waitfororder==1) 
	{
#if 1
		while(Car_Waitfororder==1)
		{
			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
			{
				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
				Wifi_Ctrl();
			}
		}
		set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*0.03+data_steer_helm_basement.center);
		set_speed_pwm(-300); //-350
		while(jishu==0)
		{
			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
			{
				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
				Wifi_Ctrl();
			}
			control_car_action();
			if(jishu==1)
			{
				sending_service_package(0x44,0xDDCC,0x0002);
				set_speed_pwm(1000);
				delay_ms(80);
				set_speed_pwm(0);
				delay_ms(500);
				sending_service_package(0x44,0xDDCC,0x0002);
				delay_ms(500);
				sending_service_package(0x44,0xDDCC,0x0002);
				delay_ms(500);
				sending_service_package(0x44,0xDDCC,0x0002);
				set_steer_helm_basement(data_steer_helm_basement.left_limit);
				set_speed_pwm(420);
				delay_ms(150);
				sending_service_package(0x44,0x1234,0x0001);
				delay_ms(150);
				sending_service_package(0x44,0x1234,0x0001);
				delay_ms(150);
				sending_service_package(0x44,0x1234,0x0001);
				delay_ms(150);
				sending_service_package(0x44,0x1234,0x0001);
				out=1;
				delay_ms(4000);
				set_steer_helm_basement(data_steer_helm_basement.right_limit);
				delay_ms(520);
				fieldover=1;
				velocity=320;
			}
		}
#endif
	}
	jishu=0;
}
