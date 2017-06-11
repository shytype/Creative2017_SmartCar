#ifndef __WIFI_H__
#define __WIFI_H__


#define WIFI_CMD_SET_HELM_TARGET (0x0001)	/* 远程修改舵机目标值 */
#define WIFI_CMD_SET_HELM_KP (0x0002)	/* 远程修改舵机P值 */
#define WIFI_CMD_SET_HELM_KI (0x0003)	/* 远程修改舵机I值 */
#define WIFI_CMD_SET_HELM_KD (0x0004)	/* 远程修改舵机D值 */
#define WIFI_CMD_SET_MOTOR_TARGET (0x0005)	/* 远程修改电机目标值 */
#define WIFI_CMD_SET_MOTOR_KP (0x0006)	/* 远程修改电机P值 */
#define WIFI_CMD_SET_MOTOR_KI (0x0007)	/* 远程修改电机I值 */
#define WIFI_CMD_SET_MOTOR_KD (0x0008)	/* 远程修改电机D值 */
#define WIFI_CMD_GET_GYRO_DATA (0x0009)	/* 获取陀螺仪数据 */
#define WIFI_CMD_UNGET_GYRO_DATA (0x000A)	/* 停止获取陀螺仪数据 */
#define WIFI_CMD_SET_STEER_HELM_CENTER (0x000B)	/* 设置方向舵机中值 */
#define WIFI_CMD_SET_STEER_HELM_LEFT (0x000C)	/* 设置方向舵机左极限 */
#define WIFI_CMD_SET_STEER_HELM_RIGHT (0x000D)	/* 设置方向舵机右极限 */
#define WIFI_CMD_WRITE_STEER_HELM_DATA_TO_TF (0x000E)	/* 方向舵机数据写入TF卡 */
#define WIFI_CMD_SEND_STEER_HELM_DATA_FROM_TF (0x000F)	/* 发送方向舵机数据至上位机 */
#define WIFI_CMD_GET_SEEED_NOW (0x0011)	/* 获取当前速度 */
#define WIFI_CMD_UNGET_SPEED_NOW (0x0012)	/* 停止获取当前速度 */
#define WIFI_CMD_START_ACTIVE (0x0013)/* 开始全场工作 */
#define WIFI_CMD_GET_GYRO_INTEGRAL_DATA (0x0014)	/* 获取陀螺仪积分值 */
#define WIFI_CMD_UNGET_GYRO_INTEGRAL_DATA (0x0015)	/* 停止获取陀螺仪积分值 */
#define WIFI_CMD_STOP_SPEED (0x0016)	/* 车辆停止 */


#define WIFI_CMD_NET (0x0100)	/* 封装网络数据 */
#define WIFI_CMD_NET_0_1 (0x0101)
#define WIFI_CMD_NET_0_2 (0x0102)
#define WIFI_CMD_NET_0_3 (0x0103)
#define WIFI_CMD_NET_0_4 (0x0104)
#define WIFI_CMD_NET_2_1 (0x0105)
#define WIFI_CMD_NET_3_2 (0x0106)
#define WIFI_CMD_NET_6_2 (0x0107)
#define WIFI_CMD_NET_6_3 (0x0108)
#define WIFI_CMD_NET_6_4 (0x0109)
#define WIFI_CMD_NET_7_1 (0x010A)
#define WIFI_CMD_NET_BRIDGE (0x010B)
#define WIFI_CMD_NET_3_1 (0x010C)
#define WIFI_CMD_NET_3_3 (0x010F)
#define WIFI_CMD_NET_5_1 (0x010D)
#define WIFI_CMD_NET_6_3_2 (0x010E)
#define WIFI_CMD_NET_5_1_2 (0x0110)
#define WIFI_CMD_NET_5_1_3 (0x0111)
#define WIFI_CMD_NET_5_2 (0x0112)
#define WIFI_CMD_NET_ONLINE (0x0113)	/* 用来表明自己已上电 */
#define WIFI_CMD_NET_4_1 (0x0114)
#define WIFI_CMD_NET_4_2 (0x0115)
#define WIFI_CMD_ASK_ROAD (0x0116)
#define WIFI_CMD_ASK_LIGHT (0x0117)

#define WIFI_CMD_LIGHT_RED     (0x0120) //红灯
#define WIFI_CMD_LIGHT_YELLOW     (0x0121) //黄灯
#define WIFI_CMD_LIGHT_GREEN     (0x0122) //绿灯

#define WIFI_CMD_ROADBLOCK_NO  (0x0130) //无路障
#define WIFI_CMD_ROADBLOCK_YES  (0x0131) //有路障
#define WIFI_CMD_ROADBLOCK_unKNOW  (0x0132) //路障未知

#define WIFI_CMD_ROADBLOCK_REPORT_NO  (0x0135) //车辆反馈无路障
#define WIFI_CMD_ROADBLOCK_REPORT_YES  (0x0136) //车辆反馈有路障



#define WIFI_NET_CMD_CAR_REPORT_CURRENT_SITE (0x0001)	/* 车报告当前读到的位置 */
#define WIFI_NET_CMD_CAR_REPORT_CACHE_SITE (0x0002)	/* 车报告缓存的位置 即最近一次读到的位置 */


#define REMOTE_FRAME_LENGTH (10)                  /* 改为10*/
#define REMOTE_FRAME_STATE_OK (1)
#define REMOTE_FRAME_STATE_NOK (0)

//#define WIFI_ADDRESS (1)
#define ANDROID_ADDRESS (3)

#define WIFI_ADDRESS_WITHOUT_INIT (0)	/* 警车1 漂移 */
#define WIFI_ADDRESS_CAR_1 (1)	/* 路虎*/
#define WIFI_ADDRESS_CAR_2 (2)	/* 宝马x6*/
#define WIFI_ADDRESS_CAR_3 (3)	/* 宝马i8 */
#define WIFI_ADDRESS_CAR_4 (4)	/* 匪车2 */
#define WIFI_ADDRESS_DRAWBRIDGE (5)	/* 吊桥 */
#define WIFI_ADDRESS_DRAHTBRIDGE (6)	/* 钢丝桥 */
#define WIFI_ADDRESS_PC (7)	/* PC上位机 */
#define WIFI_ADDRESS_ANDRIUD_ZHOU (8)	/* Andriud上位机 of 周斯航 */
#define WIFI_ADDRESS_ANDRIUD_YE (9)	/* Andriud上位机 of 叶川 */
#define WIFI_ADDRESS_ANDRIUD_JIAO (10)	/* Andriud上位机 of 焦剑 */
#define WIFI_ADDRESS_ANDRIUD_YU (11)	/* Andriud上位机 of 余婧 */
#define WIFI_ADDRESS_Road_Node_1 (2)
#define WIFI_ADDRESS_BROADCAST (0xFF)	/* 广播地址 */

extern BYTE remote_frame_data[];
extern int g_remote_frame_state;
extern BYTE g_device_NO;
extern int g_start_all;
extern int sending_waiter;
extern int have_responsed;
extern int order_received;
extern WORD response_data;
extern int Car_Waitfororder;
extern int Light_Status;
extern BYTE waiting_for_response;
extern BYTE g_device_NO_Hex;/* 设备号 即WiFi地址 */
extern BYTE des;
extern WORD cmd_WIFI;
extern WORD sending_data;

/* 远程控制标志位 */
#ifdef __WIFI_C_
struct
{
	int send_gyro_data;	/* 发送陀螺仪数据 */
	int send_seppd_now;	/* 发送当前速度 */
	int send_gyro_integral;
} g_remote_control_flags = { 0, 0, 0 };
#else
extern struct
{
	int send_gyro_data;
	int send_seppd_now;
	int send_gyro_integral;
} g_remote_control_flags;
#endif

/* 网络控制结构体 */
#ifdef __WIFI_C_
struct
{
	int is_new_cmd;
	WORD cmd;
	DWORD last_report_online_time;
} g_net_control_data = { 0, 0x00, 0x00000000 };
#else
extern struct
{
	int is_new_cmd;
	WORD cmd;
	DWORD last_report_online_time;
} g_net_control_data;
#endif

extern BYTE check_sum(const BYTE *data, WORD length);
extern void execute_remote_cmd(const BYTE *data);
extern int rev_remote_frame_2(BYTE rev);
extern void generate_remote_frame_2(BYTE scr, BYTE des, WORD cmd, BYTE length, const BYTE data[]);
extern void send_remote_request_data(void);
void send_net_cmd(BYTE des, WORD net_cmd);
extern void rfid_ask_road(BYTE scr, BYTE des, BYTE length,	WORD cmd ,WORD RFID_Num);
void report_online(void);
extern int change_hex_into_dec(BYTE rev);

extern void main_wifi_sender (void);
extern void ancillary_wifi_sender (void);
extern void Wifi_Ctrl(void);
extern void Wifi_Test(void);
extern void wifi_sender_checker (void);
extern void sending_service_package ( BYTE toward, WORD cmd, WORD data);

extern struct
{
	BYTE scr;
	BYTE des;
	BYTE length;
	WORD cmd;
	WORD RFID_Num;
} Temp_Send_Data;
#endif

