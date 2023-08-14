#include "spi.h"
#include "global.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "xprintf.h"

#define SPI2_MOSI_PIN			GPIO_Pin_15
#define SPI2_MOSI_PORT			GPIOB
#define SPI2_MOSI_AF			GPIO_AF_0
#define SPI2_MOSI_SOURCE		GPIO_PinSource15

#define SPI2_MISO_PIN			GPIO_Pin_14
#define SPI2_MISO_PORT			GPIOB
#define SPI2_MISO_AF			GPIO_AF_0
#define SPI2_MISO_SOURCE		GPIO_PinSource14

#define SPI2_SCK_PIN			GPIO_Pin_13
#define SPI2_SCK_PORT			GPIOB
#define SPI2_SCK_AF				GPIO_AF_0
#define SPI2_SCK_SOURCE			GPIO_PinSource13

#define SPI2_CS_PIN 			GPIO_Pin_12
#define SPI2_CS_PORT 			GPIOB

#define SPI1_CS_PIN 			GPIO_Pin_12	
#define SPI1_CS_PORT 			GPIOC

#define SPI1_CS_LOW()       	GPIO_ResetBits(SPI1_CS_PORT, SPI1_CS_PIN)
#define SPI1_CS_HIGH()     	 	GPIO_SetBits(SPI1_CS_PORT, SPI1_CS_PIN)

#define SPI2_CS_LOW()       	GPIO_ResetBits(SPI2_CS_PORT, SPI2_CS_PIN)
#define SPI2_CS_HIGH()      	GPIO_SetBits(SPI2_CS_PORT, SPI2_CS_PIN)

#define SPI_DUMMY_BYTE         0x00


static void SPI1_Init(void);
static void SPI2_Init(void);
static uint8_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t byte, uint16_t timeout);
static uint8_t SPI_readByte(SPI_TypeDef *SPIx, uint8_t*byte, uint16_t timeout);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void SPIx_init(SPI_TypeDef *SPIx){
	if(SPIx == SPI1)
			SPI1_Init();
	if(SPIx == SPI2)
			SPI2_Init();
	//SPI2->CR2|=SPI_CR2_FRXTH;
	//SPI2->CR2&=~SPI_CR2_FRXTH;
}


int SPIx_ReadRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){
	
	reg |= 0x80;
	
	if(SPIx == SPI1){
		SPI1_CS_LOW();
	}
	else
	{
		SPI2_CS_LOW();
	}

	if(!SPI_SendByte(SPIx,reg,timeout)){
		xprintf("time out \r\n");
		return 0;
	}
	
	while(len){
		
		if(!SPI_readByte(SPIx,data, timeout)){
			xprintf("time out \r\n");
			return 0;
		}
		len--;
		data++;
	}
	
	if(SPIx == SPI1)
	{
		SPI1_CS_HIGH();
	}
	else
	{
		SPI2_CS_HIGH();	
	}
	
	return 1;
}
#if 0
int SPIx_ReadRegister_t(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){

	reg |= 0x80;

	if(SPIx == SPI1){
		SPI1_CS_LOW();
	}
	else
	{
		SPI2_CS_LOW();
	}

	uint32_t last_time =0;
  /*!< Loop while DR register in not emplty */
	last_time = ulMiliCount+timeout;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}

  /*!< Send byte through the SPI1 peripheral */
  SPI_SendData8(SPIx, reg);
  /*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}
  SPI_ReceiveData8(SPIx);
  while(len){
	last_time = ulMiliCount+timeout;
while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}
/*!< Send byte through the SPI1 peripheral */
SPI_SendData8(SPIx, SPI_DUMMY_BYTE);

/*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}

*data=SPI_ReceiveData8(SPIx);
data++;
len--;
  }
	if(SPIx == SPI1)
	{
		SPI1_CS_HIGH();
	}
	else
	{
		SPI2_CS_HIGH();
	}

	return 1;
}

int SPIx_WriteRegister_t(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){

	if(SPIx == SPI1){
		SPI1_CS_LOW();
	}
	else
	{
		SPI2_CS_LOW();
	}

	uint32_t last_time =0;
  /*!< Loop while DR register in not emplty */
	last_time = ulMiliCount+timeout;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}

  /*!< Send byte through the SPI1 peripheral */
  SPI_SendData8(SPIx, reg);
  /*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}
  SPI_ReceiveData8(SPIx);
  while(len){
	last_time = ulMiliCount+timeout;
while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}

/*!< Send byte through the SPI1 peripheral */
SPI_SendData8(SPIx, *data);

/*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){
		if(last_time<ulMiliCount)
			return 0;
	}

	SPI_ReceiveData8(SPIx);
data++;
len--;
  }
	if(SPIx == SPI1)
	{
		SPI1_CS_HIGH();
	}
	else
	{
		SPI2_CS_HIGH();
	}

	return 1;
}
#endif

int SPIx_WriteRegister(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint16_t len, uint16_t timeout){
	
	if(SPIx == SPI1)
		SPI1_CS_LOW();
	else
		SPI2_CS_LOW();	
	
	if(!SPI_SendByte(SPIx,reg,timeout)){
		xprintf("time out \r\n");
		return 0;
	}
	while(len){
		
		if(!SPI_SendByte(SPIx,*data, timeout)){
			xprintf("time out \r\n");
			return 0;
		}
		len--;
		data++;
	}
	
	if(SPIx == SPI1)
		SPI1_CS_HIGH();
	else
		SPI2_CS_HIGH();

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI1_Init(void){
}

static void SPI2_Init(void) {
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure SCK and MOSI pins as Alternate Function Push Pull */

	  /* Configure SD_SPI pins: SCK */
	  GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_Init(SPI2_SCK_PORT, &GPIO_InitStructure);

	  /* Configure SD_SPI pins: MISO */
	  GPIO_InitStructure.GPIO_Pin = SPI2_MISO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	  GPIO_Init(SPI2_MISO_PORT, &GPIO_InitStructure);

	  /* Configure SD_SPI pins: MOSI */
	  GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	  GPIO_Init(SPI2_MOSI_PORT, &GPIO_InitStructure);

	  /* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
	  GPIO_InitStructure.GPIO_Pin = SPI2_CS_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	  GPIO_Init(SPI2_CS_PORT, &GPIO_InitStructure);

	  /* Connect PXx to SD_SPI_SCK */
	  GPIO_PinAFConfig(SPI2_SCK_PORT, SPI2_SCK_SOURCE, SPI2_SCK_AF);
	  /* Connect PXx to SD_SPI_MISO */
	  GPIO_PinAFConfig(SPI2_MISO_PORT, SPI2_MISO_SOURCE, SPI2_MISO_AF);
	  /* Connect PXx to SD_SPI_MOSI */
	  GPIO_PinAFConfig(SPI2_MOSI_PORT, SPI2_MOSI_SOURCE, SPI2_MOSI_AF);

	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	 SPI_InitStructure.SPI_CRCPolynomial = 7;
	 SPI_Init(SPI2, &SPI_InitStructure);

	 SPI_Cmd(SPI2, ENABLE); /* SD_SPI enable */


	 xprintf("spi2 init done\r\n");
}

static uint8_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t byte, uint16_t timeout)
{
	uint32_t last_time =0;
  /*!< Loop while DR register in not emplty */
	last_time = ulMiliCount+timeout;
  while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) && (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)){
		if(last_time<ulMiliCount){
		xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}

  /*!< Send byte through the SPI1 peripheral */
  SPI_SendData8(SPIx, byte);
 // SPIx->DR=byte;
  /*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
  while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) && (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)){
		if(last_time<ulMiliCount){
			xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}
  //SPIx->DR;
 SPI_ReceiveData8(SPIx);
		return 1;
}

static uint8_t SPI_readByte(SPI_TypeDef *SPIx, uint8_t*byte, uint16_t timeout){
	uint32_t last_time =0;
  /*!< Loop while DR register in not emplty */
	last_time = ulMiliCount+timeout;
  while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) && (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)){
		if(last_time<ulMiliCount){
			xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}

  /*!< Send byte through the SPI1 peripheral */
  SPI_SendData8(SPIx, SPI_DUMMY_BYTE);
  //SPIx->DR=SPI_DUMMY_BYTE;
  /*!< Wait to receive a byte */
	last_time = ulMiliCount+timeout;
  while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) && (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)){
		if(last_time<ulMiliCount){
			xprintf("%s time out \r\n",__func__);
			return 0;
		}
	}

  //*byte=(uint8_t)( SPIx->DR);
  *byte=SPI_ReceiveData8(SPIx);
	return 1;
}

