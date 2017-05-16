#ifndef __RFIDCARD_H__
#define __RFIDCARD_H__

#define RFID_CMD_READ_CARD (0x20)
#define RFID_CMD_READ_DATA_BLOCK (0x21)
#define RFID_CMD_ENERGETIC_MODE (0x41)

#define RFID_CARD_ID
#define RFID_CARD_ID_PUNCTURED_BALLON_START (0x0A1E4748)
#define RFID_CARD_ID_PUNCTURED_BALLON_TURN_1 (0x4AF14448)
#define RFID_CARD_ID_PUNCTURED_BALLON_STOP (0x1250DD76)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN1 (0xCA7C00BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN1 (0xDA8220BD)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR4_TURN2 (0xDE913FC9)
#define RFID_CARD_ID_PUNCTURED_BALLON_CAR3_TURN2 (0x4AF14448)
#define RFID_CARD_ID_1 (0xDAA23548)
#define RFID_CARD_ID_2 (0xFA8320BD)

#define RFID_CARD_ID_1_1 (0x6AEEFEBC)
#define RFID_CARD_ID_1_2 (0x4A72FFBC)
#define RFID_CARD_ID_1_3 (0x4AF14448)
#define RFID_CARD_ID_1_4 (0xDA945248)
#define RFID_CARD_ID_2_1 (0xB49C2C52)
#define RFID_CARD_ID_2_2 (0x344F2952)
#define RFID_CARD_ID_2_3 (0xE44B2852)
#define RFID_CARD_ID_2_4 (0x1250DD76)  //big card
#define RFID_CARD_ID_3_1 (0x04E32852)
#define RFID_CARD_ID_3_2 (0x24572C52)
#define RFID_CARD_ID_3_3 (0xF4812652)
#define RFID_CARD_ID_4_1 (0x5A934B48)
#define RFID_CARD_ID_4_2 (0xCA7C00BD)
#define RFID_CARD_ID_4_3 (0xDA8220BD)
#define RFID_CARD_ID_5_1 (0xDAA23548)
#define RFID_CARD_ID_5_2 (0xA41C2F52)
#define RFID_CARD_ID_5_3 (0x4A4A00BD)
#define RFID_CARD_ID_5_4 (0x74642952)
#define RFID_CARD_ID_5_5 (0xF4A22952)
#define RFID_CARD_ID_6_1 (0x040E2752)
#define RFID_CARD_ID_6_2 (0x64E12952)
#define RFID_CARD_ID_6_3 (0x64282D52)
#define RFID_CARD_ID_6_4 (0x04172952)
#define RFID_CARD_ID_6_5 (0x00000000)
#define RFID_CARD_ID_6_6 (0x00000000)
#define RFID_CARD_ID_7_1 (0x74012852)
#define RFID_CARD_ID_7_2 (0xD4CB2952)
#define RFID_CARD_ID_7_3 (0x048D2952)



#define RFID_FRAME_LENGTH_MAX (256)
#define RFID_FRAME_STATE_OK (1)
#define RFID_FRAME_STATE_NOK (0)

#define RFID_MODUL_TYPE_UNKNOW (0)
#define RFID_MODUL_TYPE_OLD (1)
#define RFID_MODUL_TYPE_NEW (2)

//#define ROAD_NUM_1001 (0x3ABE1FBD)
//#define ROAD_NUM_1002 (0xCA7C00BD)
#define ROAD_NUM_1101 (0x7C349722)//红绿灯路口0x11类型
#define ROAD_NUM_1102 (0x8C139722)
#define ROAD_NUM_1103 (0xAC3D7422)
#define ROAD_NUM_1104 (0x3221EDD3)
#define ROAD_NUM_1105 (0x4C8E9222)
#define ROAD_NUM_1105_1 (0xAC331856)
#define ROAD_NUM_1106 (0x0CF59622)
#define ROAD_NUM_1201 (0x12517BD6)
#define ROAD_NUM_1201_1 (0x72BC4059)
#define ROAD_NUM_1202 (0xE22576D6)

//天少专车地点号
#define ROAD_NUM_4001 (0xDAA23548)//桥下住宅区
#define ROAD_NUM_4001_1 (0x827958D6)
#define ROAD_NUM_4002 (0x5A934B48)//CBD工作区
#define ROAD_NUM_4002_1 (0x625544D6)
#define ROAD_NUM_4002_2 (0x52DAB259)
#define ROAD_NUM_4003 (0xAC907522)//警察局
#define ROAD_NUM_4003_1 (0x72147CD6)

#define ROAD_NUM_2101 (0x629A4659)//左打死路口
#define ROAD_NUM_2101_1 (0x02314059)
#define ROAD_NUM_2101_2 (0xE29A3E59)
#define ROAD_NUM_2102 (0xCA4900BD)
#define ROAD_NUM_2102_1 (0xFCE01C56)
#define ROAD_NUM_2102_2 (0xA2A54459)
#define ROAD_NUM_2103 (0xFA8320BD)

#define ROAD_NUM_2202 (0x526480D6)//右打死路口
#define ROAD_NUM_2203 (0x223E7AD6)

#define ROAD_NUM_2301 (0x2CA08D56)//1号库停车卡
#define ROAD_NUM_2301_1 (0xBCCC7422)
#define ROAD_NUM_2302 (0x92AB82D6)//2号库停车卡
#define ROAD_NUM_2303 (0x4C831856)//3号库停车卡

#define ROAD_NUM_2401 (0x7CB31F04)//出库卡
#define ROAD_NUM_2402 (0xFC9B8422)
#define ROAD_NUM_2403 (0x0C467522)
#define ROAD_NUM_2404 (0x4206BE59)

#define ROAD_NUM_2501 (0x1250DD76)//入库卡（收费卡）
#define ROAD_NUM_2501_1 (0xDC822656)

#define ROAD_NUM_2601 (0x72D079D6)//进停车场
#define ROAD_NUM_2601_1 (0x0CF70422)

#define ROAD_NUM_3001 (0xC269EAD3)//（桥）正常行驶
#define ROAD_NUM_3001_1 (0xC26F49D6)
#define ROAD_NUM_3001_2 (0xA20FBE59)

#define ROAD_NUM_3002 (0x9CC73424)//下桥卡
#define ROAD_NUM_3002_1 (0x7CE88B56)
#define ROAD_NUM_3002_2 (0x920CB159)

#define ROAD_NUM_3003 (0xD23487D6)//道路卡

#define ROAD_NUM_3101 (0x6C388622)//（桥）加速

#define ROAD_NUM_3201 (0x621C82D6)//（桥）减速
#define ROAD_NUM_3201_1 (0x6CDE9622)
#define ROAD_NUM_3201_2 (0x02BABA59)

#define ROAD_NUM_8001 (0x828D41D6)//全场定位
#define ROAD_NUM_8002 (0xF2FB44D6)
#define ROAD_NUM_8003 (0x629AE5D3)
#define ROAD_NUM_8004 (0xB23B54D6)
#define ROAD_NUM_8005 (0x52C6EAD5)
#define ROAD_NUM_8005_1 (0x0216BD59)
#define ROAD_NUM_8006 (0xB28B7DD6)
#define ROAD_NUM_8007 (0x328F80D6)
#define ROAD_NUM_8008 (0x829DEAD3)
#define ROAD_NUM_8009 (0x12D7ECD3)
#define ROAD_NUM_8009_1 (0x9C222756)
#define ROAD_NUM_8010 (0x621773D6)
#define ROAD_NUM_8011 (0x125981D6)
#define ROAD_NUM_8012 (0x421C76D6)
#define ROAD_NUM_8012_1 (0x320DBF59)
#define ROAD_NUM_8013 (0xD2C643D6)
#define ROAD_NUM_8014 (0x32785DD6)
#define ROAD_NUM_8014_1 (0x4C449622)
#define ROAD_NUM_8015 (0xA20086D6)
#define ROAD_NUM_8016 (0xB23D43D6)
#define ROAD_NUM_8017 (0x92BB86D6)
#define ROAD_NUM_8018 (0x02D180D6)
#define ROAD_NUM_8019 (0xDCD09722)
#define ROAD_NUM_8020 (0x620543D6)

#define ROAD_NUM_2222 (0x52DAB259)

#define ROAD_NUM_0A01 (0x5CD07422)//特种车
#define ROAD_NUM_0A02 (0x822875D6)

extern const BYTE rfid_cmd_get_id[];
extern const BYTE rfid_cmd_get_data[];
extern const BYTE rfid_cmd_energetic_mode_enable[];
extern const BYTE rfid_cmd_energetic_mode_enable_new[];
extern const BYTE rfid_cmd_energetic_mode_disable[];
extern const BYTE rfid_cmd_energetic_mode_disable_new[];
//extern DWORD rfid_site;


/* 位置数据 */
#ifdef __CONTROL_C_
struct
{
	DWORD site;
	DWORD time;
	DWORD old_site;
	WORD roadnum;
	int is_new_site;
} RFID_site_data = { 0x00000000, 0x00000000, 0x00000000,0 ,0};
#else
extern struct
{
	DWORD site;
	DWORD time;
	DWORD old_site;
	WORD roadnum;
	int is_new_site;
} RFID_site_data;
#endif

extern int rev_RFID_frame(BYTE rev);
extern void explane_RFID_ret_data(const BYTE *data, WORD length);
extern void explane_RFID_ret_cardID(DWORD id);
extern int init_RFID_modul_type(void);
extern WORD RFID_Num_Exp(DWORD site);
int Rfid_frame_rebuild(BYTE r_f_data[],int g_r_f_cnt);
#endif
