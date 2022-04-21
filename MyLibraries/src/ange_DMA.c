#include "ange_dma.h"
#if ange_DMA_CC
//DMA�洢�����洢����
//ange_DMA_CC_Config() //������һ������Ϊ�洢��ԭʼ���ݵ�ַ���ڶ���ΪĿ���ַ
void ange_DMA_CC_Config( u32* y,u32 *m)//�洢�������洢��
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_DMA_CC_Clock,ENABLE);
	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
	dmainit.DMA_PeripheralBaseAddr=(u32)y;
	dmainit.DMA_MemoryBaseAddr=(u32)m;
	dmainit.DMA_BufferSize=ange_DMA_CC_BufferSize;
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Priority=DMA_Priority_High;
	dmainit.DMA_M2M=DMA_M2M_Enable;
	DMA_Init(ange_DMA_CC_Channel,&dmainit);
	DMA_Cmd(ange_DMA_CC_Channel,ENABLE);
}
#endif
#if ange_DMA_CW
//DMA�洢�������裬���ڴ���������DMAͨ�����ڴ����ȥ
//��ʹ��ʱҪ�ȳ�ʼ�����������ݣ�ange_DMA_CW_USARTConfig�ĵ�һ���������ڴ��ַ���ڶ�������Ϊ�������ݼĴ�����ַ
void ange_DMA_CW_USARTConfig(u8* y,u32 m)//�洢��������
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_DMA_CW_Clock,ENABLE);
	dmainit.DMA_DIR=DMA_DIR_PeripheralDST;  
	dmainit.DMA_PeripheralBaseAddr=(u32)m;
	dmainit.DMA_MemoryBaseAddr=(u32)y;
	dmainit.DMA_BufferSize=ange_DMA_CW_BufferSize;
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Mode=DMA_Mode_Circular;
	dmainit.DMA_Priority=DMA_Priority_Medium;
	dmainit.DMA_M2M=DMA_M2M_Disable;
	DMA_Init(ange_DMA_CW_Channel,&dmainit);
	DMA_Cmd(ange_DMA_CW_Channel,ENABLE);
}
#endif
#if ange_DMA_WC
//�����ڽ������ݣ����䵽�ڴ��У�
/* ange_DMA_WC_USARTConfig((u32)&USART1->DR,u8 *m) //���ڵ��ڴ�
ʹ��ʱ�ȳ�ʼ�����ڲ��������ڿ����жϣ�����ʵ���ⲿһ֡һ֡���ݵĽ���
DMA_SetCurrDataCounter��������ʵ��DMA���´��׵�ַ��ʼ����
����ʹ��˳��
1.ange_USART_init();
2.NVIC�жϳ�ʼ��
2.USART_ITConfig(���ں�,USART_IT_IDLE,ENABLE);
3.USART_DMACmd()
*/
//extern u8 jieshouzu[ange_DMA_BufferSize];
//void ange_USART_IRQHandler() //dma����  //�˺���Ŀǰ�ڴ���C�ļ���
//{
//	u8 x;
//	if(USART_GetITStatus(ange_USARTx,USART_IT_IDLE)!=RESET)
//	{
//		DMA_Cmd(ange_DMA_Channel,DISABLE);
//		x=DMA_GetCurrDataCounter(ange_DMA_Channel);
//		DMA_SetCurrDataCounter(ange_DMA_Channel,ange_DMA_BufferSize);
//		ange_usart_sendnumstr(ange_USARTx,jieshouzu,ange_DMA_BufferSize-x);
//		DMA_Cmd(ange_DMA_Channel,ENABLE);
//		USART_ReceiveData(ange_USARTx);
//		USART_ClearFlag(ange_USARTx,USART_FLAG_IDLE);
//	}
//}
void ange_DMA_WC_USARTConfig(u32 y,u8 *m)//���赽�洢����8λ���� //
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_DMA_WC_Clock,ENABLE);
	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
	dmainit.DMA_PeripheralBaseAddr=(u32)y;   //�����ַ
	dmainit.DMA_MemoryBaseAddr=(u32)m;   //�ڴ��ַ
	dmainit.DMA_BufferSize=ange_DMA_WC_BufferSize;  //dma���ݴ�����
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //�����ַ������
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;  //�ڴ��ַ�Ƿ����
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;   //�������ݿ��
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;  //�ڴ����ݿ��
//	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Mode=DMA_Mode_Circular;  //ѭ����������
	dmainit.DMA_Priority=DMA_Priority_Medium;  //dma���ȼ� ����low medium high veryhigh
	dmainit.DMA_M2M=DMA_M2M_Disable;   //�洢�����洢��ʱѡ��
	DMA_Init(ange_DMA_WC_Channel,&dmainit);  //��ʼ����Ҫѡ��ͨ��
	DMA_Cmd(ange_DMA_WC_Channel,ENABLE);
}
/*
AD,DMA����,����ģʽ���ֽ����ݣ��Ѿ���ֲ��AD��C�ļ�
*/
void ange_DMA_WC_ADCConfig(u32 y,u16 *m)//���赽�洢����16λ����
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_DMA_WC_Clock,ENABLE);
	DMA_DeInit(ange_DMA_WC_Channel);
	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
	dmainit.DMA_PeripheralBaseAddr=(u32)y;   //�����ַ
	dmainit.DMA_MemoryBaseAddr=(u32)m;   //�ڴ��ַ
	dmainit.DMA_BufferSize=ange_DMA_WC_BufferSize;  //dma���ݴ�����
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //�����ַ������
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;  //�ڴ��ַ�Ƿ����
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;   //�������ݿ��
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;  //�ڴ����ݿ��
//	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Mode=DMA_Mode_Circular;  //ѭ����������
	dmainit.DMA_Priority=DMA_Priority_Medium;  //dma���ȼ� ����low medium high veryhigh
	dmainit.DMA_M2M=DMA_M2M_Disable;   //�洢�����洢��ʱѡ��
	DMA_Init(ange_DMA_WC_Channel,&dmainit);  //��ʼ��ʱҪѡ��ͨ��
	DMA_Cmd(ange_DMA_WC_Channel,ENABLE);
}
#endif
