#define __DRIFTING_C_
#include "includes.h"
DWORD start_time = 0x00000000;

void drift_right1(void)//砖地面180度 背景布100度//
{
	set_speed_target(80);
	delay_ms(1500);
	set_steer_helm(data_steer_helm.right_limit);
	delay_ms(200);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(data_steer_helm.center);
	/*set_speed_target(40);
	delay_ms(800);
	set_speed_target(0);
	delay_ms(2000);*/
	
}


void drift_right4(void)//砖地面180度 背景布100度//
{
	set_speed_target(70);
	delay_ms(3000);
	set_steer_helm(data_steer_helm.left_limit);
	delay_ms(3000);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(data_steer_helm.center);
	/*set_speed_target(40);
	delay_ms(800);
	set_speed_target(0);
	delay_ms(2000);*/
	
}


void drift_right5(void)//砖地面180度 背景布100度
{
	set_speed_target(80);
	delay_ms(1500);
	set_steer_helm(data_steer_helm.right_limit);
	delay_ms(400);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(data_steer_helm.center);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}


void drift_right6(void)//砖地面180度 背景布100度
{
	set_speed_target(70);
	delay_ms(1500);
	set_steer_helm(data_steer_helm.right_limit);
	delay_ms(600);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(data_steer_helm.center);
	/*set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);*/
	
}

void drift_left(int d)//使用陀螺仪
{
	set_speed_target(40);
	delay_ms(500);
	g_f_enable_rad_control_1 = d;
	reset_rev_data();
	start_time = g_time_basis_PIT;
	/* 前0.5s为第一阶段 */
//	while(diff_time_basis_PIT(g_time_basis_PIT,start_time)<=0x00000032)
//	{
	//  先稍微向右打方向，因为总是会有超调。。
		//set_steer_helm(data_steer_helm.center+10);
//	}
	cl_flag=1;
}


void drift_test(void)
{
	set_steer_helm(data_steer_helm.right_limit);
}


void send_speed(void)//max speed=160
{
	WORD j=0x0000;
	BYTE o,s;
	
	j=0xffff&&data_encoder.speed_now;
	o=(BYTE)j;
	s=(BYTE)(j>>8);
	serial_port_1_TX(s);
	serial_port_1_TX(o);
}

