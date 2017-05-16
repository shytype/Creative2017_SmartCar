/*
 * analyse.h
 *
 *  Created on: Jun 20, 2015
 *      Author: Administrator
 */

#ifndef ANALYSE_H_
#define ANALYSE_H_

#define MAX(a,b)            (((a) > (b)) ?   (a)  : (b))
#define MIN(a,b)            (((a) < (b)) ?   (a)  : (b))
#define DISTANCE(a,b)       (((a) < (b)) ? (b-a)  : (a-b))
#define ABS(x)              ((x) > 0 ? (x) : 0-(x))


//左右线类型
#define UnExist	(0)
#define	Exist	(1)

//线型声明
//0-1:首次找线中间量
#define UnBegin	  (0)	//没开始找线
#define CrossLine (1)   //十字
//4- :最终赛道类型
#define NoLine    (2)   //没找到线
#define Straight  (3)   //直道
#define BigT	  (4)   //大弯
#define BigS 	  (5)   //大S
#define SmallS    (6)   //小S
#define ToT		  (7)	//直道入弯
#define UTurn	  (8)	//U型弯
#define UnRec	  (9)	//无法识别的赛道类型
#define Wrong	  (10)	//无效图像
#define Cross	  (11)	//无效图像
#define Barrier1  (12)	//障碍
#define Barrier2  (13)	//障碍
#define Strange	  (14)	//奇怪

//有效行变量
#define Far		(20)
#define Mid		(21)
#define Near	(22)

//#####################
//找线参数声明
#define Left      (0)   //Left
#define Right     (1)   //Right
#define Center	  (2)
#define Stop      (20)	//首次没找到线，二次找线前移的位置
#define CrsLen	  (10)	//十字二次找线前移位置
#define COLUMN1_2 (41)
#define COLUMN2_3 (55)
#define COLUMN1_3 (27)
#define ROW1_2	  (35)
#define REROW	  (55)  //首次没找到，二次找线的起始行

//变量的外部声明
extern signed char  BlackLine[2][ROWS];
extern byte StartRow[2];
extern byte EndRow[2];
extern byte LineType[2];	//左右线类型
extern byte RoadType;		//赛道类型
extern byte RoadStart;		//赛道起始行
extern byte RoadEnd;		//赛道终止行
extern byte RoadTurn;		//赛道终止行
extern signed char CenterLine[ROWS];
extern signed char width[ROWS];

extern byte UpSlope;         //上坡标志
extern byte DownSlope;       //下坡标志
extern byte Slope;	         //坡道标志
extern byte turn[10];

//直角弯处理参数
extern byte flag_Rightangle;                 //直角标识出现标志位
extern byte flag_Rightangle_l;                          //判断直角标识 0：找到直角或者直角距离小车还很远    1：是直角
extern byte flag_Rightangle_r;                           //l为左转 r为右转
extern int VerticalAngleStraightTime;					//检测到直角弯后直跑的时间
extern byte distance_Rightangle;                        //车子距离直角底部的距离 
extern 	byte RightAngleTurnFlag;
extern byte Flag_BlackRow;						//黑线标志位行数
extern 	byte BRMR;								//较远端的那个转折行
extern 	byte TurnLeft;
extern 	byte TurnRight;
extern byte TurnTimeCount;
extern byte RightangleTurnFlag;

extern byte delta;	
extern int  delta_l;
extern int delta_r;
extern byte StopFindLine;

extern byte RightAngeleFlagCount;
extern signed char width[ROWS];

extern byte BCL_Start;
extern byte BCL_End;

extern byte BlackRowFlag;
extern byte PreBlackRowFlag;
extern byte BlackRow_Start;
extern byte BlackRow_End;
extern byte BlackRow_Start_Found_Flag;
extern byte BlackRow_End_Found_Flag;

extern byte AllBlackRow;
extern byte LeftBlackRow;
extern byte RightBlackRow;

extern byte CrossNum;
extern byte LeftWhiteRow;
extern byte RightWhiteRow;
extern byte OnlyLeftWhiteRow;			//左边为白行数
extern byte OnlyRightWhiteRow;			//右边为白行数

//检测到斜的直角标识 跳过直角标识 新的初始找线行和初始找线点
extern byte J_S_Flag;//检测直角标识位
extern byte Jump_Row;
extern byte Jump_S_P_l;
extern byte Jump_S_P_r;

void FindBlackLine(void);
void Line_Init(byte lr);
void BlackLine_Init(byte lr,byte irowb,byte irowe);
void FindLine(byte lr);
byte NextPoint(byte lr,byte irow,byte flags);
void FillLine(byte lr,byte start,byte end);
void Pre_handle(byte lr);
void RebuildLine();
void Analyze_Cross();
void Rec_Cross();
void FindCross(byte lr);
byte CrossPoint(byte lr,byte irow,byte pre_point,byte flags,byte offset);
void FillCross(byte lr);         //修复十字
void ReBuildCross();
void SmoothLine(byte lr);
void AnalyzeRoadType();
void AnalyzeRoad();
byte JudgeBarrier();
byte JudgeBCL();
byte JudgeStrange();
byte JudgeStraight();
byte JudgeSmallS();
byte JudgeToT();
byte JudgeBigT();
byte JudgeUTurn();
void JudgeGeneral();
void GetTurnPoint();
void GetCenterLine();
void DetectSlope();
void DetectUpSlope();
void DetectDownSlope();
void DetectDownSlope2();
void DetectStartLine();
void DetectStart();
void ReBuildWeight();

//******************************单黑线****************************************//
void FindLine_Simple();//找线程序
void Findedge(byte irow,signed char point_start);  //搜索边界
byte JudgeStraightFlag(byte irow,signed char search_point_l,signed char search_point_r);
byte Findline_type(byte irow);//进入单线找线程序后的单双线判断函数
//signed char Findpoint_start_double(byte irow);//计算每行(双线)搜索的起始点
void Findpoint_start_double(byte irow);//计算每行(双线)搜索的起始点
//signed char Findpoint_start_simple(byte irow);//确定中黑线的中心点 补双黑线
void Findpoint_start_simple(byte irow);//确定中黑线的中心点 补双黑线
void Findline_filldouble(signed char search_point,byte irow);//把单线转换为双线处理，并判断赛道类型
void Middle_Line_Smooth(void);//从最近处的一点开始进行平滑处理
void BCL_Smooth(void);//从最近处的一点开始进行平滑处理
void CenterLineFill(void);//从最近处的一点开始进行平滑处理
void Cross_Fill(void);



//******************************直角处理函数****************************************//
void Detect_Rigntangle();
void Analyse_Rigntangle();
float Slope_Get(byte m,byte n);//求两行中线之间的斜率，开始行为m，其数值较小
float Variance_Get(byte m,byte n);
float Slope_Get_Black(byte lr,byte m,byte n);
float Variance_Get_Black(byte lr,byte m,byte n);
void Find_Rightangle();
void Out_Rightangle();

//******************************偏移量计算****************************************//
void TargetOffset();
void UpSlopeOffset();
void BarrierOffset();
void StrangeOffset();
void DownSlopeOffset();

void StraightOffset();
void BigTOffset();
void BigSOffset();
void SmallSOffset();
void ToTOffset();
void UTurnOffset();
void ToToffset();
void GeneralOffset();
void RebuildOffset();


void NearOffset();
void MidOffset();
void FarOffset();

void GetParam(uint8_t start, uint8_t end);
//******************************2016赛季****************************************//
void Typejudge();

#endif /* ANALYSE_H_ */
