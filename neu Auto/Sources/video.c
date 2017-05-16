

#include "includes.h"
BYTE RowSAIC_DelayTime=43;
//*************************************************************************
//*		 *************************采样变量*****************************   *
//*************************************************************************
//采样图像标号
word crow = 0;           //行数计数器
byte prow = 0;  	  	 //摄像头采样当前行
byte pcolumn = 0;        //摄像头采样当前列
//采样结束标志
byte fieldover = 0;		//一场采样结束标志符 
//采样二维数组    
volatile byte g_pix[ROWS][COLUMNS];   // 采集灰度数组
byte pix[ROWS][COLUMNS];

word selectRows[ROWS]=
{		
////最小行:23
////最大行:257
////图像采样从近往远
//23 ,39 ,52 ,65 ,76 ,89 ,96 ,105,113,121,
//128,134,140,146,151,156,161,165,169,173,
//177,180,183,186,189,192,195,198,200,202,
//204,206,208,210,212,214,216,218,220,222,
//224,225,226,227,228,229,230,231,232,233,
//234,235,236,237,238,239,240,241,242,243,
//244,245,246,247,248,249,250,251,252,253,
		
//最小行:24，最大行:300
//24,		28,		32,		36,		40,		44,		48,		52,		56,		60,		
//64,		68,		72,		76,		80,		84,		88,		92,		96,		100,	
//104,	108,	112,	116,	120,	124,	128,	132,	136,	140,	
//144,	148,	152,	156,	160,	164,	168,	172,	176,	180,	
//184,	188,	192,	196,	200,	204,	208,	212,	216,	220,	
//224,	228,	232,	236,	240,	244,	248,	252,	256,	260,	
//264,	268,	272,	276,	280,	284,	288,	292,	296,	300,	

24,		27,		30,		33,		36,		39,		42,		45,		48,		51,		
54,		57,		60,		63,		66,		69,		72,		75,		78,		81,	
84,		87,		90,		93,		96,		99,		102,	105,	108,	111,	
114,	117,	120,	123,	126,	129,	132,	135,	138,	141,	
144,	147,	150,	153,	156,	159,	162,	165,	168,	171,	
174,	177,	180,	183,	186,	189,	192,	195,	198,	201,	
204,	207,	210,	213,	216,	219,	222,	225,	228,	231,	

};

//*************************************************************************
//*   **************************中断函数****************************      *
//*************************************************************************
void FieldInputCapture(void)	//捕捉场中断 
{	
    EMIOS_0.CH[3].CCR.B.FEN=0;  //关闭场中断 
	prow=0;crow=0;
	EMIOS_0.CH[7].CCR.B.FEN=1;  //开行中断
}

void RowInputCapture(void) 		//捕捉行中断
{	
	EMIOS_0.CH[7].CSR.B.FLAG = 1;  //开行中断
	++crow;
	//采样开始
	//行消影 因为摄像头输出信号的前几行是不能用的，所以扔掉前ROWS_MIN行
	if(crow>selectRows[prow])
  	{
	    unsigned char i;
	    for(pcolumn=0;pcolumn<RowSAIC_DelayTime;++pcolumn) {}
	    /************************
	    	延时消隐区
	    ***********************************/ 
	   
	   	g_pix[ROW-prow][82]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");	//汇编，延时
	   	g_pix[ROW-prow][81]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][80]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][79]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][78]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][77]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][76]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][75]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][74]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][73]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][72]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][71]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][70]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	    																																																			
	   	g_pix[ROW-prow][69]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][68]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][67]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][66]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][65]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][64]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][63]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][62]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][61]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][60]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   																																																			 
	   	g_pix[ROW-prow][59]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][58]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][57]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][56]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][55]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][54]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][53]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][52]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][51]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][50]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");

	   	g_pix[ROW-prow][49]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][48]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][47]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][46]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][45]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][44]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][43]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][42]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][41]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][40]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	    	   		   		   		   		
	   	g_pix[ROW-prow][39]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][38]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][37]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][36]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][35]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][34]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][33]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][32]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][31]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][30]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	
	   	g_pix[ROW-prow][29]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][28]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][27]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][26]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][25]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][24]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][23]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][22]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][21]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][20]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   		   		   		   	
	   	g_pix[ROW-prow][19]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][18]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][17]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][16]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][15]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][14]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][13]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][12]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][11]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][10]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	
	   	g_pix[ROW-prow][9]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][8]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][7]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][6]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][5]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][4]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][3]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][2]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][1]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][0]=SIU.GPDI[42].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");	
	    ++prow;
		if(prow==ROWS)	//一场图像传完后
		{
			EMIOS_0.CH[7].CCR.B.FEN=0; 	//关闭行中断
			fieldover=1; 
		}
	}
}


/*----------------------------------------------------------------------*/
/*视频信号输出                                            	 */
/*----------------------------------------------------------------------*/
void Video_Image(void)
{
	int y,x,i,col;
#if 1
	volatile int j=0;
	for(j=0;j<ROWS;j++)
	{
		col=CenterLine[j];
		g_pix[j][col]=0;
	}
#endif
	for(y=0;y<ROWS/8;y++)
		for(x=0;x<COLUMNS;x++)
		{
			pix[y][x]=0;
			for(i=7;i>=0;i--)
				pix[y][x]=pix[y][x]*2+g_pix[y*8+i][x];
		}	

}
void TF_Image(void)
{
	int i,j;
	for(i=0;i<ROWS;i++)
		for(j=0;j<COLUMNS;j++)
		{
			if(g_pix[i][j]==0)
				pic[i][j]=0x00;
			else
				pic[i][j]=0xFF;
		}
}
void Display_Video(void)
{
	
	BYTE x,y;

	Video_Image();
	for(y=0;y<ROWS/8;y++)
	{
		LCD_Set_Pos(10,y);				
    	for(x=0;x<COLUMNS;x++)
	    {      
	    	LCD_WrDat(pix[y][x]);		
	    }
	}
}

//*************************************************************************																							                        
//*   ********************无线发送图像******************************      *	
//*************************************************************************
void Send_CCD_Video(void) 
{
  unsigned char i,j;
  serial_port_0_TX(0x55);
  serial_port_0_TX(0xAA);
  serial_port_0_TX(0xFF);
  for(i = 0;i < ROWS;i ++)
  {   
	for(j = 0;j < COLUMNS;j ++)
	{
	   if(g_pix[i][j]==1)
		   serial_port_0_TX(0xFF);
	   else
		   serial_port_0_TX(0x00);
	}
  }
  serial_port_0_TX(0xFF);
  serial_port_0_TX(0xAA);
  serial_port_0_TX(0x55);
}

void SetupCCD(void)
{   
	serial_port_1_TX(0xAA);        
	serial_port_1_TX(0xBB);        
	serial_port_1_TX(0xFF);        
	serial_port_1_TX(ROWS);        
	serial_port_1_TX(COLUMNS);         
	serial_port_1_TX(0xFF);        
	serial_port_1_TX(0xBB);        
	serial_port_1_TX(0xAA);        
}

void SetupBKL(void)
{
    unsigned char para1[10] = {"L"};//设置显示在上位机上的变量名
    unsigned char para2[10] = {"R"};  
    unsigned char para3[10] = {"target_a"};
    byte i;
       
    serial_port_1_TX(0xCC);        
    serial_port_1_TX(0xDD);        
    serial_port_1_TX(0xFF);         
                                             
    serial_port_1_TX(ROWS);        
    serial_port_1_TX(COLUMNS);        
    serial_port_1_TX(3);         // number of parameters
    
    serial_port_1_TX(0);         // display position  0和1分别表示在左边窗口或者右边窗口显示
    serial_port_1_TX(0);         // display position
    serial_port_1_TX(1);
    
    for(i=0;i<10;++i)
    	serial_port_1_TX(para1[i]);
    for(i=0;i<10;++i)
    	serial_port_1_TX(para2[i]);
   	for(i=0;i<10;++i)
   		serial_port_1_TX(para3[i]);
    	        
   	serial_port_1_TX(9);         // parameter color
   	serial_port_1_TX(5);
   	serial_port_1_TX(9);         // parameter color
    
   	serial_port_1_TX(0xFF);        
   	serial_port_1_TX(0xDD);        
   	serial_port_1_TX(0xCC);  
}

void Send_CCD_Blackline(void)
{
	byte i=0;
	serial_port_1_TX(0x55);
	serial_port_1_TX(0xBB);
	serial_port_1_TX(0xFF);
    for(;i<ROWS;++i)
    	serial_port_1_TX(BlackLine[0][i]);
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(LineType[0]);//
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(LineType[1]);//
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(0x00);
    serial_port_1_TX(target_offset);//
    serial_port_1_TX(0xFF);        
    serial_port_1_TX(0xBB);        
    serial_port_1_TX(0x55);              
}


//*****************************************************************************************************************
//****************************************显示屏函数*****************************************************    	  *
//*****************************************************************************************************************

//******************************************显示图像*************************************
void Video()
{
	byte i,j;
	for(i=0;i<ROWS;++i)
		for(j=0;j<COLUMNS;++j)
		    pix[i][j]=g_pix[i][j];
}

//******************************************改图像和中线*************************************
void CenterLineWithVideo()
{
	byte i,j;
	for(i=0;i<ROWS;++i)
		for(j=0;j<COLUMNS;++j)
			pix[i][j]=g_pix[i][j];
	for(i=RoadEnd;i<=RoadStart;++i)
			pix[i][CenterLine[i]]=0;//中线显示为黑色	
}

//******************************************显示左右线和中线*************************************
void ThreeLine()
{
	byte i,j;
	for(i=0;i<ROWS;++i)
			for(j=0;j<COLUMNS;++j)
				pix[i][j]=1;
	for(i=RoadEnd;i<=RoadStart;++i)
		pix[i][CenterLine[i]]=0;
	for(i=EndRow[0];i<=StartRow[0];++i)
		pix[i][BlackLine[0][i]]=0;
	for(i=EndRow[1];i<=StartRow[1];++i)
		pix[i][BlackLine[1][i]]=0;
}

void Video_Show(void)
{
	unsigned char i;
	unsigned char j;
	unsigned char k;
	unsigned char _char;
	for(i=0;i<8;i++)//显示了64行数据
	{
		LCD_Set_Pos(0,i);
		for(j=0;j<COLUMNS;j++)
		{
			_char=0;
			for(k=0;k<8;k++)
			{
			_char=_char>>1;
			if(pix[i*8+k][j])
				_char=_char|0x80;	
			}
		 	LCD_WrDat(_char);
		}
	}	
}
