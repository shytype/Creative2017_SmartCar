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
byte turn[10];					//turn[10]统计中线的起点、拐点、终点
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
  return;
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
	return;
}

/**************************************************
函数功能：黑线初始化
函数输入：lr ROW 0
函数输出：BlackLine[2][70]
说明：初始化双边黑线数组的值有待于商量(根据后面的需求)
***************************************************/
void BlackLine_Init(byte lr,byte irowb,byte irowe)    
{
	return;  
}

/**************************************************
函数功能：找线程序 单/双线
函数输入：g_pix[][]
函数输出：BlackLine[2][70]  
说明：初始化中中黑线数组和双边黑线数组的值有待于商量
***************************************************/
void FindLine_Simple()//找线程序
{
	return;
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
	return;
}


/**************************************************
函数功能：判断该行为单线还是双线
函数输入：BlackLine[][]  irow  point_start_pre
函数输出：findline_flag
说明：根据BlackLine[][]的大小来判断单双线
***************************************************/
byte Findline_type(byte irow)
{
	return;
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
	return;
}


/**************************************************
函数功能：确定中黑线的中心点 
函数输入：irow  BlackLine[][]
函数输出：BlackLine[][] BlackCenterLine_Simple[]
说明：
***************************************************/
void Findpoint_start_simple(byte irow)//确定中黑线的中心点 补双黑线
{
	return;
}

/**************************************************
函数功能：已确定中黑线的中心点 补双黑线
函数输入：irow  BlackCenterLine_Simple[]
函数输出：BlackLine[][] 
说明：
***************************************************/
void Findline_filldouble(signed char search_point,byte irow)//把单线转换为双线处理，并判断赛道类型
{
	return;
}

/**************************************************
函数功能：寻找左右黑线起始点，用于赛道判断
函数输入：BlackLine[][] 
函数输出：StartRow[lr]，EndRow[lr] 
说明：
***************************************************/
void Pre_handle(byte lr)
{
	return;
}

/**************************************************
函数功能：判断直角
函数输入：BlackLine[2][70]，RoadEnd
函数输出：flag_Rightangle_l(左直角标志)  flag_Rightangle_l(右直角标志)
说明：
***************************************************/
void Analyse_Rigntangle()
{
	return;
}

/**************************************************
函数功能：找十字条件
函数输入：BlackLine[2][70]
函数输出：
说明：
***************************************************/
byte Re_Cross()
{	
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
	return;
}

/**************************************************
函数功能：十字补线
函数输入：flag_Rightangle_l，flag_Rightangle_r，Slope(上坡标志)，CrossNum，BlackLine[2][70]，RoadEnd
函数输出：BlackLine[2][70]，CenterLine[70]
说明：CrossNum值可调
***************************************************/
void Cross_Fill(void)
{
	return;
}

/**************************************************
函数功能：判断上下坡
函数输入：
函数输出：
说明：直接使用学长程序，可准确判断出上坡，但不能判断出下坡
***************************************************/
void DetectSlope()
{
	return;
}
void DetectUpSlope()
{
	return;
}
void DetectDownSlope()
{
	return;
}

/**************************************************
函数功能：判断赛道类型
函数输入：
函数输出：RoadType
说明：
***************************************************/
void AnalyzeRoadType()
{
	return;
}

/**************************************************
函数功能：检测障碍物
函数输入：LeftBlackRow，RightBlackRow，BlackLine[][]，CenterLine[]
函数输出：BlackLine[][]，CenterLine[]，RoadType
说明：
***************************************************/
byte JudgeBarrier()
{
	return;
}

/**************************************************
函数功能：判断是处于中黑线
函数输入：BCL_Start，BCL_End
函数输出：
说明：若判断为中黑心就不检测障碍物，防止干扰，ABS(BCL_Start-RoadEnd)可调
***************************************************/
byte JudgeBCL()
{
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
	return;
}
byte JudgeToT()
{
	return 0;
}
byte JudgeUTurn()
{
	return 0;
}
void JudgeGeneral()
{
	return;
}

/**************************************************
函数功能：在直角标志位处补线，当中线过短时补线
函数输入：lackRow_Start，BlackRow_End
函数输出：BlackLine[][] 
说明：
***************************************************/
void CenterLineFill(void)//从最近处的一点开始进行平滑处理
{
	return;
}

//*****************************************************************************************************************
//*	 *************************通过有效行分类计算offset******************************************************* 	  *
//*****************************************************************************************************************
void TargetOffset()
{	
	return;
}

void UpSlopeOffset()	//上坡偏移量计算
{
	return;
}

void BarrierOffset()	//直道偏移量计算
{
	return;
}

void StraightOffset()	//直道偏移量计算
{
	return;
}

void UTurnOffset()	//180度弯偏移量计算
{
	return;
}

void ToToffset()
{
	return;
}

void FarOffset()
{
	return;
}


void NearOffset()
{
	return;
}

void Typejudge()
{
	return;
}
