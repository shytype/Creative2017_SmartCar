#define __SUPERSONIC_C_
#include "includes.h"
#include "math.h"
int sign=0;
int sign_left=0;
int sign_right=0;
int get_ss=1;
int sssum = 0; 
int sscount=0;
char value_buf[21];  
int sscount,ssi,ssj,sstemp,ssr=0,sssumr=0,ssnsigma=0,sssigma=0,ssaverage=0; 
int ssvalue=0;
extern int zhangai;

//**********************超声0***************************
void init_supersonic_trigger_0(void)
{
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;
	EMIOS_0.CH[0].CCR.B.UCPRE=0;
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;
	EMIOS_0.CH[0].CCR.B.FREN = 0;
	EMIOS_0.CH[0].CCR.B.EDPOL=0;
	EMIOS_0.CH[0].CCR.B.EDSEL = 0;
	EMIOS_0.CH[0].CCR.B.FEN=0;
	
	EMIOS_0.CH[0].CADR.B;
	SIU.PCR[0].R =0x0603;	//trigger A0 65us
	
	EMIOS_0.CH[0].CADR.B.CADR = 0x00ff;
	
	EMIOS_0.CH[0].CCR.B.MODE = 0x03;
	tmp_time.R = 0x0000;
}

void init_supersonic_trigger_1(void)
{
	EMIOS_0.CH[2].CCR.B.BSL = 0x3;
	EMIOS_0.CH[2].CCR.B.UCPRE=0;
	EMIOS_0.CH[2].CCR.B.UCPEN = 1;
	EMIOS_0.CH[2].CCR.B.FREN = 0;
	EMIOS_0.CH[2].CCR.B.EDPOL=0;
	EMIOS_0.CH[2].CCR.B.EDSEL = 0;
	EMIOS_0.CH[2].CCR.B.FEN=0;
	
	EMIOS_0.CH[2].CADR.B;
	SIU.PCR[2].R =0x0603;	//trigger A2
	
	EMIOS_0.CH[2].CADR.B.CADR = 0x00ff;
	
	EMIOS_0.CH[2].CCR.B.MODE = 0x03;
}


void init_supersonic_trigger_2(void)
{
	EMIOS_0.CH[4].CCR.B.BSL = 0x3;
	EMIOS_0.CH[4].CCR.B.UCPRE=0;
	EMIOS_0.CH[4].CCR.B.UCPEN = 1;
	EMIOS_0.CH[4].CCR.B.FREN = 0;
	EMIOS_0.CH[4].CCR.B.EDPOL=0;
	EMIOS_0.CH[4].CCR.B.EDSEL = 0;
	EMIOS_0.CH[4].CCR.B.FEN=0;
	
	EMIOS_0.CH[4].CADR.B;
	SIU.PCR[4].R =0x0603;	//trigger A5
	
	EMIOS_0.CH[4].CADR.B.CADR = 0x0000ff;
	
	EMIOS_0.CH[4].CCR.B.MODE = 0x03;
}


void init_supersonic_trigger_3(void)
{
	EMIOS_0.CH[6].CCR.B.BSL = 0x3;
	EMIOS_0.CH[6].CCR.B.UCPRE=0;
	EMIOS_0.CH[6].CCR.B.UCPEN = 1;
	EMIOS_0.CH[6].CCR.B.FREN = 0;
	EMIOS_0.CH[6].CCR.B.EDPOL=0;
	EMIOS_0.CH[6].CCR.B.EDSEL = 0;
	EMIOS_0.CH[6].CCR.B.FEN=0;
	
	EMIOS_0.CH[6].CADR.B;
	SIU.PCR[6].R =0x0603;	//trigger A6
	
	EMIOS_0.CH[6].CADR.B.CADR = 0x00ff;
	
	EMIOS_0.CH[6].CCR.B.MODE = 0x03;
}

//void init_supersonic_trigger_0_2(void)
//{
//	SIU.PCR[0].R =0x0203;	//GPIO
//}
//void supersonic_trigger_0_2(void)
//{
//	SIU.GPDO[0].B.PDO=1;
//	delay_us(50);
//	SIU.GPDO[0].B.PDO=0;
//	//tmp_time.R = 0x00000000;
//}
void init_supersonic_receive_0(void)
{
	EMIOS_0.CH[1].CCR.B.MODE = 0x04; // Mode is Input Pulse Width Measurement 
	EMIOS_0.CH[1].CCR.B.BSL = 0x3;   // Use internal counter
	EMIOS_0.CH[1].CCR.B.UCPRE=0; //Set channel prescaler to divide by 1
	EMIOS_0.CH[1].CCR.B.UCPEN = 1;	//Enable prescaler; uses default divide by 1
	EMIOS_0.CH[1].CCR.B.FREN = 0;	//Freeze channel counting when in debug mode
	EMIOS_0.CH[1].CCR.B.EDPOL=1; //Edge Select rising edge
//	EMIOS_0.CH[1].CCR.B.FEN=1;  //interupt enbale

	SIU.PCR[1].R = 0x0102;  //E PA1
//	INTC_InstallINTCInterruptHandler(supersonic, 141, 4);
}


void init_supersonic_receive_1(void)
{
	EMIOS_0.CH[3].CCR.B.MODE = 0x04; // Mode is Input Pulse Width Measurement 
	EMIOS_0.CH[3].CCR.B.BSL = 0x3;   // Use internal counter
	EMIOS_0.CH[3].CCR.B.UCPRE=0; //Set channel prescaler to divide by 1
	EMIOS_0.CH[3].CCR.B.UCPEN = 1;	//Enable prescaler; uses default divide by 1
	EMIOS_0.CH[3].CCR.B.FREN = 0;	//Freeze channel counting when in debug mode
	EMIOS_0.CH[3].CCR.B.EDPOL=1; //Edge Select rising edge
//	EMIOS_0.CH[3].CCR.B.FEN=1;  //interupt enbale

	SIU.PCR[3].R = 0x0102;  //E PA3
//	INTC_InstallINTCInterruptHandler(intc_get_supersonic_time_1, 142, 4);
}


void init_supersonic_receive_2(void)
{
	EMIOS_0.CH[5].CCR.B.MODE = 0x04; // Mode is Input Pulse Width Measurement 
	EMIOS_0.CH[5].CCR.B.BSL = 0x3;   // Use internal counter
	EMIOS_0.CH[5].CCR.B.UCPRE=0; //Set channel prescaler to divide by 1
	EMIOS_0.CH[5].CCR.B.UCPEN = 1;	//Enable prescaler; uses default divide by 1
	EMIOS_0.CH[5].CCR.B.FREN = 0;	//Freeze channel counting when in debug mode
	EMIOS_0.CH[5].CCR.B.EDPOL=1; //Edge Select rising edge
//	EMIOS_0.CH[5].CCR.B.FEN=1;  //interupt enbale

	SIU.PCR[5].R = 0x0102;  //E PA5
//	INTC_InstallINTCInterruptHandler(intc_get_supersonic_time_2, 143, 4);
}


void init_supersonic_receive_3(void)
{
	EMIOS_0.CH[16].CCR.B.MODE = 0x04; // Mode is Input Pulse Width Measurement 
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;   // Use internal counter
	EMIOS_0.CH[16].CCR.B.UCPRE=0; //Set channel prescaler to divide by 1
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;	//Enable prescaler; uses default divide by 1
	EMIOS_0.CH[16].CCR.B.FREN = 0;	//Freeze channel counting when in debug mode
	EMIOS_0.CH[16].CCR.B.EDPOL=1; //Edge Select rising edge
//	EMIOS_0.CH[7].CCR.B.FEN=1;  //interupt enbale

	SIU.PCR[64].R = 0x0102;  //E PA7
//	INTC_InstallINTCInterruptHandler(intc_get_supersonic_time_3, 144, 4);
}

void trigger_supersonic_0(void)
{
	EMIOS_0.CH[0].CCR.B.MODE = 0x01;
	EMIOS_0.CH[0].CCR.B.MODE = 0x03;
}

void trigger_supersonic_1(void)
{
	EMIOS_0.CH[2].CCR.B.MODE = 0x01;
	EMIOS_0.CH[2].CCR.B.MODE = 0x03;
}


void trigger_supersonic_2(void)
{
	EMIOS_0.CH[4].CCR.B.MODE = 0x01;
	EMIOS_0.CH[4].CCR.B.MODE = 0x03;
}


void trigger_supersonic_3(void)
{
	EMIOS_0.CH[6].CCR.B.MODE = 0x01;
	EMIOS_0.CH[6].CCR.B.MODE = 0x03;
}


void get_supersonic_time_0(void)
{
	DWORD tmp_a, tmp_b;
	
	tmp_a = EMIOS_0.CH[1].CADR.B.CADR;
	tmp_b = EMIOS_0.CH[1].CBDR.B.CBDR;
	
	tmp_time.R = tmp_a - tmp_b;
	if(tmp_a >= tmp_b)
	{
		tmp_time.R = tmp_a - tmp_b;
	}
	else
	{
		tmp_time.R = 0xFFFFFFFF - tmp_b + tmp_a;
	}
	
	EMIOS_0.CH[1].CSR.B.FLAG = 1;	//清除中断标志位

}

void get_supersonic_time_1(void)
{
	DWORD tmp_a, tmp_b;
	
	tmp_a = EMIOS_0.CH[3].CADR.B.CADR;
	tmp_b = EMIOS_0.CH[3].CBDR.B.CBDR;
	
	tmp_time.R = tmp_a - tmp_b;
}


void get_supersonic_time_2(void)
{
	DWORD tmp_a, tmp_b;

	tmp_a = EMIOS_0.CH[5].CADR.R;
	tmp_b = EMIOS_0.CH[5].CBDR.R;
	
	tmp_time2.R = tmp_a - tmp_b;
#if 1
		if(tmp_a >= tmp_b)
		{
			tmp_time2.R = tmp_a - tmp_b;
		}
		else
		{
			tmp_time2.R = 0xFFFFFFFF - tmp_b + tmp_a;
		}

		EMIOS_0.CH[1].CSR.B.FLAG = 1;	//清除中断标志位
#endif

}


void get_supersonic_time_3(void)
{
	DWORD tmp_a, tmp_b;

//	tmp_a = EMIOS_0.CH[7].CADR.R;
//	tmp_b = EMIOS_0.CH[7].CBDR.R;
	tmp_a = EMIOS_0.CH[16].CADR.R;
	tmp_b = EMIOS_0.CH[16].CBDR.R;
	tmp_time.R = tmp_a - tmp_b;

}


#if 0
void supersonic(void)
{
 	trigger_supersonic_0();
    get_supersonic_time_0();
    trigger_supersonic_2();
    get_supersonic_time_2();
    if(((WORD)(tmp_time.R)/100)>0&&((WORD)(tmp_time.R)/100)<150 && ((WORD)(tmp_time2.R)/100)<(WORD)(tmp_time.R)/100)
    {
    	sign_left++;
    	trigger_supersonic_0();
    	get_supersonic_time_0();
    	trigger_supersonic_2();
    	get_supersonic_time_2();
    	LCD_Write_Num(96,6,((WORD)(tmp_time.R)/100),5);
    	LCD_Write_Num(96,7,((WORD)(tmp_time2.R)/100),5);
    }
    else if(((WORD)(tmp_time2.R)/100)>0&&((WORD)(tmp_time2.R)/100)<150 && ((WORD)(tmp_time.R)/100)<(WORD)(tmp_time2.R)/100)
    {
    	sign_right++;
    	trigger_supersonic_0();
    	get_supersonic_time_0();
    	trigger_supersonic_2();
    	get_supersonic_time_2();
    	LCD_Write_Num(96,6,((WORD)(tmp_time.R)/100),5);
    	LCD_Write_Num(96,7,((WORD)(tmp_time2.R)/100),5);    
    }
    else if(((WORD)(tmp_time.R)/100)>150&&((WORD)(tmp_time2.R)/100)>150)
    {
    	sign_left=0;
    	sign_right=0;
    	trigger_supersonic_0();
    	get_supersonic_time_0();
    	trigger_supersonic_2();
    	get_supersonic_time_2();
    	LCD_Write_Num(96,6,((WORD)(tmp_time.R)/100),5);
    	LCD_Write_Num(96,7,((WORD)(tmp_time2.R)/100),5);
    }
   if(sign_left==10)
   {
	   sign_left=0;
	   sign_right=0;
	   get_ss=1;
	   set_steer_helm_basement((data_steer_helm_basement.right_limit-data_steer_helm_basement.center)*(220-((WORD)(tmp_time.R)/100)/220+data_steer_helm_basement.center));
	   delay_ms(500);
	   set_steer_helm_basement(data_steer_helm_basement.center);
    }
   if(sign_right==10)
      {
   	   sign_left=0;
   	   sign_right=0;
   	   get_ss=1;
   	   set_steer_helm_basement(data_steer_helm_basement.center-(data_steer_helm_basement.center-data_steer_helm_basement.left_limit)*(220-((WORD)(tmp_time2.R)/100)/220));
   	   delay_ms(500);
   	   set_steer_helm_basement(data_steer_helm_basement.center);
       }
}
#endif
#if 1
void supersonic(void)
{
   sssum=0;
   sssumr=0;
   ssr=0;
   for (sscount=0;sscount<20;sscount++)  
   {  
      trigger_supersonic_0();
      get_supersonic_time_0();
      sssum+= (WORD)(tmp_time.R)/100 ;        
   } 
   
 
//   for (sscount=0;sscount<21;sscount++)  
//   {  
//	   trigger_supersonic_0();
//	   get_supersonic_time_0();
//	   value_buf[sscount] =(WORD)(tmp_time.R)/100 ; 
//	   sssum+= value_buf[sscount];
//   } 
//   ssaverage=sssum/21;
//   for (ssi=0;ssi<21;ssi++)  
//   {  
//      ssnsigma+=(value_buf[ssi]-ssaverage)*(value_buf[ssi]-ssaverage);
//   }
//   sssigma=sqrt(ssnsigma/21);
//   for(ssi=0;ssi<21;ssi++) 
//   {
//	   if(value_buf[ssi]-ssaverage<=3*sssigma)
//	   {
//		   sssumr+= value_buf[ssi];
//		   ssr++;
//	   }	   
//   }   
   ssvalue=sssum/20;
    if(ssvalue<170&&ssvalue>10)
    {
    	sign++;
//    	trigger_supersonic_0();
//    	get_supersonic_time_0();
    	LCD_Write_Num(96,6,(WORD)(ssvalue),5);
    }
    else if(ssvalue>220)
    {
    	sign=0;
//    	trigger_supersonic_0();
//    	get_supersonic_time_0();
    	set_steer_helm_basement(data_steer_helm_basement.center);
    	LCD_Write_Num(96,6,(WORD)(ssvalue),5);
    }
    
 	if(sign>=10)
 	{
 		sign=0; 
 		set_steer_helm_basement(data_steer_helm_basement.right_limit);
 		set_speed_pwm(375);
 		get_ss=1;
        //delay_ms(500);
 		//set_steer_helm_basement(data_steer_helm_basement.center);
 	}
}
#endif
