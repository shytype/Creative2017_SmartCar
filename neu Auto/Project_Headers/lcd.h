#ifndef __LCD_H__
#define __LCD_H__


void initLCD(void);
void LCD_WrDat(BYTE data);
void LCD_WrCmd(BYTE cmd);  //输出cmd
void LCD_Set_Pos(BYTE x, BYTE y);
void LCD_Fill(BYTE bmp_data);
void LCD_CLS(void);//清屏
void LCD_PutPixel(BYTE x,BYTE y);
void LCD_Rectangle(BYTE x1,BYTE y1,BYTE x2,BYTE y2,BYTE gif);
void LCD_P6x8Str(BYTE x,BYTE y,BYTE ch[]);
void LCD_P8x16Str(BYTE x,BYTE y,BYTE ch[]);
void LCD_P14x16Str(BYTE x,BYTE y,BYTE ch[]);
void LCD_Printbyte(BYTE x, BYTE y, BYTE ch[]);
void LCD_PrintoutInt(BYTE x, BYTE y, SWORD data);
void LCD_PrintoutIntS(BYTE x, BYTE y, SWORD data);
void LCD_PrintoutFloat(BYTE x, BYTE y, float n);
void LCD_DISPLAY(void);
void LCD_Write_Num(unsigned char X,unsigned char Y,int num,unsigned char N);
void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);




//*****************OLED****************//
//#define LCD_SCL  SIU.GPDO[0].R//时钟
//#define LCD_SDA  SIU.GPDO[1].R//输入输出
#define LCD_RST  SIU.GPDO[37].R//reset
#define LCD_DC  SIU.GPDO[43].R//命令/数据

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 


#define X_WIDTH 128
#define Y_WIDTH 64





#endif
