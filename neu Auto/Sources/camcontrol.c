#include "includes.h"

//*****************************************************************************************************************
//************************************************控制参数************************************************    	  *
//*****************************************************************************************************************

//**********************舵机参数**********************************************
signed long target_offset=0,last_offset=0;	//舵机偏差值记录
double Steer_kp=0,Steer_kd=0;//舵机P、D值
WORD Steer_PWM[4]={0,0,0,0};//舵机输出值记录


//**********************电机参数**********************************************
word speedcounter1=0,speedcounter2=0;
signed int currentspeed=0,targetspeed=0;		//当前速度、目标速度
signed int Error=0,PreError=0,SumError=0;

double Speed_kp=0,Speed_ki=0,Speed_kd=0;	//电机PID值
signed int Motor_PWM=0;				//电机底层控制量

//********************辅助调试参数******************************************
unsigned int Counter_Error=0;		//光编接触不牢靠错误计数量
////********************起始线停车参数******************************************
byte stop_flag=0;
byte stop_delay=0;

//********************2016赛季参数******************************************
extern int velocity;
extern int right;
//*****************************************************************************************************************
//************************************************角度控制************************************************    	  *
//*****************************************************************************************************************
void SteerControl()
{
	//*1***********出错图像角度控制,输出为前三次平均值**************
	if(RoadType==NoLine||RoadType==Wrong) {
		Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
		set_steer_helm_basement(Steer_PWM[3]);
		//set_steer_helm(Steer_PWM[3]);
		//存舵机值
		Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
		return;
	}
	//最低速
//	if(Slope==1)					{Steer_kp=10;Steer_kd=5;}
//	else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}

	
if(g_device_NO==3 && right==0)         //3号车PID参数       jqy
{
	if(ABS(target_offset)<6) 	{Steer_kp=8;Steer_kd=5;}
	else if(ABS(target_offset)<26)  {Steer_kp=7.2+target_offset*target_offset/100;Steer_kd=10;}
	else {Steer_kp=10.8+target_offset*target_offset/500;Steer_kd=5;}
}
if(g_device_NO==3 && right==1)         //3号车PID参数   转弯    jqy
{
	if(ABS(target_offset)<5) 	{Steer_kp=20;Steer_kd=5;}
    else if(ABS(target_offset)<9) 	{Steer_kp=30;Steer_kd=5;}
	else if(ABS(target_offset)<14) 	{Steer_kp=42;Steer_kd=5;}
	else if(ABS(target_offset)<26)  {Steer_kp=58.2+target_offset*target_offset/100;Steer_kd=5;}
	else {Steer_kp=36.8+target_offset*target_offset/500;Steer_kd=5;}
}
#if 1
if(g_device_NO==2 && right==1)         //2号车PID参数   转弯    jqy
{
	if(ABS(target_offset)<5) 	{Steer_kp=14;Steer_kd=5;}
    else if(ABS(target_offset)<9) 	{Steer_kp=24;Steer_kd=5;}
	else if(ABS(target_offset)<14) 	{Steer_kp=32;Steer_kd=5;}
	else if(ABS(target_offset)<26)  {Steer_kp=38.2+target_offset*target_offset/100;Steer_kd=5;}
	else {Steer_kp=26.8+target_offset*target_offset/500;Steer_kd=5;}
}

if(g_device_NO==2 && right==0)         //2号车PID参数       jqy
{
	if(ABS(target_offset)<6) 	{Steer_kp=17;Steer_kd=5;}//25
	else if(ABS(target_offset)<26)  {Steer_kp=21+target_offset*target_offset/100;Steer_kd=10;}//30
	else {Steer_kp=26+target_offset*target_offset/500;Steer_kd=5;}//34
}
#endif

if(g_device_NO==1)                   //1号车PID参数       jqy
{
	if(ABS(target_offset)<6) 	{Steer_kp=6;Steer_kd=5;}
	else if(ABS(target_offset)<26)  {Steer_kp=15.8+target_offset*target_offset/100;Steer_kd=10;}
	else {Steer_kp=16.2+target_offset*target_offset/500;Steer_kd=5;}
}

#if 0
	if(ABS(target_offset)<5) 		{Steer_kp=5;Steer_kd=5;}
			else if(ABS(target_offset)<10)  {Steer_kp=5;Steer_kd=5;}
			else if(ABS(target_offset)<20)  {Steer_kp=10;Steer_kd=5;}
			else if(ABS(target_offset)<30)  {Steer_kp=10;Steer_kd=5;}
			else if(ABS(target_offset)<40)  {Steer_kp=12;Steer_kd=5;}
			else							{Steer_kp=12;Steer_kd=5;}
#endif
	Steer_PWM[3]=data_steer_helm_basement.center+Steer_kp*target_offset+Steer_kd*(target_offset-last_offset);//位置式PD
	//感觉不太靠谱，调的不好
	
	//舵机限值+舵机输出
	set_speed_pwm(velocity);
	set_steer_helm_basement(Steer_PWM[3]);
	LCD_Write_Num(105,5,(int)Steer_PWM[3],4);
	//存舵机值和offset值
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}



//*****************************************************************************************************************
//************************************************速度控制************************************************    	  *
//*****************************************************************************************************************
void SpeedControl()
{
	//1*******************************起始线停车速度控制及光编线接触不牢控制***********************
//	if(StartLine){
//    	stop_delay++;
//
//    		if(stop_delay==43){
//		    	if(stop_flag==2) set_speed_pwm(0);
//		    	else if(data_encoder.is_forward) set_speed_pwm(-300);
//		    	else stop_flag++;
//		    	stop_delay--;
//		      	return;
//		    }
//	}
   	if(Counter_Error>50) {set_speed_pwm(0);return;}
   	
   	StartTime2s++;
	//2*****正常速度控制...速度分配还要结合最远行和offset


   //最低速
	if(Slope==1)				{data_speed_settings.speed_target=180;
									data_speed_pid.p=6.5;data_speed_pid.i=0.1;data_speed_pid.d=0.2;}
	else if(Slope==2)			{data_speed_settings.speed_target=140;
									data_speed_pid.p=6;data_speed_pid.i=0.1;data_speed_pid.d=0.2;}
									
	else if(RoadEnd<15)			{data_speed_settings.speed_target=175;
									data_speed_pid.p=5.5;data_speed_pid.i=0.1;data_speed_pid.d=0.2;}
	else if(RoadEnd<30)			{data_speed_settings.speed_target=155-target_offset*target_offset/40;
									data_speed_pid.p=5.5;data_speed_pid.i=0.2;data_speed_pid.d=0.2;}
	else						{data_speed_settings.speed_target=130;
									data_speed_pid.p=5.5;data_speed_pid.i=0.2;data_speed_pid.d=0.2;}


	if(StartTime2s<290)	{if(data_encoder.speed_now>data_speed_settings.speed_target) SumError=0;}
	else {StartTime2s--;}
}
