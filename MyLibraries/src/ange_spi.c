#include "ange_spi.h"
#include "ange_usart.h"
#define ange_SPI_timeout 1000
//ע�Ⲣδ������֤
void ange_SPI_GPIO_Congfig()
{
	GPIO_InitTypeDef gpioinit;
	//////����spi��ʱ��
	ange_SPI_APBxClock(ange_SPI_CLK,ENABLE);
	///����gpioʱ��
	ange_SPI_CS_APBxClock(ange_SPI_CS_CLK|ange_SPI_SCK_CLK|ange_SPI_MISO_CLK|ange_SPI_MISI_CLK,ENABLE);
	///cs
	gpioinit.GPIO_Mode=GPIO_Mode_Out_PP;  //������Ҫ�޸�
	gpioinit.GPIO_Pin=ange_SPI_CS_Pin;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_SPI_CS_PORT,&gpioinit);
	//
	gpioinit.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioinit.GPIO_Pin=ange_SPI_SCK_Pin;
	GPIO_Init(ange_SPI_SCK_PORT,&gpioinit);
	///
	gpioinit.GPIO_Pin=ange_SPI_MISO_Pin;
	GPIO_Init(ange_SPI_MISO_PORT,&gpioinit);
	//
	gpioinit.GPIO_Pin=ange_SPI_MISI_Pin;
	GPIO_Init(ange_SPI_MISI_PORT,&gpioinit);
}
void ange_SPI_Config()
{
	SPI_InitTypeDef spiinit;
	spiinit.SPI_Mode=SPI_Mode_Master;  //spi��ģʽ ��Ϊ��master ��slave
	spiinit.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //���䷽��
	spiinit.SPI_DataSize=SPI_DataSize_8b;  //������ʽ 8bits ��16bits
	spiinit.SPI_CPOL=SPI_CPOL_High;    //sck���е�ƽ
	spiinit.SPI_CPHA=SPI_CPHA_2Edge;   //���ݲ������ڵ�һ�����ǵڶ����仯��ƽ
	spiinit.SPI_NSS=SPI_NSS_Soft;   //cs�������������Ӳ������
	spiinit.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;  //������Ԥ��Ƶ  spi1����APB2������
	spiinit.SPI_FirstBit=SPI_FirstBit_MSB;  //С��ģʽ���Ǵ��ģʽ
	spiinit.SPI_CRCPolynomial=7;   //crc 
	SPI_Init(ange_SPIx,&spiinit);
	SPI_Cmd(ange_SPIx,ENABLE);
}
void ange_SPI_ERROR(u8 x)
{
	printf("spi����%d",x);
}
u8 ange_SPI_sendbyte(u8 x)
{
	u16 t=ange_SPI_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPIx,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI_ERROR(0);
			return 0xff;
		}
	}
	SPI_I2S_SendData(ange_SPIx,x);
	t=ange_SPI_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPIx,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI_ERROR(1);
			return 1;
		}
	}
	x=SPI_I2S_ReceiveData(ange_SPIx);
	return x;
}
u16 ange_SPI_send2byte(u16 x)
{
	u16 t=ange_SPI_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPIx,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI_ERROR(0);
			return 0xffff;
		}
	}
	SPI_I2S_SendData(ange_SPIx,x);
	t=ange_SPI_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPIx,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI_ERROR(1);
			return 1;
		}
	}
	x=SPI_I2S_ReceiveData(ange_SPIx);
	return x;
}
///
#define ange_SPI2_timeout 1000
//ע�Ⲣδ������֤
void ange_SPI2_GPIO_Congfig()
{
	GPIO_InitTypeDef gpioinit;
	//////����spi��ʱ��
	ange_SPI2_APBxClock(ange_SPI2_CLK,ENABLE);
	///����gpioʱ��
	ange_SPI2_CS_APBxClock(ange_SPI2_CS_CLK|ange_SPI2_SCK_CLK|ange_SPI2_MISO_CLK|ange_SPI2_MISI_CLK,ENABLE);
	///cs
	gpioinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioinit.GPIO_Pin=ange_SPI2_CS_Pin;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_SPI2_CS_PORT,&gpioinit);
	//
	gpioinit.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioinit.GPIO_Pin=ange_SPI2_SCK_Pin;
	GPIO_Init(ange_SPI2_SCK_PORT,&gpioinit);
	///
	gpioinit.GPIO_Pin=ange_SPI2_MISO_Pin;
	GPIO_Init(ange_SPI2_MISO_PORT,&gpioinit);
	//
	gpioinit.GPIO_Pin=ange_SPI2_MISI_Pin;
	GPIO_Init(ange_SPI2_MISI_PORT,&gpioinit);
}
void ange_SPI2_Config()
{
	SPI_InitTypeDef spiinit;
	spiinit.SPI_Mode=SPI_Mode_Master;  //spi��ģʽ ��Ϊ��master ��slave
	spiinit.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //���䷽��
	spiinit.SPI_DataSize=SPI_DataSize_8b;  //������ʽ 8bits ��16bits
	spiinit.SPI_CPOL=SPI_CPOL_High;    //sck���е�ƽ
	spiinit.SPI_CPHA=SPI_CPHA_2Edge;   //���ݲ������ڵ�һ�����ǵڶ����仯��ƽ
	spiinit.SPI_NSS=SPI_NSS_Soft;   //cs�������������Ӳ������
	spiinit.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;  //������Ԥ��Ƶ  spi1����APB2������
	spiinit.SPI_FirstBit=SPI_FirstBit_MSB;  //С��ģʽ���Ǵ��ģʽ
	spiinit.SPI_CRCPolynomial=7;   //crc 
	SPI_Init(ange_SPI2x,&spiinit);
	SPI_Cmd(ange_SPI2x,ENABLE);
}
void ange_SPI2_ERROR(u8 x)
{
	printf("spi����%d",x);
}
u8 ange_SPI2_sendbyte(u8 x)
{
	u16 t=ange_SPI2_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPI2x,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI2_ERROR(0);
			return 0xff;
		}
	}
	SPI_I2S_SendData(ange_SPI2x,x);
	t=ange_SPI2_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPI2x,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI2_ERROR(1);
			return 1;
		}
	}
	x=SPI_I2S_ReceiveData(ange_SPI2x);
	return x;
}
u16 ange_SPI2_send2byte(u16 x)
{
	u16 t=ange_SPI2_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPI2x,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI2_ERROR(0);
			return 0xffff;
		}
	}
	SPI_I2S_SendData(ange_SPI2x,x);
	t=ange_SPI2_timeout;
	while(SPI_I2S_GetFlagStatus(ange_SPI2x,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((t--)==0)
		{
			ange_SPI2_ERROR(1);
			return 1;
		}
	}
	x=SPI_I2S_ReceiveData(ange_SPI2x);
	return x;
}
