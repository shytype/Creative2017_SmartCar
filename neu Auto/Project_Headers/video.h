#ifndef __VEDIO_H__
#define __VEDIO_H__
#define byte unsigned char
#define word unsigned int


#define ROW  (69)	
#define ROWS  (70)	
#define COLUMN (82)
#define COLUMNS (83)

extern BYTE croadtype;


extern void FieldInputCapture(void);
extern void RowInputCapture(void);
void Video_Image(void);
extern void Display_Video(void);
extern void TF_Image(void);

//*************************************************************************
//*			**********************图像发送函数*************************   *
//*************************************************************************
void Send_CCD_Video(void); 
void SetupCCD(void);

void SetupBKL(void);
void Send_CCD_Blackline(void);
void Video(void);
void CenterLineWithVideo(void);
void ThreeLine(void);
void Video_Show(void);
//*************************************************************************
//*			**********************采样变量*****************************   *
//*************************************************************************
extern byte fieldover;                //一场采样结束标志符 
extern volatile byte g_pix[ROWS][COLUMNS]; 
extern volatile byte pic[ROWS][COLUMNS];
extern byte	pix[ROWS][COLUMNS];

#endif

