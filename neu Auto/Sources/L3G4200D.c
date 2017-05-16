#define __L3G4200D_C_
/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : L3G4200D.c
* Author             : MSH Application Team
* Author             : Andrea Labombarda
* Version            : $Revision:$
* Date               : $Date:$
* Description        : L3G4200D driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 22/03/2011         |	Initial Revision                |	Andrea Labombarda

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUID ANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SET 0x01
#define RESET 0x00
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

int cl_flag=0;

BYTE L3G4200D_read_write_byte(BYTE data)
{
	uint32_t tmp_tx = 0x00000000;
	BYTE tmp_rx;
	tmp_tx |= 0xA0010000;
	tmp_tx |= (uint32_t)data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return tmp_rx;
}

/*******************************************************************************
* Function Name		: ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI reading functions					
* Input				: Register Address
* Output			: Data REad
* Return			: None
*******************************************************************************/
u8_t ReadReg(u8_t Reg, u8_t* Data) {
	
	//To be completed with either I2c or SPI reading function
	//i.e.: *Data = SPI_Mems_Read_Reg( Reg );
	uint32_t tmp_tx = 0x00000000;
	BYTE tmp_rx;
	
	tmp_tx = 0xA0080000 | Reg | 0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x20080000 | 0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data = tmp_rx;
	
	return 1;
}

/*******************************************************************************
* Function Name		: WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, Data to be written
* Output			: None
* Return			: None
*******************************************************************************/
u8_t WriteReg(u8_t Reg, u8_t Data) {
    
	//To be completed with either I2c or SPI writing function
	//i.e.: //SPI_Mems_Write_Reg(Reg, Data);
	uint32_t tmp_tx = 0x00000000;
	BYTE tmp_rx;
	
	tmp_tx = 0xA0080000 | (Reg & 0x7f);
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x20080000 | Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : SetODR
* Description    : Sets L3G4200D Output Data Rate
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetODR(ODR_t ov){
  u8_t value;

  if( !ReadReg(CTRL_REG1, &value) )
    return MEMS_ERROR;

  value &= 0x0f;
  value |= ov<<4;

  if( !WriteReg(CTRL_REG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetMode
* Description    : Sets L3G4200D Operating Mode
* Input          : Modality (NORMAL, SLEEP, POWER_DOWN)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetMode(Mode_t md) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG1, &value) )
    return MEMS_ERROR;
                  
  switch(md) {
  
  case POWER_DOWN:		
    value &= 0xF7;
    value |= (RESET<<PD);
    break;
          
  case NORMAL:
    value &= 0xF7;
    value |= (SET<<PD);
    break;
          
  case SLEEP:		
    value &= 0xF0;
    value |= ( (SET<<PD) | (RESET<<ZEN) | (RESET<<YEN) | (RESET<<XEN) );
    break;
          
  default:
    return MEMS_ERROR;
  }
  
  if( !WriteReg(CTRL_REG1, value) )
    return MEMS_ERROR;
                  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetAxis
* Description    : Enable/Disable L3G4200D Axis
* Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetAxis(Axis_t axis) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG1, &value) )
    return MEMS_ERROR;
    
  value &= 0xf8;
  value |= axis;
  
  if( !WriteReg(CTRL_REG1, value) )
    return MEMS_ERROR;  
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetFullScale
* Description    : Sets the L3G4200D FullScale
* Input          : FS_250/FS_500/FS_2000
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetFullScale(Fullscale_t fs) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG4, &value) )
    return MEMS_ERROR;
                  
  value &= 0xCF;	
  value |= (fs<<FS);
  
  if( !WriteReg(CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetBDU(State_t bdu) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG4, &value) )
    return MEMS_ERROR;
 
  value &= 0x7F;
  value |= (bdu<<BDU);

  if( !WriteReg(CTRL_REG4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : BLE_LSB / BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetBLE(Endianess_t ble) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG4, &value) )
    return MEMS_ERROR;
                  
  value &= 0xBF;	
  value |= (ble<<BLE);
  
  if( !WriteReg(CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetSelfTest
* Description    : Set Self Test Modality
* Input          : ST_NORMAL_MODE/ST_0/ST_1
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetSelfTest(SelfTest_t st) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG4, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF9;
  value |= (st<<SELF_TEST);
  
  if( !WriteReg(CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : HPFEnable
* Description    : Enable/Disable High Pass Filter
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t HPFEnable(State_t hpf) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG5, &value) )
    return MEMS_ERROR;
                  
  value &= 0xEF;
  value |= (hpf<<HPEN);
  
  if( !WriteReg(CTRL_REG5, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE/HPM_AUTORESET_INT
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetHPFMode(HPFMode_t hpf) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG2, &value) )
    return MEMS_ERROR;
                  
  value &= 0xCF;
  value |= (hpf<<HPM);
  
  if( !WriteReg(CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF_0,HPFCF_1,HPFCF_2... See Table 27 of the datasheet
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetHPFCutOFF(HPFCutOffFreq_t hpf) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG2, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF0;
  value |= (hpf<<HPFC0);
  
  if( !WriteReg(CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : SetIntPinMode
* Description    : Set Interrupt Pin Modality (push pull or Open drain)
* Input          : PUSH_PULL/OPEN_DRAIN
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetIntPinMode(IntPinMode_t pm) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG3, &value) )
    return MEMS_ERROR;
                  
  value &= 0xEF;
  value |= (pm<<PP_OD);
  
  if( !WriteReg(CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt1Pin
* Description    : Set Interrupt1 pin Function
* Input          : I1_ON_PIN_INT1_ENABLE | I1_BOOT_ON_INT1 | INT1_ACTIVE_HIGH
* example        : SetInt1Pin(I1_ON_PIN_INT1_ENABLE | I1_BOOT_ON_INT1_ENABLE | INT1_ACTIVE_LOW) 
* to enable Interrupt 1 or Bootsatus on interrupt 1 pin
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Pin(Int1PinConf_t pinConf) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG3, &value) )
    return MEMS_ERROR;
                  
  value &= 0x1F;
  value |= pinConf;
  
  if( !WriteReg(CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt2Pin
* Description    : Set Interrupt2 pin Function
* Input          : I2_DRDY_ON_INT2_ENABLE/DISABLE | 
                   WTM_ON_INT2_ENABLE/DISABLE | 
                   OVERRUN_ON_INT2_ENABLE/DISABLE | 
                   EMPTY_ON_INT2_ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt2Pin(Int2PinConf_t pinConf) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG3, &value) )
    return MEMS_ERROR;
                  
  value &= 0xF0;
  value |= pinConf;
  
  if( !WriteReg(CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t Int1LatchEnable(State_t latch) {
  u8_t value;
  
  if( !ReadReg(INT1_CFG, &value) )
    return MEMS_ERROR;
                  
  value &= 0xBF;
  value |= latch<<LIR;
  
  if( !WriteReg(INT1_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t ResetInt1Latch(void) {
  u8_t value;
  
  if( !ReadReg(INT1_SRC, &value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetIntConfiguration
* Description    : Interrupt 1 Configuration
* Input          : AND/OR, INT1_LIR ZHIE_ENABLE/DISABLE | INT1_ZLIE_ENABLE/DISABLE...
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetIntConfiguration(Int1Conf_t ic) {
  u8_t value;
  
  value = ic;

  if( !WriteReg(INT1_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Threshold(IntThsAxis axis, u16_t ths) {
  u8_t value;
  
  switch (axis) {
    
    case THS_X:
      //write the threshold LSB
      value = (u8_t)( ths & 0x00ff); 
      if( !WriteReg(INT1_THS_XL, value) )
        return MEMS_ERROR;
      
      //write the threshold LSB
      value = (u8_t)( ths >> 8); 
      if( !WriteReg(INT1_THS_XH, value) )
        return MEMS_ERROR;
      
      break;
      
    case THS_Y:
      //write the threshold LSB
      value = (u8_t)( ths & 0x00ff); 
      if( !WriteReg(INT1_THS_YL, value) )
        return MEMS_ERROR;
      
      //write the threshold LSB
      value = (u8_t)( ths >> 8); 
      if( !WriteReg(INT1_THS_YH, value) )
        return MEMS_ERROR;
      
      break;
      
    case THS_Z:
      //write the threshold LSB
      value = (u8_t)( ths & 0x00ff); 
      if( !WriteReg(INT1_THS_ZL, value) )
        return MEMS_ERROR;
      
      //write the threshold LSB
      value = (u8_t)( ths >> 8); 
      if( !WriteReg(INT1_THS_ZH, value) )
        return MEMS_ERROR;
      
      break;     

        
  }

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Duration(Int1Conf_t id) {
 
  if (id > 127)
    return MEMS_ERROR;

  if( !WriteReg(INT1_DURATION, id) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : FIFOModeEnable
* Description    : Sets Fifo Modality
* Input          : 
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t FIFOModeEnable(FifoMode_t fm) {
  u8_t value;  
  
  if(fm == FIFO_DISABLE) {
    
    if( !ReadReg(CTRL_REG5, &value) )
      return MEMS_ERROR;
                    
    value &= 0xBF;    
    
    if( !WriteReg(CTRL_REG5, value) )
      return MEMS_ERROR;
    
  }
  else {
    
    if( !ReadReg(CTRL_REG5, &value) )
      return MEMS_ERROR;
                    
    value &= 0xBF;
    value |= MEMS_SET<<FIFO_EN;
    
    if( !WriteReg(CTRL_REG5, value) )
      return MEMS_ERROR;
    
    
    if( !ReadReg(FIFO_CTRL_REG, &value) )
      return MEMS_ERROR;
    
    value &= 0x1f;
    value |= (fm<<FM0);
    
    if( !WriteReg(FIFO_CTRL_REG, value) )
      return MEMS_ERROR;
  }

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetWaterMark
* Description    : Sets Watermark Value
* Input          : Watermark = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetWaterMark(u8_t wtm) {
  u8_t value;
  
  if(wtm > 31)
    return MEMS_ERROR;  
  
  if( !ReadReg(FIFO_CTRL_REG, &value) )
    return MEMS_ERROR;
                  
  value &= 0xE0;
  value |= wtm; 
  
  if( !WriteReg(FIFO_CTRL_REG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetSatusReg
* Description    : Read the status register
* Input          : None
* Output         : status register buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetSatusReg(u8_t* buff) {
  if( !ReadReg(STATUS_REG, buff) )
      return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : GetAngRateRaw
* Description    : Read the Angular Rate Registers
* Input          : None
* Output         : Angular Rate Registers buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetAngRateRaw(AngRateRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;
  
  if( !ReadReg(OUT_X_L, &valueL) )
      return MEMS_ERROR;
  if( !ReadReg(OUT_X_H, &valueH) )
      return MEMS_ERROR;
  
  buff->x = (i16_t)( (valueH << 8) | valueL );
  
  if( !ReadReg(OUT_Y_L, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(OUT_Y_H, &valueH) )
      return MEMS_ERROR;
  
  buff->y = (i16_t)( (valueH << 8) | valueL );
  
   if( !ReadReg(OUT_Z_L, &valueL) )
      return MEMS_ERROR;
  
  if( !ReadReg(OUT_Z_H, &valueH) )
      return MEMS_ERROR;
  
  buff->z = (i16_t)( (valueH << 8) | valueL );
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetInt1Src(u8_t* buff) {
  
  if( !ReadReg(INT1_SRC, buff) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : GetFifoSourceReg
* Description    : Read Fifo source Register
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t GetFifoSourceReg(u8_t* buff) {
  
  if( !ReadReg(FIFO_SRC_REG, buff) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : SetOutputDataAndFifoFilters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to output and fifo registers
*                : See Table 8 of AN3393 for more details
* Input          : NONE, HPH, LPF2, HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetOutputDataAndFifoFilters(HPF_LPF2_Enable hpf){
  u8_t value;
  
  //HPF
  if( !ReadReg(CTRL_REG5, &value) )
    return MEMS_ERROR;
  
  switch(hpf) {
    
  case NONE:
    value &= 0xfc;
    value |= 0x00; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 0
    break;
    
  case HPF:
    value &= 0xfc;
    value |= 0x01; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 1
    break;

  case LPF2:
    value &= 0xed;
    value |= 0x02; //hpen = 0, Out_sel_1 = 1, Out_sel_0 = x
    break;    
   
  case HPFLPF2:
    value &= 0xed;
    value |= 0x12; //hpen = 1, Out_sel_1 = 1, Out_sel_0 = x
    break;    
  }
  
  if( !WriteReg(CTRL_REG5, value) )
    return MEMS_ERROR;
  
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : SetInt1Filters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to Int1 circuitery
*                : See Table 9 of AN3393 for more details
* Input          : NONE, HPH, LPF2, HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetInt1Filters(HPF_LPF2_Enable hpf){
  u8_t value;
  
  //HPF
  if( !ReadReg(CTRL_REG5, &value) )
    return MEMS_ERROR;
  
  switch(hpf) {
    
  case NONE:
    value &= 0xf3;
    value |= 0x00<<INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 0
    break;
    
  case HPF:
    value &= 0xf3;
    value |= 0x01<<INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 1
    break;

  case LPF2:
    value &= 0xe7;
    value |= 0x02<<INT1_SEL0; //hpen = 0, Int1_sel_1 = 1, Int1_sel_0 = x
    break;    
   
  case HPFLPF2:
    value &= 0xe7;
    value |= 0x01<<HPEN;
    value |= 0x02<<INT1_SEL0; //hpen = 1, Int1_sel_1 = 1, Int1_sel_0 = x
    break;    
  }
  
  if( !WriteReg(CTRL_REG5, value) )
    return MEMS_ERROR;
  
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : SetSPIInterface
* Description    : Set SPI mode: 3 Wire Interface OR 4 Wire Interface
* Input          : SPI_3_WIRE, SPI_4_WIRE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t SetSPIInterface(SPIMode_t spi) {
  u8_t value;
  
  if( !ReadReg(CTRL_REG4, &value) )
    return MEMS_ERROR;
                  
  value &= 0xFE;
  value |= spi<<SIM;
  
  if( !WriteReg(CTRL_REG4, value) )
    return MEMS_ERROR;
  
  
  return MEMS_SUCCESS;
}


BYTE TestWhoAmI(void)
{
	uint32_t tmp_tx = 0x00000000;
	BYTE tmp_rx;
	
	tmp_tx = 0xA0080000 | WHO_AM_I | 0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x20080000 | 0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (BYTE)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return tmp_rx;
}


void set_pos_target(void)
{	
	switch(g_device_NO)
	{
		case(1):
			pos_target.x = 455;
			pos_target.y = -904;
			pos_target.z = 1200;
			break;
		case(2):
			pos_target.x = 468;
			pos_target.y = -1018;
			pos_target.z = 1066;
			break;
		case(3):
			pos_target.x = 478;
			pos_target.y = -997;
			pos_target.z = 945;
			break;
		case(4):
			pos_target.x = 451;
			pos_target.y = -963;
			pos_target.z = 995;
			break;	
	}
}


/*-----------------------------------------------------------------------*/
/* 陀螺仪置零   //叶川添加                                                       */
/*-----------------------------------------------------------------------*/
void reset_rev_data(void)
{
	rad.x=0;
	rad.y=0;	
	rad.z=0;	
}


/*-----------------------------------------------------------------------*/
/* 读取陀螺仪数据                                                                   */
/* 读出返回0                                                                          */
/*-----------------------------------------------------------------------*/
SWORD read_rev_data(void)
{
	u8_t status;
	if (MEMS_SUCCESS == GetSatusReg(&status))
	{
		if (status & 80)
		{
			GetAngRateRaw(&rev);	
			
			rev.x/=1000;
			rev.y/=500;
			rev.z/=500;
			rad.x+=rev.x;
			rad.y+=rev.y;	
			rad.z+=rev.z;
#if 0
			LCD_PrintoutInt(32, 0, rev.x);
			LCD_PrintoutInt(32, 2, rev.y);
			LCD_PrintoutInt(32, 4, rev.z);
#endif
			return 0;
		}
		return 1;
	}
	
	return 2;
}

/* 由陀螺仪控制漂移 */
int control_steer_helm_2(int d_flag)
{
	static int error_count=0,i=0;
	int error=0,Kp=4,Kd=15,start_flag=1,steer_rate=0;
	static int steer_pwm=0,rev_count=0,cnt=0;
	error=pos_target.z-rad.z;
	/* 0.5s后为第二阶段 */
	if(diff_time_basis_PIT(g_time_basis_PIT,start_time)>0x00000032&&cl_flag==1)
	{
		if(d_flag==1)
		{
			set_speed_target(140);
			set_steer_helm((WORD)(data_steer_helm.left_limit));
			cl_flag=2;	
		}
		if(d_flag==2)
		{
			set_speed_target(140);
			set_steer_helm((WORD)(data_steer_helm.left_limit));
			cl_flag=2;	
		}
		if(d_flag == 3)
		{
			set_speed_target(160);
			set_steer_helm((WORD)(data_steer_helm.left_limit));
			cl_flag=2;	
		}
	}
	/* 判断开始漂移（z轴转过9）为第三阶段 */
	else if(rad.z>=(pos_target.z/10)&&cl_flag==2)
	{
		set_steer_helm((WORD)(-600));	
		set_speed_target(80);
		cl_flag=3;	
	}
	/* 5~50为第四阶段 */
	else if(rad.z>=(pos_target.z/9)&&cl_flag==3)
	{	
		set_steer_helm((WORD)(-100));	
		set_speed_target(60);
		cl_flag=4;	
	}
	/* 判断漂移过40度后为第五阶段 */
	else if(rad.z>(pos_target.z*4/9)&&cl_flag==4)
	{						
		set_steer_helm((WORD)(200));	
		cl_flag=5;
		set_speed_target(40);
	}
	/* 判断漂移过80度为第六阶段 */
	else if(rad.z>(pos_target.z*8/9)&&cl_flag==5)
	{
		cl_flag=6;
	}
	if(abs(error)>=1&&cl_flag==6)
	{
		steer_rate = (Kp*error+Kd*error_count);
		error_count = rev.z;
		steer_pwm = -steer_rate;
		set_steer_helm((WORD)(steer_pwm));	
		set_speed_target(0);
	}					
	if(abs(error)<=200)
	{
		i++;
	}
	if(i!=0)
	{
		set_speed_target(10);
	}
	rev_count+=abs(error);
	cnt++;
	if(cnt==10)
	{
		rev_count=0;
	}
	if((cnt==9&&abs(rev_count)<=20||diff_time_basis_PIT(g_time_basis_PIT,start_time)>=0x000000C8)&&d_flag!=2)
	{
		start_flag=0;
		error_count=0;
		i=0;
		rev_count=0;
		cnt=0;
	}
	if((cnt==9&&abs(rev_count)<=20||diff_time_basis_PIT(g_time_basis_PIT,start_time)>=0x000000C8)&&d_flag==2)
	{
		cl_flag=7;
		set_steer_helm(0);
		set_speed_target(-10); 
	}
	if(cl_flag==7&&diff_time_basis_PIT(g_time_basis_PIT,start_time)>=0x0000012C&&d_flag==2)
	{
		start_flag=0;
		error_count=0;
		i=0;
		rev_count=0;
		cnt=0;
	}
	return start_flag;
	if (g_remote_control_flags.send_gyro_data)
	{
		//generate_remote_frame(WIFI_CMD_GET_GYRO_DATA, (BYTE *)&rev, sizeof(rev));
	}
}


//由陀螺仪控制转向角度
int control_steer_helm_3(int angle_1)
{
	static int error_count=0,i=0;
	int error=0,Kp=6,Kd=3,start_flag=1,steer_rate=0,angle_base=0;
	static int steer_pwm=0;
	angle_base = angle_1*pos_target.z/90;
	error=angle_base-rad.z;
//	LCD_PrintoutInt(0, 6, pos_target.z);
	if(abs(error)>=1)
	{
		steer_rate = (Kp*error+Kd*error_count);
		error_count = rev.z;
		steer_pwm = -steer_rate;
		set_steer_helm((WORD)(steer_pwm));	 
	}
	if((abs(error))<=1)
	{
		start_flag=0;
	}
	if(diff_time_basis_PIT(g_time_basis_PIT,start_time)>0x00000190 && find_mag_back_box_2 == 1)
	{
		start_flag=0;
	}
	return start_flag;
	if (g_remote_control_flags.send_gyro_data)
	{
		//generate_remote_frame(WIFI_CMD_GET_GYRO_DATA, (BYTE *)&rev, sizeof(rev));

	}
}
//判断是否稳定
int check_stable(void)
{
	static int cnt = 0,rad_cnt_z = 0;
	cnt++;
	rad_cnt_z+=rev.z;
	if(cnt==4&&rad_cnt_z<=5)
	{
		return 0;
	}
	else if(cnt==5)
	{
		rad_cnt_z = 0;
		cnt = 0;
		return 1;	
	}
}

//由陀螺仪控制上坡加速下坡减速
void control_speed_target_1(int speed)
{
	static int speed_1=0,speed_2=0;
	static fly_flag = 0,single_bridge_flag = 0,normal_flag = 0;
	static int cnt = 0,rad_cnt_z = 0;
	static int max = -100;
	/* 钢丝桥 */
	if(g_f_enable_single_bridge_control==0&&g_f_enable_fly_bridge==0)
	{
		if(normal_flag==0)
		{
			speed_1=10-speed;
			if(rad.y<-10)
				normal_flag=1;
		}
		 
		else if(normal_flag==1)
		{
			speed_1 =10 -speed;
			cnt++;
			rad_cnt_z+=rev.z;
			if(cnt==4&&rad_cnt_z<=5)
			{
				normal_flag = 2;
			}
			else if(cnt==5)
			{
				rad_cnt_z = 0;
				cnt = 0;
			}	
		}
		else if(rad.y<-80&&normal_flag==2)
		{
			speed_1 = 5;
		}
		else if(rad.y>-10&&normal_flag==2)
		{
			speed_1=0;
		}
	}
	/* 单边桥 */
	if(g_f_enable_single_bridge_control)
	{
		if(single_bridge_flag == 0)
		{
			speed_1 =10 -speed;
			cnt++;
			rad_cnt_z+=rev.z;
			if(cnt==4&&rad_cnt_z<=5)
			{
				single_bridge_flag = 1;
			}
			else if(cnt==5)
			{
				rad_cnt_z = 0;
				cnt = 0;
			}	
		}
		if(single_bridge_flag ==1)
		{
			speed_1 = 0;
			single_bridge_flag = 2;
		}
		if(rad.y>100&&single_bridge_flag==2)
		{
			rad_cnt_z = 0;
			cnt = 0;
			speed_1 = -15;
			single_bridge_flag=3;
		}
		if(rad.y>200&&single_bridge_flag==3)
		{
			speed_1 = -25;
			single_bridge_flag=4;
		}
		if(abs(rad.z)>50)
		{
			speed_2 = -10;
		}
		else
		{
			speed_2 = 0;
		}		
	}
	/* 飞桥 */
	if(g_f_enable_fly_bridge)
	{	
		if(fly_flag == 0)
		{
			speed_1 =10 -speed;
			cnt++;
			rad_cnt_z+=rev.z;
			if(cnt==14&&rad_cnt_z<=14)
			{
				fly_flag = 1;
			}
			else if(cnt==15)
			{
				rad_cnt_z = 0;
				cnt = 0;
			}	
		}
		if(fly_flag==1)
		{
			g_f_enable_fly_bridge=2;
			rad_cnt_z = 0;
			cnt = 0;
			speed_1 = 0;
			fly_flag = 2;
			set_steer_helm(0);
			
		}
		if(rad.y<-30&&fly_flag==2)
		{
			speed_1 = 150;
			fly_flag = 3;
		}
		if(rad.y>0&&fly_flag ==3)
		{
			speed_1 =10 -speed;
			set_steer_helm(0);
			fly_flag=5;
		}
		/*
		if(rad.y>max&&(fly_flag==3))
		{
			max = rad.y;
			LCD_PrintoutInt(0, 0,(SWORD)max);	
		}
		*/
	}
//	LCD_PrintoutInt(0, 0, (SWORD)(speed+speed_1+speed_2));
	set_speed_target((SWORD)(speed+speed_1+speed_2));
	if(fly_flag ==5) 
	{
		g_f_enable_fly_bridge=0;
		g_f_enable_speed_control_2=0;
		speed_1=0;
		speed_2=0;
		fly_flag=0;
	}
}
