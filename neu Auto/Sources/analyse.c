/*
 * analyse.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Administrator
 */

#include "includes.h"
//2016赛季 参数定义
extern int bz;
extern int jishu;
extern int zhangai;
extern int up;
//找线变量定义
signed char BlackLine[2][ROWS];	  //左右线的数组
byte StartRow[2];       	  //线的起点
byte EndRow[2];         	  //线的终点
byte BreakRow[2];       	  //线的中断点
byte ContinueRow[2];   	      //线的续接点
byte SegNum[2];				  //黑线的段数
byte TurnRow[2];         	  //拐角点位置
byte LineType[2]={0,0};  	  //找到的线的类型，初始化为0
signed char CenterLine[ROWS]; //找到的中线数组
signed char dif[70];		  //二次差分量

//赛道类型有关量
byte RoadType=UnBegin;					//赛道类型
byte RoadStart=ROWS;					//赛道起始行
byte RoadEnd=0;			   			    //赛道终止行
byte RoadTurn;			   			    //赛道拐点
byte PastType[10];						//未用
byte MostL[3];							//0:左线	1:右线	2:中线部分	
byte MostR[3];							//0:左线	1:右线	2:中线部分	
byte left_d[3],right_d[3],mid_d[3];		//0:左线	1:右线	2:中线部分	
byte turn[10],count;					//turn[10]统计中线的起点、拐点、终点
byte TurnPoint=0;

//十字类型判断
byte NearCross[2];
byte FarCross[2];
byte CrossFlags;

//坡道识别参数
byte up_fnum=0;
byte up_unfnum=0;
byte down_fnum=0;
byte down_unfnum=0;
byte slope_flags=0;
byte UpSlope=0;					//上坡标志
byte DownSlope=0;				//下坡标志
byte slope_delay=0;
byte detect_slope_delay=0;
byte detected=0;
byte Slope=0;					//0:正常	1:上坡和坡上	2:下坡
byte PastUpSlope=0;
byte PPastUpSlope=0;
byte PPPastUpSlope=0;
byte PastDownSlope=0;
byte PPastDownSlope=0;
byte TopFlag=0;
byte DownFlag=0;

//直角弯处理参数
byte flag_Rightangle=0;  
byte flag_Rightangle_l=0;                         //判断直角标识 0：找到直角或者直角距离小车还很远    1：是直角
byte flag_Rightangle_r=0;						  //l为左转 r为右转
byte Distance_Rightangle=ROW;                     //车子距离直角底部的距离
int VerticalAngleStraightTime=0;				  //检测到直角弯后直跑的时间
byte RightAngleTurnFlag=0;
byte Flag_BlackRow;								  //黑线标志位行数
byte BRMR;										  //较远端的那个转折行
byte RightAngeleFlagCount=0;
byte TurnLeft=0;								  //左直角预处理标志
byte TurnRight=0;								  //右直角预处理标志
byte TurnTimeCount=0;
byte RightangleTurnFlag=0;

//单黑线
byte FindLine_Type=0;
byte StartPoint_Simple;//找单线的起始行
byte StartRow_Simple;//找单线的起始点
signed char BlackCenterLine_Simple[ROWS];//中黑线数组

//双黑线
signed char BlackCenterLine_Double[ROWS];//中黑线数组

//新找线参数
byte delta;
int delta_l;
int delta_r;
byte BCL_Start;						//中黑线开始行
byte BCL_End;						//中黑线结束行
byte BlackRowFlag;					//此行左搜索点或右搜索点为黑
byte PreBlackRowFlag;				//上一行左搜索点或右搜索点为黑
byte BlackRow_Start;				//黑行开始
byte BlackRow_End;					//黑行结束
byte BlackRow_Start_Found_Flag;		//黑行开始找到标志
byte BlackRow_End_Found_Flag;		//黑行结束找到标志
byte AllBlackRow;					//全黑行数
byte LeftBlackRow;					//左边为黑行数
byte RightBlackRow;				    //右边为黑行数
byte CrossNum=0;					//全白行数
byte OnlyLeftWhiteRow=0;			//左边为白行数
byte OnlyRightWhiteRow=0;			//右边为白行数
byte LeftWhiteRow=0;				//左边为白行数
byte RightWhiteRow=0;				//右边为白行数
byte StopFindLine=0;				//停止找线标志

signed char width[ROWS]=
{//赛道宽度(严重影响入中黑线，会不会影响障碍判断未知)

//实际赛道宽度
//22,22,24,24,26,26,28,28,30,30,
//32,32,34,34,34,36,36,38,38,40,
//40,42,42,42,44,44,46,46,48,48,
//48,50,50,52,52,52,54,54,54,56,
//56,58,58,58,60,60,62,62,62,64,
//64,64,66,66,68,68,68,70,70,70,
//70,72,72,74,74,74,74,76,76,78,
		
//略宽裕实际赛道宽度，中黑线处理效果较好
28,30,30,32,32,34,34,36,36,38,
38,38,40,40,42,42,44,44,46,46,
48,48,50,50,50,52,52,54,54,54,
56,56,58,58,58,60,60,60,62,62,
64,64,64,66,66,68,68,68,70,70,
70,72,72,72,74,74,74,76,76,76,
78,78,78,78,80,80,80,80,80,80,
};

//*****************************************************************************************************************
//*	     *************************有效行权值分配******************************************************* 	      *
//*****************************************************************************************************************
byte ToTWeight[ROWS]=//15-29
{//远
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};//近
byte UTurnWeight[ROWS]=		//偏移量权值
{
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte FarWeight[ROWS]=//0-14
{	
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte NearWeight[ROWS]=//30-69
{//远
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte BarrierWeight[ROWS]=
{
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte StraightWeight[ROWS]=		//直道偏移量权值
{//远
		10,10,10,10,10,9,9,9,9,9,//1
		8,8,8,8,8,7,7,7,7,7,//2
		6,6,6,6,6,5,5,5,5,5,//3
		4,4,4,4,4,4,4,4,4,4,//4
		3,3,3,3,3,3,3,3,3,3,//5
		2,2,2,2,2,2,2,2,2,2,//6
		1,1,1,1,1,1,1,1,1,1,//7
};//近
byte ToTWeight1[ROWS]=//15-29
{//远
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,		
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
};//近
byte UTurnWeight1[ROWS]=		//偏移量权值
{
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,		
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
};
byte FarWeight1[ROWS]=//0-14
{	
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,		
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,
};
byte NearWeight1[ROWS]=//30-69
{//远
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte BarrierWeight1[ROWS]=
{
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		1,1,1,1,1,1,1,1,1,1,
};
byte StraightWeight1[ROWS]=		//直道偏移量权值
{//远		
		10,10,10,10,10,9,9,9,9,9,//1
		8,8,8,8,8,7,7,7,7,7,//2
		6,6,6,6,6,5,5,5,5,5,//3
		4,4,4,4,4,4,4,4,4,4,//4
		3,3,3,3,3,3,3,3,3,3,//5
		2,2,2,2,2,2,2,2,2,2,//6
		1,1,1,1,1,1,1,1,1,1,//7
};//近
byte SlopeWeight[ROWS]=
{
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,
};
/*
** ###################################################################
**     CCD 视频寻找黑线算法
**	   输入 g_pix[][]           // 采集的灰度数组
**	   输出 g_black_line[]		// 显示的黑线数组 
**		    BlackLine[][]		//左右黑线
** ###################################################################
*/
void FindBlackLine(void) 
{
    byte lr;
	signed char i;
	LineType[0]=LineType[1]=0;
	RoadType=UnBegin;
	RoadStart=ROWS;
	RoadEnd=0;
	BlackRowFlag=0;
	PreBlackRowFlag=0;
	BlackRow_Start=ROWS;
	BlackRow_End=0;
	BlackRow_Start_Found_Flag=0;
	BlackRow_End_Found_Flag=0;
	AllBlackRow=0;
	LeftBlackRow=0;
	RightBlackRow=0;
	CrossNum=0;	
	OnlyLeftWhiteRow=0;
	OnlyRightWhiteRow=0;
	BCL_Start=ROWS;
	BCL_End=0;
	//最近行找线偏移量要大，防止中黑线处车身不正
	delta_l=30;
	delta_r=30;
	
	for(lr=0;lr<2;lr++) //找左右线（0：左线，1：右线）
	{           
	   	Line_Init(lr);				//初始化左右线变量
	}
	FindLine_Simple();	//首次找单黑线 判断单->双  判断此次找线的起点和终点
	for(lr=0;lr<2;lr++) //找左右线（0：左线，1：右线）
	{   
		Pre_handle(lr);				//判断此次找线的起点和终点
	}
	Analyse_Rigntangle();           //分析是不是直角	
	if(flag_Rightangle_l==1)		D0=0;
	else if(flag_Rightangle_l==0)	D0=1;
	if(flag_Rightangle_r==1)		D1=0;
	else if(flag_Rightangle_r==0)	D1=1;
	Cross_Fill();
	//DetectSlope();					//检测坡道
	AnalyzeRoadType();				//分析赛道类型	
	CenterLineFill();				//中线补线
	if(g_device_NO==1)           //1号车中线偏移参数       jqy
	{
		if(up==0)
		{
			for(i=RoadEnd;i<RoadStart/2-10;i++)
				CenterLine[i]=(CenterLine[i]+11);
			for(i=RoadStart/2-10;i<=RoadStart;i++)
				CenterLine[i]=(CenterLine[i]+22);
		}
		if(up==1)
		{
		    for(i=RoadEnd;i<RoadStart/2-10;i++)
				CenterLine[i]=(CenterLine[i]+7);
			for(i=RoadStart/2-10;i<=RoadStart;i++)
				CenterLine[i]=(CenterLine[i]+14);
		}
	}
	if(g_device_NO==2)           //2号车中线偏移参数       jqy
	{
	for(i=RoadEnd;i<RoadStart/2-10;i++)
		CenterLine[i]=(CenterLine[i]+9);
	for(i=RoadStart/2-10;i<=RoadStart;i++)
		CenterLine[i]=(CenterLine[i]+16);
	}
	if(g_device_NO==3)           //3号车中线偏移参数       jqy
	{
	for(i=RoadEnd;i<RoadStart/2-10;i++)
		CenterLine[i]=(CenterLine[i]+9);
	for(i=RoadStart/2-10;i<=RoadStart;i++)
		CenterLine[i]=(CenterLine[i]+16);
	}
	TargetOffset();					//目标控制量
}

/**************************************************
函数功能：找线初始化
函数输入：lr
函数输出：BlackLine[2][70]  StartRow[lr]=ROWS;  EndRow[lr]=0; 
BlackCenterLine_Simple[irow]
说明：初始化中中黑线数组和双边黑线数组的值有待于商量 BlackLine[][]的数据类型根据后面的情况来确定
***************************************************/
void Line_Init(byte lr) 
{
	byte irow=0;
    StartRow[lr]=ROWS;
	EndRow[lr]=0;

	BlackLine_Init(lr,ROW,0);					//初始化双黑线
	for(;irow<=ROW;irow++)						//初始化中黑线数组
	{
		CenterLine[irow]=120;					//由于补线，中线实际范围在（-20）至100，故将中线初始化为120便于后面判断
	}
}

/**************************************************
函数功能：黑线初始化
函数输入：lr ROW 0
函数输出：BlackLine[2][70]
说明：初始化双边黑线数组的值有待于商量(根据后面的需求)
***************************************************/
void BlackLine_Init(byte lr,byte irowb,byte irowe)    
{
	byte irow=irowe;
	for(;irow<=irowb;++irow)
	   BlackLine[lr][irow]=COLUMNS;       
}

/**************************************************
函数功能：找线程序 单/双线
函数输入：g_pix[][]
函数输出：BlackLine[2][70]  
说明：初始化中中黑线数组和双边黑线数组的值有待于商量
***************************************************/
void FindLine_Simple()//找线程序
{
	signed char irow;
	byte findline_flag=0;							//单双线标志 0为双线 1为单线
	byte findline_flag_pre=0;						//单双线标志 0为双线 1为单线

	for(irow=ROW;irow>=0;irow--)
	{
		if((irow==ROW)||(CenterLine[irow+1]==120))//最近行和上一行未找线的情况
		{
			Findedge(irow,41);	
		}
		else
		{
			Findedge(irow,CenterLine[irow+1]);
		}
		
		findline_flag=Findline_type(irow);		//判断单线还是双线 findline_flag=0为双线，findline_flag=1为单线
		if(!findline_flag)
		{	
			Findpoint_start_double(irow);
		}
		if(findline_flag)
		{	
			Findpoint_start_simple(irow);
		}

		//找到中黑线起始行和结束行
		if((findline_flag)&&(!findline_flag_pre))
		{
			BCL_Start=irow;
		}
		if((!findline_flag)&&(findline_flag_pre))
		{
			BCL_End=irow+1;
		}
		findline_flag_pre=findline_flag;

		//上一行中线偏到最左或最右，下一行未找线时停止找线
		if((irow<=60)&&(CenterLine[irow]==120)&&(((CenterLine[irow+1]<=0)||(CenterLine[irow+1]>=(COLUMN-1)))&&(CenterLine[irow+1]!=120)))
		{
			RoadEnd=irow+1;
			break;
		}
		//上一行中线偏到最左或最右，下一行偏差过大时时停止找线
		if(((CenterLine[irow+1]<=3)||((CenterLine[irow+1]>=(COLUMN-4))&&(CenterLine[irow+1]!=120)))&&(ABS(CenterLine[irow]-CenterLine[irow+1])>20)&&(irow!=ROW))
		{
			RoadEnd=irow+1;
			break;
		}
		//在较远处，未找线时停止找线
		if((irow<20)&&(CenterLine[irow]==120))
		{
			RoadEnd=irow+1;
			break;
		}
		if((BCL_End!=0)&&(ABS(CenterLine[BCL_End]-CenterLine[BCL_End-1])>5))
		{
			RoadEnd=irow+1;
			break;
		}
		//如果是单黑线且单黑线偏到最左或最右，则单黑线后的图像较乱，停止找线
		if((findline_flag)&&((CenterLine[irow]<=2)||(CenterLine[irow]>=(COLUMN-2))))
		{
			RoadEnd=irow;
			break;
		}	
//		//针对斜直角标志位，中线跳变较大时
//		if((irow>40)&&(irow!=69)&&(ABS(CenterLine[irow]-CenterLine[irow+1])>5))
//		{
//			RoadEnd=irow+1;
//			break;
//		}
//		//针对弯道入中黑线，加了后直角过不好
//		if((irow!=ROW)&&(CenterLine[irow]!=120)&&(CenterLine[irow+1]!=120)&&(ABS(CenterLine[irow]-CenterLine[irow+1])>5))
//		{
//			RoadEnd=irow+1;
//			break;
//		}
	}
	//用黑色起始行进一步确保RoadEnd正确
	if((BlackRow_Start!=ROWS)&&(BlackRow_End==0))
	{
		RoadEnd=BlackRow_Start+1;
	}
}


/**************************************************
函数功能：搜索边界
函数输入：irow  point_start_pre（上一行的中心点 作为本行的起始搜索点) gix[70][83]
函数输出：BlackLine[][]
说明：
***************************************************/
//void Findedge(byte irow,signed char point_start )  //搜索边界
//{
//	byte i,j;								//循环变量
//	signed char search_point_l,search_point_r;      //左右搜索点
//
//	search_point_r=point_start-delta_l;		//初始化左右搜索点
//	if(search_point_r<0)
//	{
//		search_point_r=0;
//	}
//	if(search_point_r>(COLUMN-1))//因第82列为黑，将整个图像当做81列来处理
//	{
//		search_point_r=(COLUMN-1);//最右边有一条黑线
//	}
//	search_point_l=point_start+delta_r;
//	if(search_point_l<0)
//	{
//		search_point_l=0;
//	}
//	if(search_point_l>(COLUMN-1))//因第82列为黑，将整个图像当做81列来处理
//	{
//		search_point_l=(COLUMN-1);//最右边有一条黑线
//	}
//
//	if((g_pix[irow][search_point_l]==0)||(g_pix[irow][search_point_r]==0))//如果左搜索点或右搜索点为黑
//	{
//		if(irow==ROW)//最近行需特殊处理
//		{
//			if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]!=0)&&(g_pix[irow][0]>0))//赛道在视场左下角
//			{
//				BlackLine[0][irow]=0;
//				for(i=0;i<(COLUMN-1);i++)
//				{
//					if(g_pix[irow][i]==0)
//					{
//						break;
//					}			
//				}
//				BlackLine[1][irow]=i;
//			}
//			else if((g_pix[irow][search_point_l]!=0)&&(g_pix[irow][search_point_r]==0)&&(g_pix[irow][COLUMN-1]>0))//赛道在视场右下角
//			{
//				BlackLine[1][irow]=(COLUMN-1);
//				for(j=(COLUMN-1);j>0;j--)
//				{
//					if(g_pix[irow][j]==0)//从中间向右搜索到黑线，Right_Valid_Line++，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
//					{
//						break;
//					}			
//				}
//				BlackLine[0][irow]=j;
//			}
//			else//左右搜索点均为黑
//			{
//				BlackLine[0][irow]=-1;
//				BlackLine[1][irow]=COLUMN;
//
//				//判断是否为黑色起始行
//				BlackRowFlag=1;
//				if(BlackRow_Start_Found_Flag==0)
//				{
//					if(irow==ROW)
//					{
//						BlackRow_Start=irow;
//						BlackRow_Start_Found_Flag=1;
//					}
//					else if(PreBlackRowFlag==0)
//					{
//						BlackRow_Start=irow;
//						BlackRow_Start_Found_Flag=1;
//					}
//				}
//			}
//		}
//		else
//		{
//			BlackLine[0][irow]=-1;
//			BlackLine[1][irow]=COLUMN;
//
//			//判断是否为黑色起始行
//			BlackRowFlag=1;
//			if(BlackRow_Start_Found_Flag==0)
//			{
//				if(irow==ROW)
//				{
//					BlackRow_Start=irow;
//					BlackRow_Start_Found_Flag=1;
//				}
//				else if(PreBlackRowFlag==0)
//				{
//					BlackRow_Start=irow;
//					BlackRow_Start_Found_Flag=1;
//				}
//			}
//		}
//
//		//记录全黑、左黑、右黑行数
//		if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]==0))
//		{
//			AllBlackRow++;
//		}
//		if((g_pix[irow][search_point_l]!=0)&&(g_pix[irow][search_point_r]==0))
//		{
//			LeftBlackRow++;//用于判断障碍
//		}
//		if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]!=0))
//		{
//			RightBlackRow++;//用于判断障碍
//		}
//	}
//	else//左右搜索点均不为黑的情况
//	{
//		BlackRowFlag=0;
//		if((BlackRow_End_Found_Flag==0)&&(BlackRow_Start_Found_Flag==1)&&(PreBlackRowFlag==1))
//		{
//			BlackRow_End=irow+1;
//			BlackRow_End_Found_Flag=1;
//		}
//		for(i=search_point_l;i>0;i--)
//		{
//			if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
//			{
//				break;
//			}		
//		}
//		BlackLine[0][irow]=i;//左线最小为0
//		for(j=search_point_r;j<(COLUMN-1);j++)
//		{
//			if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
//			{
//				break;
//			}			
//		}
//		BlackLine[1][irow]=j;//右线最大为81
//	}
//	PreBlackRowFlag=BlackRowFlag;
//}

void Findedge(byte irow,signed char point_start )  //搜索边界
{
	byte i,j;								//循环变量
	signed char search_point_l,search_point_r;      //左右搜索点

	search_point_r=point_start-delta_l;		//初始化左右搜索点
	search_point_l=point_start+delta_r;
	//if(J_S_Flag)
	//{
	//	search_point_l=Jump_S_P_l;
	//	search_point_r=Jump_S_P_r;
	//	J_S_Flag=0;
	//}
	if(search_point_r<0)
	{
		search_point_r=0;
	}
	if(search_point_r>(COLUMN-1))//因第82列为黑，将整个图像当做81列来处理
	{
		search_point_r=(COLUMN-1);//最右边有一条黑线
	}	
	if(search_point_l<0)
	{
		search_point_l=0;
	}
	if(search_point_l>(COLUMN-1))//因第82列为黑，将整个图像当做81列来处理
	{
		search_point_l=(COLUMN-1);//最右边有一条黑线
	}

	//记录左黑、右黑行数
	if((g_pix[irow][search_point_l]!=0)&&(g_pix[irow][search_point_r]==0))
	{
		LeftBlackRow++;//用于判断障碍
	}
	if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]!=0))
	{
		RightBlackRow++;//用于判断障碍
	}

	if((g_pix[irow][search_point_l]==0)||(g_pix[irow][search_point_r]==0))//如果左搜索点或右搜索点为黑
	{
		if(irow==ROW)//最近行需特殊处理
		{
			if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]!=0)&&(g_pix[irow][0]>0))//赛道在视场左下角
			{
				for(i=71;i>41;i--)
				{
					if(g_pix[irow][i]>0)
					{
						search_point_l=i;
						break;
					}
				}
				if(i>41)
				{
					for(i=search_point_l;i>0;i--)
					{
						if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
						{
							break;
						}		
					}
					BlackLine[0][irow]=i;//左线最小为0
					for(j=search_point_r;j<(COLUMN-1);j++)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][irow]=j;//右线最大为81
					if((BlackLine[0][irow]-BlackLine[1][irow])>8)
					{
						BlackLine[0][irow]=-1;
						BlackLine[0][irow]=ROW;
					}
				}
				else
				{
					BlackLine[0][irow]=0;
					for(j=0;j<(COLUMN-1);j++)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，Right_Valid_Line++，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][irow]=j;
				}
			}
			else if((g_pix[irow][search_point_l]!=0)&&(g_pix[irow][search_point_r]==0)&&(g_pix[irow][COLUMN-1]>0))//赛道在视场右下角
			{
				for(i=12;i<41;i++)
				{
					if(g_pix[irow][i]>0)
					{
						search_point_r=i;
						break;
					}
				}
				if(i<41)
				{
					for(i=search_point_l;i>0;i--)
					{
						if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
						{
							break;
						}		
					}
					BlackLine[0][irow]=i;//左线最小为0
					for(j=search_point_r;j<(COLUMN-1);j++)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][irow]=j;//右线最大为81
					if((BlackLine[0][irow]-BlackLine[1][irow])>8)
					{
						BlackLine[0][irow]=-1;
						BlackLine[0][irow]=ROW;
					}
				}
				else
				{
					BlackLine[1][irow]=(COLUMN-1);
					for(j=(COLUMN-1);j>0;j--)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，Right_Valid_Line++，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[0][irow]=j;
				}
			}
			else//左右搜索点均为黑
			{
				search_point_l=search_point_l+5;
				search_point_r=search_point_r-5;

				if((g_pix[irow][search_point_l]==0)&&(g_pix[irow][search_point_r]!=0))
				{
					for(i=search_point_l;i>41;i--)
					{
						if(g_pix[irow][i]>0)
						{
							search_point_l=i;
							break;
						}
					}
					for(i=search_point_l;i>0;i--)
					{
						if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
						{
							break;
						}		
					}
					BlackLine[0][irow]=i;//左线最小为0
					for(j=search_point_r;j<(COLUMN-1);j++)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][irow]=j;//右线最大为81

					if((BlackLine[0][irow]-BlackLine[1][irow])>8)
					{
						BlackLine[0][irow]=-1;
						BlackLine[1][irow]=COLUMN;
					}
				}
				else if((g_pix[irow][search_point_l]!=0)&&(g_pix[irow][search_point_r]==0))
				{
					for(i=search_point_r;i<41;i++)
					{
						if(g_pix[irow][i]>0)
						{
							search_point_r=i;
							break;
						}
					}
					for(i=search_point_l;i>0;i--)
					{
						if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
						{
							break;
						}		
					}
					BlackLine[0][irow]=i;//左线最小为0
					for(j=search_point_r;j<(COLUMN-1);j++)
					{
						if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][irow]=j;//右线最大为81
					if((BlackLine[0][irow]-BlackLine[1][irow])>8)
					{
						BlackLine[0][irow]=-1;
						BlackLine[1][irow]=COLUMN;
					}
				}
				else
				{
					BlackLine[0][irow]=-1;
					BlackLine[1][irow]=COLUMN;
				}
			}
		}
		else
		{
			BlackLine[0][irow]=-1;
			BlackLine[1][irow]=COLUMN;
		}

		if((BlackLine[0][irow]==(-1))&&(BlackLine[1][irow]==COLUMN))
		{
			//判断是否为黑色起始行
			BlackRowFlag=1;
			if(BlackRow_Start_Found_Flag==0)
			{
				if(irow==ROW)
				{
					BlackRow_Start=irow;
					BlackRow_Start_Found_Flag=1;
				}
				else if(PreBlackRowFlag==0)
				{
					BlackRow_Start=irow;
					BlackRow_Start_Found_Flag=1;
				}
			}
		}
	}
	else//左右搜索点均不为黑的情况
	{
		BlackRowFlag=0;
		if((BlackRow_End_Found_Flag==0)&&(BlackRow_Start_Found_Flag==1)&&(PreBlackRowFlag==1))
		{
			BlackRow_End=irow+1;
			BlackRow_End_Found_Flag=1;
		}
		for(i=search_point_l;i>0;i--)
		{
			if(g_pix[irow][i]==0)//从中间向左搜索到黑线，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
			{
				break;
			}		
		}
		BlackLine[0][irow]=i;//左线最小为0
		for(j=search_point_r;j<(COLUMN-1);j++)
		{
			if(g_pix[irow][j]==0)//从中间向右搜索到黑线，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
			{
				break;
			}			
		}
		BlackLine[1][irow]=j;//右线最大为81
		//J_S_Flag=JudgeStraightFlag(irow,search_point_l,search_point_r);
	}
	PreBlackRowFlag=BlackRowFlag;
}

byte JudgeStraightFlag(byte irow,signed char search_point_l,signed char search_point_r)
{
	byte Left_Flag=0;
	byte Right_Flag=0;
	byte search_row;

	byte Start_Row_l=0,Start_Row_r=0;
	byte End_Row_l,End_Row_r;
	byte i;
	float delta,delta_y;
	byte delta_y_c;

	byte BR_num_l=0,BR_num_r=0;

	
	if((irow==ROW)||(irow<45))//远处不判断  弯道近处出现直角标志位 不去理他
	{
		return 0;
	} 
	if((irow<=60)&&(((CenterLine[irow+1]<=0)||(CenterLine[irow+1]>=(COLUMN-1)))&&(CenterLine[irow+1]!=120)))
	{
		return 0;
	}

	search_row=irow;
	for(i=0;i<6;i++)
	{
		if(g_pix[search_row-i][search_point_l]!=0)
			continue;
		Start_Row_l=search_row-i;
		Left_Flag=1;
		break;
	}
	for(i=0;i<6;i++)
	{
		if(g_pix[search_row-i][search_point_r]!=0)
			continue;
		Start_Row_r=search_row-i;
		Right_Flag=1;
		break;
	}

	if((Left_Flag==1)&&(Right_Flag==0))//有可能一边已经检测到黑点但是另一边还没检测到 实际是直角标识的情况
	{
		search_row=Start_Row_l;
		for(i=0;i<10;i++)
		{
			if(g_pix[search_row-i][search_point_r]!=0)
				continue;
			Start_Row_r=search_row-i;
			Right_Flag=1;
			break;
		}
	}
	else if((Left_Flag==0)&&(Right_Flag==1))
	{
		search_row=Start_Row_r;
		for(i=0;i<10;i++)
		{
			if(g_pix[search_row-i][search_point_l]!=0)
				continue;
			Start_Row_l=search_row-i;
			Left_Flag=1;
			break;
		}
	}
	if((Left_Flag==0)||(Right_Flag==0))
	{
		return 0;
	}

	BlackRow_Start_Found_Flag=1;//找到全黑行
	BlackRow_Start=Start_Row_l;//起始行

	if(Start_Row_l<Start_Row_r)
	{
		BlackRow_Start=Start_Row_r;
	}

	delta=(Start_Row_r-Start_Row_l)*1.0/(search_point_l-search_point_r);//转化为一个小数
	if(delta>0)//赛道向左偏 处理search_point_l
	{
		delta_y=3.2*delta+0.335;//线性化意淫偏移量 见图1-1
		delta_y_c=(unsigned char)delta_y;//转化为整形
		search_point_l=search_point_l-delta_y_c*15;
		End_Row_l=Start_Row_l-15;
		if(g_pix[End_Row_l][search_point_l]==0)//如果移动后的搜索点是黑点
		{
			search_point_l=search_point_l-delta_y_c*15/2;
			End_Row_l=Start_Row_l-20;
			if((g_pix[End_Row_l][search_point_r]==0)&&(g_pix[End_Row_l][search_point_l]==0))//如果移动后的搜索点是黑点
			{
				BlackRow_Start_Found_Flag=0;//找到全黑行
				BlackRow_Start=ROW;//起始行
				return 0;
			}
		}
		for(irow=Start_Row_l;irow>End_Row_l;irow--)//处理上坡的那种奇怪类直角标识图像
		{
			if(g_pix[irow][search_point_l]==0)
			{
				BR_num_l++;
			}
			else
			{
				break;
			}
		}
		if(BR_num_l<7)
		{
			BlackRow_Start_Found_Flag=0;//找到全黑行
			BlackRow_Start=ROW;//起始行
			return 0;
		}

		Jump_Row=End_Row_l;
		BlackRow_End=End_Row_l;

		Jump_S_P_l=search_point_l;
		Jump_S_P_r=search_point_r;
	}
	else if(delta<0)
	{
		delta_y=-3.2*delta+0.335;
		delta_y_c=(unsigned char)delta_y;
		search_point_r=search_point_r+delta_y_c*15;
		End_Row_r=Start_Row_r-15;
		if(g_pix[End_Row_r][search_point_r]==0)//如果移动后的搜索点是黑点
		{
			search_point_r=search_point_r+delta_y_c*15/2;
			End_Row_r=Start_Row_r-20;
			if((g_pix[End_Row_r][search_point_r]==0)&&(g_pix[End_Row_r][search_point_l]==0))//如果移动后的搜索点是黑点
			{
				BlackRow_Start_Found_Flag=0;//找到全黑行
				BlackRow_Start=ROW;//起始行
				return 0;
			}
		}
		for(irow=Start_Row_r;irow>End_Row_r;irow--)
		{
			if(g_pix[irow][search_point_r]==0)
			{
				BR_num_r++;
			}
			else
			{
				break;
			}
		}
		if(BR_num_r<7)
		{
			BlackRow_Start_Found_Flag=0;//找到全黑行
			BlackRow_Start=ROW;//起始行
			return 0;
		}
		Jump_Row=End_Row_r;
		BlackRow_End=End_Row_r;

		Jump_S_P_l=search_point_l;
		Jump_S_P_r=search_point_r;
	}
	else//(delta==0)
	{
		if((g_pix[BlackRow_Start-15][search_point_r]==0)
			&&(g_pix[BlackRow_Start-15][search_point_l]==0))//如果移动后的搜索点是黑点
		{
			BlackRow_Start_Found_Flag=0;//找到全黑行
			BlackRow_Start=ROW;//起始行
			return 0;
		}
		for(irow=BlackRow_Start;irow>BlackRow_Start-15;irow--)//处理上坡的那种奇怪类直角标识图像
		{
			if(g_pix[irow][search_point_l]==0)
			{
				BR_num_l++;
			}
			else
			{
				break;
			}
		}
		for(irow=BlackRow_Start;irow>BlackRow_Start-15;irow--)
		{
			if(g_pix[irow][search_point_r]==0)
			{
				BR_num_r++;
			}
			else
			{
				break;
			}
		}

		if((BR_num_r<7)&&(BR_num_r<7))
		{
			BlackRow_Start_Found_Flag=0;//找到全黑行
			BlackRow_Start=ROW;//起始行
			return 0;
		}


		Jump_Row=BlackRow_Start-15;
		BlackRow_End=BlackRow_Start-15;

		Jump_S_P_l=search_point_l;
		Jump_S_P_r=search_point_r;
	}
	//BlackRow_Start=irow;
	return 1;
}


/**************************************************
函数功能：判断该行为单线还是双线
函数输入：BlackLine[][]  irow  point_start_pre
函数输出：findline_flag
说明：根据BlackLine[][]的大小来判断单双线
***************************************************/
byte Findline_type(byte irow)
{
//	if(BlackLine[0][irow]<BlackLine[1][irow])//正常
//	{
//		return 0;
//	}
//	else 
//	{
//		return 1;
//	}
	
	if(BlackLine[0][irow]<BlackLine[1][irow])//正常
	{
		return 0;
	}
	else if((BlackLine[0][irow]-BlackLine[1][irow])<8)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**************************************************
函数功能：计算每行(双线)搜索的起始点
函数输入：BlackLine[][]  irow  point_start_pre
函数输出：point_start
说明：
***************************************************/
//void Findpoint_start_double(byte irow)//计算每行(双线)搜索的起始点
//{
//	if(BlackLine[0][irow]==-1&&BlackLine[1][irow]==COLUMN)//最近行左和右搜索点为黑，其余行左或右搜索点为黑，不找中线																
//	{
//		delta_l=10;
//		delta_r=10;
//	}
//	else if(BlackLine[0][irow]==0&&BlackLine[1][irow]==(COLUMN-1))//全白行																	//只找到一边 flags=2
//	{
//		if(irow<20)
//		{
//			//CenterLine[irow]=120;					//防止赛道背景为白，不找中线
//		}
//		else
//		{
//			CenterLine[irow]=41;
//		}
//		delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])/4;
//		delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])/4;
////		if(irow<60)
////		{
//			CrossNum++;	
////		}		
//	}
//	else if(BlackLine[0][irow]!=0&&BlackLine[1][irow]==(COLUMN-1))//左边有线 右边没线																	//只找到一边 flags=2
//	{
//		CenterLine[irow]=BlackLine[0][irow]+width[irow]/2;					//以一条边为准平移
//		delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])/4;
//		delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])/4;
////		if(irow<60)
////		{
//			OnlyRightWhiteRow++;
////		}	
//	}
//	else if(BlackLine[0][irow]==0&&BlackLine[1][irow]!=(COLUMN-1))//右边有线 左边没线
//	{
//		CenterLine[irow]=BlackLine[1][irow]-width[irow]/2;	
//		delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])/4;
//		delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])/4;	
////		if(irow<60)
////		{
//			OnlyLeftWhiteRow++;
////		}
//	}
//	else
//	{
//		CenterLine[irow]=(BlackLine[0][irow]+BlackLine[1][irow])/2;
//		delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])/4;
//		delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])/4;
//	}
//}

void Findpoint_start_double(byte irow)//计算每行(双线)搜索的起始点
{
	if(BlackLine[0][irow]==-1&&BlackLine[1][irow]==COLUMN)//最近行左和右搜索点为黑，其余行左或右搜索点为黑，不找中线																
	{
		if(irow>50)			{delta_l=15;delta_r=15;}
		else if(irow>30)	{delta_l=10;delta_r=10;}
		else				{delta_l=5;delta_r=5;}	
	}
	else if(BlackLine[0][irow]==0&&BlackLine[1][irow]==(COLUMN-1))//全白行																	//只找到一边 flags=2
	{
		CenterLine[irow]=41;
		if(irow>60)
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/4;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/4;
		}
		else
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/8;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/8;
		}
		CrossNum++;			
	}
	else if(BlackLine[0][irow]!=0&&BlackLine[1][irow]==(COLUMN-1))//左边有线 右边没线																	//只找到一边 flags=2
	{
		CenterLine[irow]=BlackLine[0][irow]+width[irow]/2;					//以一条边为准平移
		if(irow>60)
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/4;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/4;
		}
		else
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/8;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/8;
		}
		OnlyRightWhiteRow++;
	}
	else if(BlackLine[0][irow]==0&&BlackLine[1][irow]!=(COLUMN-1))//右边有线 左边没线
	{
		CenterLine[irow]=BlackLine[1][irow]-width[irow]/2;	
		if(irow>60)
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/4;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/4;
		}
		else
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/8;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/8;
		}
		OnlyLeftWhiteRow++;
	}
	else
	{
		CenterLine[irow]=(BlackLine[0][irow]+BlackLine[1][irow])/2;
		if(irow>60)
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/4;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/4;
		}
		else
		{
			delta_l=ABS(CenterLine[irow]-BlackLine[0][irow])*3/8;
			delta_r=ABS(BlackLine[1][irow]-CenterLine[irow])*3/8;
		}
	}
}


/**************************************************
函数功能：确定中黑线的中心点 
函数输入：irow  BlackLine[][]
函数输出：BlackLine[][] BlackCenterLine_Simple[]
说明：
***************************************************/
void Findpoint_start_simple(byte irow)//确定中黑线的中心点 补双黑线
{
	CenterLine[irow]=(BlackLine[0][irow]+BlackLine[1][irow])/2;	
	delta_l=15;
	delta_r=15;
	Findline_filldouble(CenterLine[irow],irow);//补双黑线
}

/**************************************************
函数功能：已确定中黑线的中心点 补双黑线
函数输入：irow  BlackCenterLine_Simple[]
函数输出：BlackLine[][] 
说明：
***************************************************/
void Findline_filldouble(signed char search_point,byte irow)//把单线转换为双线处理，并判断赛道类型
{
	signed char edge_num_l,edge_num_r;//左右边界点

	edge_num_l=search_point-width[irow]/2;//补左线
	if(edge_num_l<0)										
	{
		BlackLine[0][irow]=0;
	}
	else
	{
		BlackLine[0][irow]=search_point-width[irow]/2;
	}

	edge_num_r=search_point+width[irow]/2;//补右线
	if(edge_num_r>(COLUMN-1))									
	{
		BlackLine[1][irow]=(COLUMN-1);
	}
	else
	{
		BlackLine[1][irow]=search_point+width[irow]/2;
	}
}

/**************************************************
函数功能：寻找左右黑线起始点，用于赛道判断
函数输入：BlackLine[][] 
函数输出：StartRow[lr]，EndRow[lr] 
说明：
***************************************************/
void Pre_handle(byte lr)
{
	byte irowb=ROW;		//起点
	byte irowe=0;		//终点
	for(;irowe<irowb;++irowe)			//寻找终点
		if(BlackLine[lr][irowe]!=COLUMNS)
			break;
	for(;irowb>irowe;--irowb)				//寻找起点
		if(BlackLine[lr][irowb]!=COLUMNS)
			break;
	if(irowb>irowe){					//首次找线
		StartRow[lr]=irowb;
		EndRow[lr]=irowe;
	}
}

/**************************************************
函数功能：判断直角
函数输入：BlackLine[2][70]，RoadEnd
函数输出：flag_Rightangle_l(左直角标志)  flag_Rightangle_l(右直角标志)
说明：
***************************************************/
void Analyse_Rigntangle()
{
	byte i;
	LeftWhiteRow=0;
	RightWhiteRow=0;	

	if((flag_Rightangle_l==0)&&(flag_Rightangle_r==0))
	{
		//当RoadEnd在一定范围内时，判断RoadEnd以下左白行和右白行数目，预判断是左直角还是右直角
		if((RoadEnd>=10)&&(RoadEnd<=20))
		{
			for(i=RoadEnd;i<40;i++)
			{
				if(BlackLine[0][i]==0)
				{
					LeftWhiteRow++;
				}
				if(BlackLine[1][i]==(COLUMN-1))
				{
					RightWhiteRow++;
				}
			}
			if((LeftWhiteRow-RightWhiteRow)>10)
			{
				TurnLeft=1;
				TurnRight=0;
			}
			if((RightWhiteRow-LeftWhiteRow)>10)
			{
				TurnLeft=0;
				TurnRight=1;
			}
		}

		//当RoadEnd一半距离处赛道宽度大于65时判断为直角，此处RoadEnd的值要大于等于判断出直角时的Road值
		//if((RoadEnd>25)&&(ABS(BlackLine[0][(RoadEnd+ROW)/2]-BlackLine[1][(RoadEnd+ROW)/2])>65)&&(ABS(BlackLine[0][(RoadEnd+ROW)/2]-BlackLine[1][(RoadEnd+ROW)/2])<=(COLUMN-1))&&(Slope!=1))
		if((RoadEnd>25)&&(RoadEnd<40)&&(ABS(BlackLine[0][(RoadEnd+ROW)/2]-BlackLine[1][(RoadEnd+ROW)/2])>65)&&(ABS(BlackLine[0][(RoadEnd+ROW)/2]-BlackLine[1][(RoadEnd+ROW)/2])<=(COLUMN-1)))
		{
			if(TurnLeft)
			{
				flag_Rightangle_l=1;
				flag_Rightangle_r=0;
			}
			if(TurnRight)
			{
				flag_Rightangle_l=0;
				flag_Rightangle_r=1;
			}
		
		}	
	}

	//出直角判断
	if((RoadEnd<10)&&(flag_Rightangle_l||flag_Rightangle_r))
	{
		TurnLeft=0;
		TurnRight=0;
		flag_Rightangle_l=0;
		flag_Rightangle_r=0;
	}
}

/**************************************************
函数功能：找十字条件
函数输入：BlackLine[2][70]
函数输出：
说明：
***************************************************/
byte Re_Cross()
{	
	byte lr=0,irow=ROW;
	byte Row_Start[2]={ROW};
	byte Row_End[2]={ROW};

	byte Step_Row[2]={ROW};
	byte Step_Point[2]={0};

	byte Step_Row_f[2]={ROW};
	byte Step_Row_b[2]={ROW};

	byte unStep_Flag[2]={0};//无跳变标志

	for(lr=0;lr<2;lr++)
	{
		irow=ROW;
		while((BlackLine[0][irow]==0)||(BlackLine[0][irow]==COLUMN-1))
		{
			if(irow==25)
			{
				break;
			}
			irow--;
		}
		Row_Start[lr]=irow;
		while((BlackLine[0][irow]!=0)||(BlackLine[0][irow]!=COLUMN-1))
		{
			if(irow==25)
			{
				break;
			}
			irow--;
		}
		Row_End[lr]=irow;

	}

	//做一些判断
	for(lr=0;lr<2;lr++)
	{
		if(Row_Start[lr]==Row_End[lr])
		{
			unStep_Flag[lr]=1;
		}
	}

	for(lr=0;lr<2;lr++)
	{
		if(unStep_Flag[lr])
		{
			continue;
		}
		Step_Row[lr]=Row_Start[lr];
		Step_Point[lr]=BlackLine[lr][Row_Start[lr]];
		for(irow=Row_Start[lr];irow>(Row_End[lr]-1);irow--)
		{
			if((BlackLine[0][irow]>BlackLine[0][Step_Row[0]])
				||(BlackLine[1][irow]<BlackLine[1][Step_Row[1]]))
			{
				Step_Row[lr]=irow;
				Step_Point[lr]=BlackLine[lr][irow];
			}
		}
	}

	for(lr=0;lr<2;lr++)
	{
		if(unStep_Flag[lr])
		{
			continue;
		}
		Step_Row_f[lr]=Step_Row[lr]+5;
		if(Step_Row_f[lr]>Row_Start[lr])
		{
			Step_Row_f[lr]=Row_Start[lr];
		}
		Step_Row_b[lr]=Step_Row[lr]-5;
		if(Step_Row_b[lr]<Row_End[lr])
		{
			Step_Row_b[lr]=Row_End[lr];
		}
	}

	//判断十字 找那个突变的拐点 前面都是准备工作
	for(lr=0;lr<2;lr++)
	{
		if(unStep_Flag[lr])
		{
			continue;
		}
		if((Step_Point[lr]-BlackLine[lr][Step_Row_f[lr]])
			*(Step_Point[lr]-BlackLine[lr][Step_Row_b[lr]])>0)//变化趋势相反，有转折
		{
			if((Step_Point[lr]-BlackLine[lr][Step_Row_f[lr]])
			+(Step_Point[lr]-BlackLine[lr][Step_Row_b[lr]])>9)
			{
				return 1;
			}
		}
	}
	return 0;

}
/**************************************************
函数功能：加强正十字
函数输入：BlackLine[2][70]
函数输出：
说明：
***************************************************/
void Cross_Positive()
{
	byte lr,irow=ROW;
	byte Step_Row[2]={ROW};
	byte D_Value[2]={0};
	byte D_Value_Step=0;

	for(lr=0;lr<2;lr++)
	{
		irow=ROW;
		while((lr==0)&&(BlackLine[0][irow]==0))
		{
			irow--;
		}
		while((lr==1)&&(BlackLine[1][irow]==COLUMN-1))
		{
			irow--;
		}
		Step_Row[lr]=irow;
		D_Value[lr]=ABS(BlackLine[lr][Step_Row[lr]+1]-BlackLine[lr][Step_Row[lr]-2]);
	}
	D_Value_Step=ABS(BlackLine[0][Step_Row[0]]-BlackLine[1][Step_Row[1]]);

	if((D_Value[0]>8)&&(D_Value[0]<30)
		&&(D_Value[1]>8)&&(D_Value[1]<30)
		&&(D_Value_Step>40))
	{
		RoadType=Cross;
	}
	else
	{
		//RoadType=NoLine;
	}

}
/**************************************************
函数功能：检查前五行是否为全白行
函数输入：BlackLine[2][70]
函数输出：
说明：
***************************************************/
byte Check_First5row()
{
	byte irow,WriteRow=0;
	for(irow=ROW;irow>ROW-5;irow--)
	{
		if((BlackLine[0][irow]==0)&&(BlackLine[1][irow]>=COLUMN-1))
		{
			WriteRow++;
		}
	}
	if(WriteRow==5)
	{
		return 1;
	}
	else
	{
		return 0;	
	}
}

/**************************************************
函数功能：十字补线
函数输入：flag_Rightangle_l，flag_Rightangle_r，Slope(上坡标志)，CrossNum，BlackLine[2][70]，RoadEnd
函数输出：BlackLine[2][70]，CenterLine[70]
说明：CrossNum值可调
***************************************************/
void Cross_Fill(void)
{
	unsigned char Cross_Cout=0;
	unsigned char Cross_Start=ROWS;
	unsigned char Cross_End=ROWS;
	unsigned char Flag1=0;//十字标志位
	unsigned char Flag2=0;//凸字标志位
	unsigned char Flag3=0;//十字标志位
	unsigned char Flag4=0;//凸字标志位
	signed char Start=ROWS;
	signed char End=ROWS;
	float slope=0;
	signed char i,j,irow;
	byte Re_Cross_Flag=0;
	byte Check_First5row_Flag=0;

	if(BCL_Start==ROWS)
	{
		for(i=67;i>=4;i--)
		{
			if(((BlackLine[0][i+2]<BlackLine[0][i+1])&&(BlackLine[0][i+1]<BlackLine[0][i])&&(BlackLine[0][i]>BlackLine[0][i-1])&&(BlackLine[0][i-1]>BlackLine[0][i-2]))||((BlackLine[0][i+2]<BlackLine[0][i+1])&&(BlackLine[0][i+1]<BlackLine[0][i])&&(BlackLine[0][i]==BlackLine[0][i-1])&&(BlackLine[0][i-1]>BlackLine[0][i-2])&&(BlackLine[0][i-2]>BlackLine[0][i-3]))||((BlackLine[0][i+2]<BlackLine[0][i+1])&&(BlackLine[0][i+1]<BlackLine[0][i])&&(BlackLine[0][i]==BlackLine[0][i-1])&&(BlackLine[0][i-1]==BlackLine[0][i-2])&&(BlackLine[0][i-2]>BlackLine[0][i-3])&&(BlackLine[0][i-3]>BlackLine[0][i-4])))
			{
				Flag3=1;
				break;
			}
			if(((BlackLine[1][i+2]>BlackLine[1][i+1])&&(BlackLine[1][i+1]>BlackLine[1][i])&&(BlackLine[1][i]<BlackLine[1][i-1])&&(BlackLine[1][i-1]<BlackLine[1][i-2]))||((BlackLine[1][i+2]>BlackLine[1][i+1])&&(BlackLine[1][i+1]>BlackLine[1][i])&&(BlackLine[1][i]==BlackLine[1][i-1])&&(BlackLine[1][i-1]<BlackLine[1][i-2])&&(BlackLine[1][i-2]<BlackLine[1][i-3]))||((BlackLine[1][i+2]>BlackLine[1][i+1])&&(BlackLine[1][i+1]>BlackLine[1][i])&&(BlackLine[1][i]==BlackLine[1][i-1])&&(BlackLine[1][i-1]==BlackLine[1][i-2])&&(BlackLine[1][i-2]<BlackLine[1][i-3])&&(BlackLine[1][i-3]<BlackLine[1][i-4])))
			{
				Flag4=1;
				break;
			}
		}
	}

	if(((Flag3==1)&&(Flag4==0))||((Flag3==0)&&(Flag4==1)))
	{
		RoadEnd=i;
	}
	else
	{
		if((!(flag_Rightangle_l||flag_Rightangle_r))&&(Slope==0)&&(CrossNum>=8))
		{
			RoadType=Cross;
			Check_First5row_Flag=Check_First5row();
			if(Check_First5row_Flag)
			{
				Cross_Positive();
			}
		}
	}

	if(RoadType==Cross)
	{
		//补左边十字
		for(i=ROW;i>8;i--)//参数i=8可改（CrossEnd极限值）
		{
			if(BlackLine[0][i]!=(-1)&&BlackLine[0][i]!=0&&(ABS(BlackLine[0][i]-BlackLine[0][i-1])<=5))
			{
				Cross_Start=i;
				for(i=Cross_Start-1;i>8;i--)
				{
					if(BlackLine[0][i]!=(-1)&&BlackLine[0][i]!=0&&(ABS(BlackLine[0][i]-BlackLine[0][i-1])<=2))
					{
						Cross_Cout++;
					}
					else
					{//若在这个范围内没有检测到十字出口，则将i=9定为Cross_End
						Cross_End=i;
						break;
					}
				}
				if(i<=8)
				{//如果十字入口太远，则将出入口统一，即暂不处理十字
					Cross_End=i;
				}
				if(Cross_Cout>8)//此参数可改
				{//检测十字到一定范围内时，开始处理十字
					Flag1=1;
					break;
				}
				else
				{
					Cross_Cout=0;
					continue;
				}
			}
		}
		
		if(Flag1)
		{//处理十字
			if(Cross_Start==ROW)
			{
				Flag2=1;//判断不为凸字
			}
			Cross_End=(Cross_Start+Cross_End)/2;
			Cross_Start=ROW;
			if(BlackLine[0][i]==(-1)||BlackLine[0][ROW]==0)
			{//如果左黑线在视频边界或已出边界
				Start=BlackLine[0][ROW]-(81-BlackLine[1][ROW]);//将右边多出的量补到左边
			}
			else
			{
				Start=BlackLine[0][ROW];
			}
			slope=((float)BlackLine[0][Cross_End]-(float)Start)/((float)Cross_Start-(float)Cross_End);
			for(i=Cross_End;i<ROWS;i++)
			{
				BlackLine[0][i]=BlackLine[0][Cross_End]-(signed char)(slope*(i-Cross_End));
			}
			if(Flag2)
			{
				for(i=Cross_End;i>20;i--)
				{
					BlackLine[0][i]=BlackLine[0][Cross_End]+(signed char)(slope*(Cross_End-i));
				}
			}
		}
		
		Flag1=0;
		Flag2=0;
		Cross_Cout=0;
		//补右边十字
		for(i=ROW;i>8;i--)
		{
			if(BlackLine[1][i]!=(COLUMN-1)&&BlackLine[1][i]!=COLUMN&&(ABS(BlackLine[1][i]-BlackLine[1][i-1])<=2))
			{
				Cross_Start=i;
				for(i=Cross_Start-1;i>8;i--)
				{
					if(BlackLine[1][i]!=(COLUMN-1)&&BlackLine[1][i]!=COLUMN&&(ABS(BlackLine[1][i]-BlackLine[1][i-1])<=2))
					{
						Cross_Cout++;
					}
					else
					{
						Cross_End=i;
						break;
					}
				}
				if(i<=8)
				{
					Cross_End=i;
				}
				if(Cross_Cout>8)
				{
					Flag1=1;
					break;
				}
				else
				{
					Cross_Cout=0;
					continue;
				}
			}
		}
		
		if(Flag1)
		{
			if(Cross_Start==ROW)
			{
				Flag2=1;
			}
			Cross_End=(Cross_Start+Cross_End)/2;
			Cross_Start=71;
			if(BlackLine[1][i]==(COLUMN-1)||BlackLine[1][ROW]==COLUMN)
			{
				Start=BlackLine[1][ROW]+BlackLine[0][ROW];
			}
			else
			{
				Start=BlackLine[1][ROW];
			}
			slope=((float)BlackLine[1][Cross_End]-(float)Start)/((float)Cross_Start-(float)Cross_End);
			for(i=Cross_End;i<ROWS;i++)
			{
				BlackLine[1][i]=BlackLine[1][Cross_End]-(signed char)(slope*(i-Cross_End));
			}
			if(Flag2)
			{
				for(i=Cross_End;i>20;i--)
				{
					BlackLine[1][i]=BlackLine[1][Cross_End]+(signed char)(slope*(Cross_End-i));
				}
			}
		}

		for(irow=ROW;irow>=RoadEnd;irow--)
		{
			Findpoint_start_double(irow);
		}
		
		//修正错误十字补线
		if((CenterLine[RoadEnd]<20)||(CenterLine[RoadEnd]>60))
		{
			for(irow=ROW;irow>=RoadEnd;irow--)
			{
				CenterLine[irow]=CenterLine[ROW];
			}
		}
	}
}

/**************************************************
函数功能：判断上下坡
函数输入：
函数输出：
说明：直接使用学长程序，可准确判断出上坡，但不能判断出下坡
***************************************************/
void DetectSlope()
{
	if(Slope==0)	//正常赛道
	{
		if(detected==0)	//检测坡道前
		{
			DetectUpSlope();//检测上坡标志
			if(UpSlope==1&&PastUpSlope==1&&PPastUpSlope==1&&PPPastUpSlope==1)//连续三场上坡标志认为检测到坡道
			{
				Slope=1;
				detected=1;			
			}
			else	//记录上坡标志
			{
				PPPastUpSlope=PPastUpSlope;
				PPastUpSlope=PastUpSlope;
				PastUpSlope=UpSlope;
			}		
		}
		else	//下坡后直道
		{
			detect_slope_delay++;
			if(detect_slope_delay==35)//延时40场开始下一次坡道检测
			{
				detect_slope_delay=0;
				detected=0;
			}
		}
	}
	else if(Slope==1)	//检测到上坡后
	{
		slope_delay++;
		if(slope_delay<42)
		{
			DetectDownSlope();	//检测下坡标志
			if(PPastDownSlope==1&&PastDownSlope==1&&DownSlope==0)	//连续3场下坡标志认为到了下坡
			{
				Slope=2;
				PastUpSlope=0;
				PPastDownSlope=0;
			}
			else	//记录下坡标志
			{
				PPastDownSlope=PastDownSlope;
				PastDownSlope=DownSlope;	
			}
		}
		else
		{
			//Slope=0;
			PastUpSlope=0;
			PPastUpSlope=0;
			PPPastUpSlope=0;
			PastDownSlope=0;
			PPastDownSlope=0;
			slope_delay=0;
		}
	}
	else if(Slope==2)	//下坡
	{
		detect_slope_delay++;
		if(detect_slope_delay>18)	//20场之后自动认为离开坡道
		{
			//Slope=0;
			PastUpSlope=0;
			PPastUpSlope=0;
			PPPastUpSlope=0;
			PastDownSlope=0;
			PPastDownSlope=0;
			slope_delay=0;
		}
		else if(detect_slope_delay>8)	//延时10场检测离开坡道
		{
			DetectUpSlope();	//离开坡道时有上坡标志
			if(PastUpSlope==1&&UpSlope==0)	//连续两场检测到上坡标志
			{
				//Slope=0;	//离开坡道
				PastUpSlope=0;
				PPastUpSlope=0;
				PPPastUpSlope=0;
				PastDownSlope=0;
				PPastDownSlope=0;
				slope_delay=0;
			}
			else	PastUpSlope=UpSlope;
		}
	}
}
void DetectUpSlope()
{
	byte irowend,fupslope=0;
	//把无效线和小S踢出去
	if(RoadType==SmallS||RoadType==NoLine)
	{
		UpSlope=0;
		return;
	}
	//把虚线小S踢出去
	if((SegNum[0]+SegNum[1])>3||left_d[0]>=5||right_d[1]>=5)
	{
		UpSlope=0;
		return;
	}
	if(RoadEnd>15)
	{
		UpSlope=0;
		return;
	}
	if(StartRow[0]==ROWS||StartRow[1]==ROWS)//左线或右线没找到线
	{
		UpSlope=0;
		return;
	}
	if(EndRow[0]<2&&EndRow[1]<2)
	{
		irowend=MAX(EndRow[1],EndRow[0]);
		if(BlackLine[1][irowend]-BlackLine[0][irowend]>28)
			fupslope=1;
	}
	if(fupslope==1)
		UpSlope=1;
	else
		UpSlope=0;
}
void DetectDownSlope()
{
	byte irowe,irow,irowb,fdownslope=0,countnum=0;
	irowe=MAX(EndRow[0],EndRow[1]);
	irowb=MIN(StartRow[0],StartRow[1]);
	if(irowb<=40)
		fdownslope=0;
	else if(irowe<10)
	{
		for(irow=irowe;irow<=30;irow++)
		{
			if(BlackLine[1][irow]-BlackLine[0][irow]<15+irow/3)
				countnum++;
		}
		if(countnum>8)
			fdownslope=1;
	}
	else if(irowe<20)
	{
		if(ABS(CenterLine[irowe]-COLUMN1_2)<6&&ABS(CenterLine[irowb]-COLUMN1_2)<6)
			fdownslope=1;
	}
	if(fdownslope==1)
		DownSlope=1;
	else
		DownSlope=0;
}

/**************************************************
函数功能：判断赛道类型
函数输入：
函数输出：RoadType
说明：
***************************************************/
void AnalyzeRoadType()
{
	AnalyzeRoad();
	if(RoadType!=NoLine)
	{
		if(RoadType==Cross)
		{
			if(!JudgeStraight())
			{
				if(!JudgeSmallS())
				{
					if(!JudgeToT())
					{
						//if(!JudgeUTurn())//判断UTurn后会过多舍弃远处赛道
						//{
							JudgeGeneral();
						//}
					}
				}
			}
		}
		else
		{
			//if(!JudgeBCL())
			//{				
				if(!JudgeBarrier())
				{
					if(!JudgeStraight())
					{
						if(!JudgeSmallS())
						{
							if(!JudgeToT())
							{
								//if(!JudgeUTurn())
								//{
									JudgeGeneral();
								//}
							}
						}
					}					
				}
			//}
			//else
			//{
			//	if(!JudgeStraight())
			//	{
			//		if(!JudgeSmallS())
			//		{
			//			if(!JudgeToT())
			//			{
			//				//if(!JudgeUTurn())
			//				//{
			//					JudgeGeneral();
			//				//}
			//			}
			//		}
			//	}
			//}
		}
	}
}

/**************************************************
函数功能：检测障碍物
函数输入：LeftBlackRow，RightBlackRow，BlackLine[][]，CenterLine[]
函数输出：BlackLine[][]，CenterLine[]，RoadType
说明：
***************************************************/
byte JudgeBarrier()
{
	signed char i,m,n,o,p,gg,cc;
	byte Flag1=0;//控制语句辅助标志位
	byte Flag2=0;//控制语句辅助标志位
	byte Flag3=0;//控制语句辅助标志位
	byte Count1=0;//左障碍线数
	byte Count2=0;//右障碍线数

	if((LeftBlackRow<3)&&(RightBlackRow>5))//障碍在视场右侧
	{
		if(CenterLine[ROW]==120)//障碍在视场下方
		{
			for(i=ROW;i>40;i--)//从最近行到第40行补线，此处40可能有问题
			{
				if(CenterLine[i]==120)//只在未找中线的地方补线，以35为中心搜索左右线
				{
					for(m=35;m>0;m--)
					{
						if(g_pix[i][m]==0)
						{
							break;
						}		
					}
					BlackLine[0][i]=m;
					for(n=35;n<(COLUMN-1);n++)
					{
						if(g_pix[i][n]==0)
						{
							break;
						}			
					}
					BlackLine[1][i]=n;
					CenterLine[i]=(BlackLine[0][i]+BlackLine[1][i])/2;
				}
			}
		}
		else//障碍在视场中间
		{
			for(i=ROW;i>RoadEnd;i--)
			{
				if(CenterLine[i]==120)
				{
					for(m=CenterLine[i+1];m>0;m--)//以上一行中点为中心搜索左右线
					{
						if(g_pix[i][m]==0)
						{
							break;
						}		
					}
					BlackLine[0][i]=m;
					for(n=35;n<(COLUMN-1);n++)
					{
						if(g_pix[i][n]==0)
						{
							break;
						}			
					}
					BlackLine[1][i]=n;
					CenterLine[i]=(BlackLine[0][i]+BlackLine[1][i])/2;
				}
			}
		}
		RoadType=Barrier1;//左或右搜索点进入障碍物的情况（此处为左搜索点）
//		D0=0;
	}
	else if((LeftBlackRow>5)&&(RightBlackRow<3))//障碍在视场左侧
	{
		if(CenterLine[ROW]==120)
		{
			for(i=ROW;i>40;i--)
			{
				if(CenterLine[i]==120)
				{
					for(m=45;m>0;m--)
					{
						if(g_pix[i][m]==0)
						{
							break;
						}		
					}
					BlackLine[0][i]=m;
					for(n=45;n<(COLUMN-1);n++)
					{
						if(g_pix[i][n]==0)
						{
							break;
						}			
					}
					BlackLine[1][i]=n;
					CenterLine[i]=(BlackLine[0][i]+BlackLine[1][i])/2;
				}
			}
		}
		else
		{
			for(i=ROW;i>RoadEnd;i--)
			{
				if(CenterLine[i]==120)
				{
					for(m=CenterLine[i+1];m>0;m--)
					{
						if(g_pix[i][m]==0)//从中间向左搜索到黑线，Left_Valid_Line++，并记录第n行向左黑色起始列i，即为Black_Line[1][n]
						{
							break;
						}		
					}
					BlackLine[0][i]=m;//左线最小为0
					for(n=35;n<(COLUMN-1);n++)
					{
						if(g_pix[i][n]==0)//从中间向右搜索到黑线，Right_Valid_Line++，并记录第n行向右黑色起始列j，即为Black_Line[2][n]
						{
							break;
						}			
					}
					BlackLine[1][i]=n;//右线最大为81
					CenterLine[i]=(BlackLine[0][i]+BlackLine[1][i])/2;
				}
			}
		}
		RoadType=Barrier1;//左或右搜索点进入障碍物的情况（此处为右搜索点）
//		D0=0;
	}
	else//左右搜索点均错过障碍物的情况，中线与没有障碍物时一样
	{
		for(i=ROW;i>=RoadEnd;i--)
		{
			if((CenterLine[i]>0)&&(CenterLine[i]<(COLUMN-1)))
			{
				Flag1=0;
				Flag2=0;
				Flag3=0;
				for(m=CenterLine[i];m>0;m--)//从中点向左搜索，是否存在变黑和变白两次跳变，且跳变点相距较近，若存在，Count1++
				{
					if(g_pix[i][m]==0)
					{
						for(n=m-1;n>0;n--)
						{
							if(g_pix[i][n]>0)
							{
								if(((ABS(m-n)>5)&&(ABS(m-n)<25))&&(n>1)&&(ABS(n-CenterLine[i])<(width[i]*3/4))&&(i>20))
								{
									BlackLine[0][i]=m;
									Flag3=1;
									Count1++;
								}
								Flag1=1;
								break;
							}
						}
						if(Flag1)
							break;
					}	
				}
				for(m=CenterLine[i];m<(COLUMN-1);m++)//从中点向右搜索，是否存在变黑和变白两次跳变，且跳变点相距较近，若存在，Count2++
				{
					if(g_pix[i][m]==0)
					{
						for(n=m+1;n<(COLUMN-1);n++)
						{
							if(g_pix[i][n]>0)
							{
								if(((ABS(m-n)>5)&&(ABS(m-n)<25))&&(n<80)&&(ABS(n-CenterLine[i])<(width[i]*3/4))&&(i>20))
								{
									BlackLine[1][i]=m;
									Flag3=1;
									Count2++;
								}
								Flag2=1;
								break;	
							}	
							if(Flag2)
								break;
						}
					}		
				}
				if(Flag3)
				{					
					CenterLine[i]=(BlackLine[0][i]+BlackLine[1][i])/2;
				}
			}
		}
		//dll_road_condition[1]=Count1;
		//dll_road_condition[2]=Count2;
		if((Count1>1)||(Count2>1))
		{
			RoadType=Barrier2;//左右搜索点均错过障碍物的情况判断出的障碍
			D1=0;
		}
	}
	o=0;
	p=0;
#if 1           //道路类型66  判断方法      jqy
	for(i=ROW;i>=0;i--)
	{
		for(n=20;n<COLUMN-1;n++)
		{
			if(g_pix[i][n-1]!=0 && g_pix[i][n]==0)
			{
				break;
			}			
		}
		BlackLine[1][i]=n;
	}
#endif
#if 1
	while(BlackLine[1][o]==COLUMN-1)
	{
		BlackLine[1][o]=0;
		o++;
	}
//	gg=0;
//	if(o<14)
//		o=14;
//	BlackLine[1][o-5]=0;BlackLine[1][o-4]=0;BlackLine[1][o-3]=0;BlackLine[1][o-2]=0;BlackLine[1][o-1]=0;
	for(i=o;i<ROW;i++)
	{
	    if(BlackLine[1][i-3]>BlackLine[1][i])
	    {
	    	if(p==0)
	    		gg=i;
	    	p++;
	    }
	    if(BlackLine[1][i]>BlackLine[1][i-3] && p>2)
	    	if(i-gg>8)
	        {
	    	    p=100;
	    	    break;
	        }
	}
	if(p==100)
		RoadType=66;

	cc=0;
	for(i=ROW;i>=12;i--)          //道路类型88  判断方法      jqy
	{
		gg=0;
		for(n=0;n<COLUMN-1;n++)
			if(g_pix[i][n]!=0)		
				gg++;
		if(g_pix[i][gg-1]!=0 && g_pix[i][gg]==0)
			gg=0;
		if(gg<25 && gg>5)
		{
			cc++;
			LCD_Write_Num(105,3,gg,4);
		}
	}
	if(cc>8)
		RoadType=88;
#endif
//	LCD_Write_Num(105,3,p,3);
	if((RoadType==Barrier1)||(RoadType==Barrier2))
		return 1;
	else if(RoadType==66)
		return 1;
	else if(RoadType==88)
		return 1;
	else	
		return 0;	
}

/**************************************************
函数功能：判断是处于中黑线
函数输入：BCL_Start，BCL_End
函数输出：
说明：若判断为中黑心就不检测障碍物，防止干扰，ABS(BCL_Start-RoadEnd)可调
***************************************************/
byte JudgeBCL()
{
	if(((BCL_Start!=ROWS)||(BCL_End!=0))&&(ABS(BCL_Start-RoadEnd)>20))
		return 1;
	else								
		return 0;

}

/**************************************************
函数功能：
函数输入：
函数输出：
说明：以下为学长程序，先分析赛道，再判断赛道类型
***************************************************/
void AnalyzeRoad()
{
	byte i,j,k,irow,irowb,irowe,flags;

	if((StartRow[0]==ROWS&&StartRow[1]==ROWS)||(EndRow[0]>60&&EndRow[1]>60)||(RoadEnd>60)||(CrossNum>50))
	{
		RoadType=NoLine;
		return;
	}
	if(StartRow[0]-EndRow[0]<5)
		LineType[0]=UnExist;
	else
		LineType[0]=Exist;
	if(StartRow[1]-EndRow[1]<5)
		LineType[1]=UnExist;
	else
		LineType[1]=Exist;
	//计算三根线的向左、向右、向中
	left_d[0]=0;	right_d[0]=0;	mid_d[0]=0;		
	left_d[1]=0;	right_d[1]=0;	mid_d[1]=0;		
	left_d[2]=0;	right_d[2]=0;	mid_d[2]=0;	
	for(irow=RoadStart;irow>RoadEnd;irow--)
		if(CenterLine[irow]==CenterLine[irow-1])
			mid_d[2]++;
		else if(CenterLine[irow]>CenterLine[irow-1])
			left_d[2]++;
		else
			right_d[2]++;
	if(LineType[0]==Exist)
	{
		for(irow=StartRow[0];irow>EndRow[0];irow--)
			if(BlackLine[0][irow]==BlackLine[0][irow-1])
				mid_d[0]++;
			else if(BlackLine[0][irow]>BlackLine[0][irow-1])
				left_d[0]++;
			else
				right_d[0]++;
	}
	if(LineType[1]==Exist)
	{
		for(irow=StartRow[1];irow>EndRow[1];irow--)
			if(BlackLine[1][irow]==BlackLine[1][irow-1])
				mid_d[1]++;
			else if(BlackLine[1][irow]>BlackLine[1][irow-1])
				left_d[1]++;
			else
				right_d[1]++;
	}
	
	//统计起点、拐点、终点
	for(i=0;i<10;i++)
		turn[i]=0;
	count=1;
	turn[0]=RoadStart;
	irow=RoadStart;
	irowe=RoadEnd;
	while(CenterLine[irow]==CenterLine[irow-1]&&irow>irowe)
		irow--;
	if(CenterLine[irow]>CenterLine[irow-1])//向左
		flags=1;		//1:表示向左
	else
		flags=0;
	while(irow>irowe)
	{
		if(flags==1)
		{
			if(CenterLine[irow]<CenterLine[irow-1])
			{
				flags=0;
				turn[count]=irow;//运行此语句后BlackRow_Start和BlackRow_End变化，没找到原因
				if(count<9)
				{
					count++;
				}				
			}
		}
		else
		{
			if(CenterLine[irow]>CenterLine[irow-1])
			{
				flags=1;
				turn[count]=irow;
				if(count<9)
				{
					count++;
				}
			}
		}
		irow--;
	}
	turn[count]=RoadEnd;
	if(count<9)
	{
		count++;
	}
}

byte JudgeStraight()
{
	byte irowe;
	irowe=MAX(EndRow[0],EndRow[1]);
	if(EndRow[0]<25&&EndRow[1]<25)	//包含直道、小S
	{
		if(ABS((BlackLine[0][irowe]+BlackLine[1][irowe])/2-COLUMN1_2)<15)//远处赛道中心在市场中心
		{
			if(left_d[0]<3&&right_d[1]<3&&SegNum[0]==1&&SegNum[1]==1)//直道类型
			//if(left_d[0]<3&&right_d[1]<3)//直道类型
			{
				RoadType=Straight;
				return 1;
			}
		}
	}
	return 0;
}
byte JudgeSmallS()
{
	byte i;
	byte left_m=83,right_m=0;
	for(i=RoadEnd;i<=69;i++)
	{
		if(CenterLine[i]<left_m)
			left_m=CenterLine[i];
		if(CenterLine[i]>right_m)
			right_m=CenterLine[i];
	}
	if((right_m-left_m<=40)&&(RoadEnd<10)&&(CrossNum<5))
	{
		RoadType=SmallS;
		return 1;
	}
	else
	{
		return 0;
	}
}
byte JudgeToT()
{
	byte irow;
	if(LineType[0]==UnExist&&LineType[1]==UnExist)
		return 0;
	if(count<3)	//没有拐点
	{
		irow=RoadStart;
		if(EndRow[0]<EndRow[1])		//左线长
		{
			while(CenterLine[irow]-CenterLine[RoadStart]<7&&irow>RoadEnd)
			{
				irow--;
			}
			if(irow==RoadEnd)
				return 0;
			if(irow<45)
			{
				RoadType=ToT;
				return 1;
			}
			else
				return 0;
		}
		else			//右线长
		{
			while(CenterLine[RoadStart]-CenterLine[irow]<7&&irow>RoadEnd)
			{
				irow--;
			}
			if(irow==RoadEnd)
				return 0;
			if(irow<45)
			{
				RoadType=ToT;
				return 1;
			}
			else
				return 0;
		}
	}
	else
	{
		if(turn[1]>45)
			return 0;
		else if(ABS(CenterLine[RoadStart]-CenterLine[turn[1]])<6)
		{
			RoadType=ToT;
			return 1;
		}
		else
		{
			irow=turn[1];
			while(ABS(CenterLine[irow]-CenterLine[turn[1]])<6&&irow<RoadStart)
			{
				irow++;
			}
			if(irow==RoadStart)
			{
				RoadType=ToT;
				return 1;
			}
			if(irow-turn[1]>30)
			{
				RoadType=ToT;
				return 1;
			}
			return 0;
		}
	}
}
byte JudgeUTurn()
{
	byte irow,iturn;
	signed char tem;
	if(RoadEnd>=28)
	{
		if(EndRow[0]<EndRow[1]||StartRow[1]==ROWS)		//左线长
		{
			if(BlackLine[0][EndRow[0]]<=COLUMN1_2)
				return 0;
			irow=StartRow[0];
			while(irow>EndRow[0])
			{
				if(BlackLine[0][irow]>BlackLine[0][irow-1])
					break;
				irow--;
			}
			if(irow==EndRow[0])
			{
				RoadType=UTurn;
				return 1;
			}
		}
		else				//右线长
		{
			if(BlackLine[1][EndRow[1]]>=COLUMN1_2)
				return 0;
			irow=StartRow[1];
			while(irow>EndRow[1])
			{
				if(BlackLine[1][irow]<BlackLine[1][irow-1])
					break;
				irow--;
			}
			if(irow==EndRow[1])
			{
				RoadType=UTurn;
				return 1;
			}
		}
	}
	if(count>2&&CenterLine[RoadStart]-CenterLine[turn[1]]>35)		//近处向左
	{
		iturn=turn[1];
		while(iturn<RoadStart)
		{
			if(CenterLine[iturn]!=CenterLine[iturn+1])
				break;
			iturn++;
		}
		if(BlackLine[1][iturn]>COLUMN1_2-10||iturn<26)
			return 0;
		irow=StartRow[1];
		while(irow>iturn)
		{
			if(BlackLine[1][irow]<BlackLine[1][irow-1])
				break;
			irow--;
		}
		if(irow==iturn)
		{
			RoadType=UTurn;
			RoadEnd=turn[1];
			EndRow[1]=turn[1];
	    	if(StartRow[0]==ROWS)	tem=EndRow[1]/8+(8-(StartRow[1]-EndRow[1])/8);
	    	else
	    	{
	    		tem=EndRow[0]>60?(EndRow[0]-60)/2:(60-EndRow[0])/10;
	    		tem+=6-(StartRow[1]-EndRow[1])/8;	
	    	}
			for(irow=RoadEnd;irow<=RoadStart;irow++)
				CenterLine[irow]=CenterLine[irow]-2*tem;
			return 1;
		}
		else	return 0;
	}
	else if(count>2&&CenterLine[turn[1]]-CenterLine[RoadStart]>35)	//近处向右
	{
		iturn=turn[1];
		while(iturn<RoadStart)
		{
			if(CenterLine[iturn]!=CenterLine[iturn+1])
				break;
			iturn++;
		}
		if(BlackLine[0][iturn]<COLUMN1_2+10||iturn<26)
			return 0;
		irow=StartRow[0];
		while(irow>iturn)
		{
			if(BlackLine[0][irow]>BlackLine[0][irow-1])
				break;
			irow--;
		}
		if(irow==iturn)
		{
			RoadType=UTurn;
			RoadEnd=turn[1];
			EndRow[0]=turn[1];
			if(StartRow[1]==ROWS)	tem=EndRow[0]/8+(8-(StartRow[0]-EndRow[0])/8);
			else
			{
				tem=EndRow[1]>60?(EndRow[1]-60):(60-EndRow[1])/10;
				tem+=6-(StartRow[0]-EndRow[0])/8;	
			}
			for(irow=EndRow[0];irow<=StartRow[0];irow++)
				CenterLine[irow]=CenterLine[irow]+2*tem;
			return 1;
		}
		else	return 0;
	}
	return 0;
}
void JudgeGeneral()
{
	if(RoadEnd<15)
	{
		RoadType=Far;
	}
	else if(RoadEnd<30)
	{
		RoadType=Mid;	
	}
	else
	{
		RoadType=Near;
	}
}

/**************************************************
函数功能：在直角标志位处补线，当中线过短时补线
函数输入：lackRow_Start，BlackRow_End
函数输出：BlackLine[][] 
说明：
***************************************************/
void CenterLineFill(void)//从最近处的一点开始进行平滑处理
{
	signed int i,j;

	//直角标志位补线
	if((RoadType!=Barrier1)&&(Slope!=1))
	{
		if(((ABS(BlackRow_Start-BlackRow_End)>=1)&&(ABS(BlackRow_Start-BlackRow_End)<=20))&&(BlackRow_Start!=ROWS)&&(BlackRow_End!=0))//黑色起始行和黑色结束行行距一定范围内时认为是直角标志位
		{
			if((BlackRow_Start<=(ROW-10))&&(BlackRow_End>=10))//直角标志位较远时
			{
				for(i=(BlackRow_Start+9);i>=(BlackRow_End-9);i--)
				{
					CenterLine[i]=((float)CenterLine[BlackRow_End-10]-(float)CenterLine[BlackRow_Start+10])/((float)BlackRow_End-(float)BlackRow_Start-(float)20)*((float)i-(float)BlackRow_Start-(float)10)+(float)CenterLine[BlackRow_Start+10];
				}			
			}
			else if((BlackRow_Start<=(ROW-5))&&(BlackRow_End>=5))//直角标志位较近时
			{
				for(i=(BlackRow_Start+4);i>=(BlackRow_End-4);i--)
				{
					CenterLine[i]=((float)CenterLine[BlackRow_End-5]-(float)CenterLine[BlackRow_Start+5])/((float)BlackRow_End-(float)BlackRow_Start-(float)10)*((float)i-(float)BlackRow_Start-(float)5)+(float)CenterLine[BlackRow_Start+5];
				}			
			}
			else//直角标志位很近时
			{
				for(i=ROW;i>=(BlackRow_End-9);i--)
				{
					CenterLine[i]=((float)CenterLine[BlackRow_End-7]-(float)CenterLine[BlackRow_End-10])/(float)3*((float)i-(float)BlackRow_End+(float)7)+(float)CenterLine[BlackRow_End-7];
				}
			}
		}
	}
	
	//防止补线失败
	for(i=ROW;i>=RoadEnd;i--)
	{
		if(CenterLine[i]==120)  
		{
			CenterLine[i]=41;
		}
	}

	//中线过短时补线
	if((RoadEnd>55)&&(RoadEnd<60)&&(!(flag_Rightangle_l||flag_Rightangle_r))&&(Slope!=1))
	{
		for(i=(RoadEnd-1);i>=50;i--)
		{
			CenterLine[i]=(float)CenterLine[RoadStart]-((float)RoadStart-(float)i)*((float)CenterLine[RoadStart]-(float)CenterLine[RoadEnd])/((float)RoadStart-(float)RoadEnd);
		}
		RoadEnd=50;
	}
}

//*****************************************************************************************************************
//*	 *************************通过有效行分类计算offset******************************************************* 	  *
//*****************************************************************************************************************
void TargetOffset()
{	
	if(Slope==1)										UpSlopeOffset();
	else if(Slope==2) 									UpSlopeOffset();	
	else if((RoadType==Barrier1)||(RoadType==Barrier2)) BarrierOffset();	
	else if(RoadType==Straight||RoadType==SmallS) 		StraightOffset();
	else if(RoadType==UTurn) 							UTurnOffset();					
	else if(RoadType==ToT)								ToToffset();
	else if(RoadType==Near||RoadType==Mid) 				NearOffset();
	else 												FarOffset();
	
	if(target_offset>43) target_offset=43;
	else if(target_offset<-43) target_offset=-43;
}

void UpSlopeOffset()	//上坡偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	irowe=RoadEnd<45?45:RoadEnd;
	for(irow=irowe;irow<=(RoadStart-5);++irow){
		target_offset+=(CenterLine[irow]-COLUMN1_2)*SlopeWeight[irow];
		sum_weight+=SlopeWeight[irow];
	}
	target_offset/=sum_weight;
}

void BarrierOffset()	//直道偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*BarrierWeight[irow];
			sum_weight+=BarrierWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*BarrierWeight1[irow];
			sum_weight+=BarrierWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}

void StraightOffset()	//直道偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*StraightWeight[irow];
			sum_weight+=StraightWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*StraightWeight1[irow];
			sum_weight+=StraightWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}

void UTurnOffset()	//180度弯偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*UTurnWeight[irow];
			sum_weight+=UTurnWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*UTurnWeight1[irow];
			sum_weight+=UTurnWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}

void ToToffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*ToTWeight[irow];
			sum_weight+=ToTWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*ToTWeight1[irow];
			sum_weight+=ToTWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}

void FarOffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*FarWeight[irow];
			sum_weight+=FarWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*FarWeight1[irow];
			sum_weight+=FarWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}


void NearOffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		if((mode==0)||(mode==1)||(mode==2))
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*NearWeight[irow];
			sum_weight+=NearWeight[irow];
		}
		else if(mode==3)
		{
			target_offset+=(CenterLine[irow]-COLUMN1_2)*NearWeight1[irow];
			sum_weight+=NearWeight1[irow];
		}
	}
	target_offset/=sum_weight;
}

void Typejudge()
{
	if(RoadType==66)      //道路类型66，表示单车道障碍    jqy
		jishu++;
//	if(RoadType==12 || RoadType==12)      //道路类型66，表示单车道障碍    jqy
//		jishu++;
	if(jishu>=4 && bz==1)   //bz位为1时，进行超车    jqy
	{
		zhangai=0;
		jishu=0;
	}
	if(jishu>=1 && bz==0)   //bz位为1时，进行避障停车   jqy
	{
		zhangai=0;
		jishu=0;
	}
	if(RoadType==88 && bz==2 && g_device_NO==1)    //道路类型88，表示双车道障碍，需掉头绕行    jqy
	{
	//	delay_ms(300);
		set_speed_pwm(-800);
		delay_ms(70);
		set_speed_pwm(0);
		jishu=0;
		Car_Stop=1;
		while(Car_Stop==1)
		{
			if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
			{
				g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
				Wifi_Ctrl();
			}
			if(order_received==1)
			{
				order_received=0;
				generate_remote_frame_2(g_device_NO_Hex, 0x33, 0x0000, 2, (const BYTE *)(&response_data));
			}
			FindBlackLine();
			CenterLineWithVideo(); 
			Video_Show();
			LCD_Write_Num(105,2,RoadType,2);
			control_car_action();
			EMIOS_0.CH[3].CSR.B.FLAG = 1;
			EMIOS_0.CH[3].CCR.B.FEN=1;
		}
		zhangai=1;
		bz=-1;
#if 0
		set_speed_pwm(0);
		delay_ms(2000);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		set_speed_pwm(320);
		delay_ms(1600);/////
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.right_limit);
		delay_ms(1000);
		set_speed_pwm(-320);
		delay_ms(1200);/////
		set_speed_pwm(0);
		set_steer_helm_basement(data_steer_helm_basement.left_limit);
		delay_ms(1000);
		set_speed_pwm(320);
		delay_ms(1500);
		bz=5;/////
#endif
	}
	if(RoadType==88 && bz==2 && g_device_NO==2)    //道路类型88，表示双车道障碍，需掉头绕行    jqy
	{
		sending_service_package(0x33,0x0088,0x00);
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
		delay_ms(1300);/////
		bz=5;
	}
}
