#ifndef __CAMCONTROL_H__
#define __CAMCONTROL_H__


#define Motor_PWM_MAX (200)
#define Motor_PWM_MIN (-100)


//**********************舵机参数**********************************************
extern signed long target_offset,last_offset;
extern double Steer_kp,Steer_kd;//舵机P、D值
extern WORD Steer_PWM[4];//舵机输出值记录


//**********************电机参数**********************************************
extern word speedcounter1,speedcounter2;
extern signed int currentspeed,targetspeed;		//当前速度、目标速度
extern signed int Error,PreError,SumError;
extern double Speed_kp,Speed_ki,Speed_kd;	//电机PID值
extern signed int Motor_PWM;				//电机底层控制量

//********************辅助调试参数******************************************
extern unsigned int Counter_Error;		//光编接触不牢靠错误计数量
extern unsigned int StartTime2s;		//起始线时间计算(起车2s,延时一段时间再检测起始线,第二次见到起始线停车)
extern byte stop_delay;
extern byte stop_flag;

//********************角度、速度控制******************************************
extern void PitISR(void);
void SteerControl(void);
void SpeedControl(void);




#endif
