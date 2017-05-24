#define __WIFI_C_
#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_all=0;
int have_responsed;
int order_received; 
WORD response_data=0x0;
int Car_Waitfororder=0;
int Light_Status=0;//默认红灯
int Traffic_Jam=0;//默认畅通
int End=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
BYTE g_device_NO = WIFI_ADDRESS_CAR_1;
BYTE g_device_NO_Hex;/* 设备号 即WiFi地址 */
BYTE des;
WORD cmd_WIFI;
WORD sending_data;
BYTE waiting_for_response=0;
int  lost_data=0;
extern int place[4];
extern int used;
extern int Emergency;
extern int Hold_a;
extern int bz;
//********************2017赛季参数******************************************
//********************dby 修改*********************************************
int right_turn=0;
int left_turn=0;
int straight_drive=0;
int car_stop=0;
int car_go_back=0;
int speed_change=0;
int car_turn_around=0;
int uturn=0;
int target_lost=0;
int message_received=0;
int target_access=0;
int target_near=0;
int barrier_left_detected=0;
int barrier_right_detected=0;
int stuck=0;
int barrier_offset=0;
int light_offset=0;
int yanshi=0;
extern get_ss;
short high8;
short low8;
short high9;
short low9;
short steer_rate=0;
short angle_rate=0;
short speed_number=0;
/*-----------------------------------------------------------------------*/
/* 执行远程命令                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	WORD cmd = 0;
	
	cmd = ((WORD)(data[0])<<8) | ((WORD)(data[1]));
	switch (cmd)
	{
		/* 舵机调参 */
		case WIFI_CMD_SET_HELM_TARGET :
		set_steer_helm_basement(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_HELM_KP :
		break;
		case WIFI_CMD_SET_HELM_KI :
		break;
		case WIFI_CMD_SET_HELM_KD :
		break;
		case WIFI_CMD_SET_STEER_HELM_CENTER :
		set_steer_helm_basement_center(*((WORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_STEER_HELM_LEFT :
		set_steer_helm_basement_left_limit(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_STEER_HELM_RIGHT :
		set_steer_helm_basement_right_limit(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_WRITE_STEER_HELM_DATA_TO_TF :
		if (!update_steer_helm_basement_to_steer_helm())
		{
//			SWORD tmp_read_rad_xyz = (SWORD)read_rad_xyz;	/* 暂存陀螺仪读取标志位 */
//			
//			read_rad_xyz = 0;	/* 暂存禁用陀螺仪读取，防止干扰TF卡读写 */
			write_steer_helm_data_to_TF();	/* 大约60ms */
//			read_rad_xyz = tmp_read_rad_xyz;	/* 恢复陀螺仪读取标志位 */
		}
		else
		{
			//舵机数据不合理
		}
		break;
		case WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF :
		generate_remote_frame_2(g_device_NO, ANDROID_ADDRESS, WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF, sizeof(data_steer_helm_basement), (const BYTE *)&data_steer_helm_basement);
		break;
		
		
		/* 电机调参 */
		case WIFI_CMD_SET_MOTOR_TARGET :
		set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_STOP_SPEED :
		set_speed_target((SWORD)0);
		break;
		case WIFI_CMD_SET_MOTOR_KP :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_MOTOR_KI :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case WIFI_CMD_SET_MOTOR_KD :
		set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
	
		case WIFI_CMD_GET_SEEED_NOW :
		g_remote_control_flags.send_seppd_now = 1;
		break;
		case WIFI_CMD_UNGET_SPEED_NOW :
		g_remote_control_flags.send_seppd_now = 0;
		break;
		
		case WIFI_CMD_NET :
		//D0=~D0;//测试用
		g_net_control_data.is_new_cmd = 1;
		g_net_control_data.cmd = *((WORD *)(&(data[2])));
		break;
		/*开始全场工作*/
		case WIFI_CMD_START_ACTIVE:
		g_start_all=1;
//		g_f_red=1;//测试用
		break;
	}
}
int change_hex_into_dec(BYTE rev)
{
	int o=rev;
	if(rev>0x7F)
	{
		o=o-256;
		
	}
	return o;
}
/*-----------------------------------------------------------------------*/
/* 接受远程数据帧                                                        */
/* 第二版                                                                */
/* 修改地址位定义:源地址(1B)+目的地址(1B)                                */
/* AA BB 源地址(1B) 目的地址(1B) 长度(1B) 数据体 校验字(1B)              */
/* 未改变原数据帧的大部分定义及长度                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	if (g_remote_frame_cnt == 0)	//接收帧头
	{
		if (rev == 0xAA)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xAA;
		}
	}
	else if (g_remote_frame_cnt == 1)	//接收帧头
	{
		if (rev == 0xBB)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xBB;
		}
		else
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt == 2)	//接收源地址
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//接收目的地址
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev != g_device_NO && rev != 0xFF && rev != 0xEE)
		{
			g_remote_frame_cnt = 0;	//不是发给本机的
		}
	}
	else if (g_remote_frame_cnt == 4)	//接收长度
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev+6>REMOTE_FRAME_LENGTH)	//判断是否会导致缓冲区溢出
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>4 && g_remote_frame_cnt<=remote_frame_data[4]+4)	//接收数据区
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[4]+4+1)	//接收校验字节	
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data+2), (WORD)(remote_frame_data[4]+3));
		if (sum != remote_frame_data[remote_frame_data[4]+5])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	return g_remote_frame_state;
}


/*-----------------------------------------------------------------------*/
/* 生成并发送远程命令                                                    */
/* 第二版                                                                */
/* 修改地址位定义:源地址(1B)+目的地址(1B)                                */
/* AA BB 源地址(1B) 目的地址(1B) 长度(1B) 数据体 校验字(1B)              */
/* 未改变原数据帧的大部分定义及长度                                      */
/* 参数 : scr 源地址                                                     */
/*        des 目标地址                                                   */
/*        cmd WiFi命令字                                                 */
/*        length data长度                                                */
/*        data发出的数据体，接在cmd后                                    */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE scr, BYTE des, WORD cmd, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	remote_frame_data_send[i++] = 0xAA;
	remote_frame_data_send[i++] = 0xBB;
	remote_frame_data_send[i++] = scr;
	remote_frame_data_send[i++] = des;
	remote_frame_data_send[i++] = length+2;
	remote_frame_data_send[i++] = (BYTE)(cmd>>8);
	remote_frame_data_send[i++] = (BYTE)cmd;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i++] = check_sum(remote_frame_data_send+2, i-2);
	for (; i < REMOTE_FRAME_LENGTH; i++)	/* 清空未使用区域 */
	{
		remote_frame_data_send[i] = 0x00;
	}
	serial_port_0_TX_array(remote_frame_data_send, REMOTE_FRAME_LENGTH);
}


/*-----------------------------------------------------------------------*/
/* 发送赛场网络控制命令                                                          */
/* 依赖generate_remote_frame_2()                                          */
/*-----------------------------------------------------------------------*/
void send_net_cmd(BYTE des, WORD net_cmd)
{
	generate_remote_frame_2(g_device_NO, des, WIFI_CMD_NET, sizeof(net_cmd), (const BYTE *)(&net_cmd));
}


/*-----------------------------------------------------------------------*/
/* 异或校验                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, WORD length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}


/*-----------------------------------------------------------------------*/
/* 报告在线                                                                            */
/*-----------------------------------------------------------------------*/
void report_online(void)
{
	WORD online = WIFI_CMD_NET_ONLINE;
	
	if (200 < diff_time_basis_PIT(g_time_basis_PIT, g_net_control_data.last_report_online_time))	/* 2秒报告一次 */
	{
		generate_remote_frame_2(g_device_NO, WIFI_ADDRESS_BROADCAST, WIFI_CMD_NET, sizeof(online), (const BYTE *)(&online));
		g_net_control_data.last_report_online_time = g_time_basis_PIT;
	}
	
}


/*-----------------------------------------------------------------------*/
/* 发送远程请求的数据                                                             */
/*-----------------------------------------------------------------------*/
void send_remote_request_data(void)
{
	/* 发送当前速度 */
	if (g_remote_control_flags.send_seppd_now)
	{
		SWORD speed_now_tmp;
		
		if (data_encoder.is_forward)
		{
			speed_now_tmp = (SWORD)(data_encoder.speed_now);
		}
		else
		{
			speed_now_tmp = (SWORD)0 - (SWORD)(data_encoder.speed_now);
		}
		generate_remote_frame_2(g_device_NO, WIFI_ADDRESS_ANDRIUD_ZHOU, WIFI_CMD_GET_SEEED_NOW, sizeof(speed_now_tmp), (const BYTE *)&speed_now_tmp);
	}
	
	/* 发送陀螺仪积分值 */
	/*
	if (g_remote_control_flags.send_gyro_integral)
	{
		generate_remote_frame_2(g_device_NO, WIFI_ADDRESS_ANDRIUD_ZHOU, WIFI_CMD_GET_GYRO_INTEGRAL_DATA, sizeof(rad), (const BYTE *)&rad);
	}
	*/
}
#if 0
void rfid_ask_road(BYTE scr, BYTE des, BYTE length,	WORD cmd ,WORD RFID_Num)
{ 
	WORD i = 0,j = 0;
    byte num_1=0x00,num_2=0x00,num_3=0x00, num_4=0x00;//ou
	byte check;
	int num_temp=0x00000000;
	Temp_Send_Data.scr=scr;
	Temp_Send_Data.des=des;
	Temp_Send_Data.length=length;
	Temp_Send_Data.cmd=cmd;
	Temp_Send_Data.RFID_Num=RFID_Num;
	remote_frame_data_send[i++] = 0xAA;
	remote_frame_data_send[i++] = 0xBB;
	remote_frame_data_send[i++] = scr;
	remote_frame_data_send[i++] = des;
	remote_frame_data_send[i++] = length;
	num_temp=0xFF;
	num_1=(byte)((cmd>>8)&num_temp);
	remote_frame_data_send[i++] = num_1 ;
	num_2=(byte)(cmd&num_temp);
	remote_frame_data_send[i++] = num_2 ;
	num_3=(byte)((RFID_Num>>8)&num_temp);
	remote_frame_data_send[i++] = num_3 ;
	num_4=(byte)(RFID_Num&num_temp);
	remote_frame_data_send[i++] = num_4;
	check=remote_frame_data_send[2];
	for(j=3;j<9;j++)
	{
		check^=remote_frame_data_send[j];
	}
	remote_frame_data_send[i++] = check;
	serial_port_0_TX_array(remote_frame_data_send, 10);//ouyang
}
#endif 
// 以下部分非必须程序，用于2016赛季应答机制//
//*********************************************************************************
//  打包程序 ，用来使程序可读性更好
//*********************************************************************************
void sending_service_package (BYTE toward, WORD cmd, WORD data)
{  
	des=toward;
	cmd_WIFI=cmd;
	sending_data=data;
	main_wifi_sender();
}

//*********************************************************************************
//  主发送程序                 输入： 发送所需的数据      输出： 1 串口发送      2  waiting位     3 串口发送备份给备发送程序    4 发送丢失数
//*********************************************************************************
void main_wifi_sender (void)
{  

//	***********如果依然在等待回复，放弃上一个发送的等待，并且lostdata数加一***************
	if (waiting_for_response==1)
	{
	   lost_data++;
	   waiting_for_response=0;
	}
//	***********发送函数主体***************	                                    
	generate_remote_frame_2(g_device_NO_Hex, des, cmd_WIFI, 2, (const BYTE *)(&sending_data));
//  ***********等待回复位置1*************** 
	waiting_for_response=1;
	have_responsed=0;  
	sending_waiter=0;
}

//*********************************************************************************
//  应答检查程序               定时检查发送的数据是否得到了应答，若未，则使用辅助发送程序再次发送。 直到收到应答或有新的程序要发数据。
//*********************************************************************************
void wifi_sender_checker (void)
{ 
	if (sending_waiter<5)
	{
		return;
	}
	else
	{
		if (waiting_for_response==1)
		{
			if (have_responsed==1)
			{
				waiting_for_response=0;
			}
			else if (have_responsed==0)
			{
				ancillary_wifi_sender ();
			}
		}
	}
}
//*********************************************************************************
//  辅助发送程序                 输入： 如果未应答，再发送数据      输出：  串口发送    
//*********************************************************************************
void ancillary_wifi_sender (void)
{                                      		    
	generate_remote_frame_2(g_device_NO_Hex, des, cmd_WIFI, 2, (const BYTE *)(&sending_data));
}



void Wifi_Ctrl()
{
	if(remote_frame_data[2]==0x55)//来自dby的电脑
	{
		int m=change_hex_into_dec(remote_frame_data[7]);
		WORD distance=(WORD)(remote_frame_data[8]);
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x66)//右转
//		{
//			right_turn=1;
//			high8=((WORD)(remote_frame_data[7])<<8);
//			low8=(WORD)(remote_frame_data[8]);
//			steer_rate=(high8|low8);
//		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x77)//直走
//		{
//			straight_drive=1;
//		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x00)//停车
//		{
//			car_stop=1;
//		}
//		
//		if(remote_frame_data[5]==0x00&&remote_frame_data[6]==0x33)//倒车_李秋键
//		{
//			car_go_back=1;
//		}
//		if(remote_frame_data[5]==0x00&&remote_frame_data[6]==0x99)//调头_李秋键
//		{
//			car_turn_around=1;
//		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x55)//左转
//		{
//			left_turn=1;
//			high8=((WORD)(remote_frame_data[7])<<8);
//			low8=(WORD)(remote_frame_data[8]);
//			steer_rate=(high8|low8);	
//		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x88)//变速
		{
			speed_change=1;
			high9=((WORD)(remote_frame_data[7])<<8);
			low9=(WORD)(remote_frame_data[8]);
			speed_number=(high9|low9);
		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x11)//给角度zhaodeng
		{
			barrier_left_detected=0;
			barrier_right_detected=0;
			barrier_offset=0;
			light_offset=0;
			if(m>=0+3*light_offset&&m<42)
			{
				left_turn=1;
			    //high8=(0x00<<8);
			    steer_rate=m;
			}
			if(m<0&&m>=-42)
			{
				right_turn=1;
				//high8=(0x00<<8);
				steer_rate=-1*m;
			}
			if(m>=42&&m<=127)
			{
				left_turn=1;
//				high8=(0x00<<8);
			 //   low8=(WORD)((WORD)(remote_frame_data[7]));
				steer_rate=42;
			}
			if(m>=-128&&m<-42)
			{
				right_turn=1;
			//	high8=(0x00<<8);
			//  low8=(WORD)((WORD)(remote_frame_data[7]));
				steer_rate=42;
			}
			if(m>=-128&&m<=-107||m<=127&&m>=106)
			{
				yanshi=1;
			}
			if(distance<16&&distance>5)
			{
				light_offset=1;
			}
//			if(remote_frame_data[7]>=0x40&&remote_frame_data[7]<0xC0)//如果信标不在车前半部分范围内时执行
//			{
//				car_turn_around=1;
//			}
//			if(remote_frame_data[8]<0x02)
//			{
//				target_near=1;
//			}
//			if(remote_frame_data[7]<=0x0E||remote_frame_data[7]>=0xF1&&remote_frame_data[8]<0x05)
//			{
//				target_access=1;
//			}
			message_received=1;
			
		}		
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x22)//丢失
		{
			//barrier_offset=0;
			target_lost++;
		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x44)//避障
		{
			//if(m>0-10*barrier_offset&&m<30&&remote_frame_data[8]<0x78)
			if(m>=0+3*barrier_offset)			
			{
				barrier_left_detected=1;
				barrier_offset=-1;
				LCD_Write_Num(105,1,m,2);
				LCD_Write_Num(105,2,barrier_offset,2);
				//low8=120-(WORD)(remote_frame_data[8]);
				if(m>=-25&&m<=25)
				{
					angle_rate=60;
				}				
//					
//				if(angle_rate>60)
//				{
//					angle_rate=60;
//				}
			}
			//if(m<0-10*barrier_offset&&m>-30&&remote_frame_data[8]<0x78)
			//else if(m)
			else
			{
				barrier_right_detected=1;
				barrier_offset=1;
				LCD_Write_Num(105,1,m,2);
				LCD_Write_Num(105,2,barrier_offset,2);
				//low8=120-(WORD)(remote_frame_data[8]);
				if(m>=-25&&m<=25)
				{
				   angle_rate=60;
				}
			}
//			if(remote_frame_data[8]<0x30)
//			{
//				stuck++;
//			}
			message_received=1;
			bz=1;
		}
		
	}
	//if(remote_frame_data[2]==0x02)//来自dby的自制上位机
	//{
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x66)
	//	{
	//		right_turn=1;
	//	}
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x77)
	//	{
	//		straight_drive=1;
	//	}
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x00)
	//	{
	//		car_stop=1;
	//	}
	//}


}


