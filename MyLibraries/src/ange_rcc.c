#include "ange_rcc.h"
//���������Ŀ�ͷ����
/*�������Ϊ���໷ʱ�ӱ�Ƶ*/
void HSE_SetSysClock(u32 RCC_PLLMul_x)
{
	__IO uint32_t HSEStartUpStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
			
		}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource()!=0x08);
	}
	else
	{
		while(1);
	}
}
//ʹ���ڲ�HSIʱ����Ϊʱ����Դ
void HSI_SetSysClock(u32 RCC_PLLMul_x)
{
	__IO uint32_t HSIStartUpStatus;
	RCC_DeInit();
	RCC_HSICmd(ENABLE);
//	HSIStartUpStatus=GetFlagStatus(RCC_FLAG_HSIRDY);
	HSIStartUpStatus=RCC->CR&RCC_CR_HSIRDY;
	if(HSIStartUpStatus==RCC_CR_HSIRDY)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ʹ��flashԤ��ȡ������
		FLASH_SetLatency(FLASH_Latency_2); //����sysclk������flash����ʱ��ı�����Ƶ�ʸߵ�ͬʱ���������õ�̫С
	RCC_HCLKConfig(RCC_SYSCLK_Div1);   //����hclkԤ��Ƶ
	RCC_PCLK2Config(RCC_HCLK_Div1);    //APB2Ԥ��Ƶ
	RCC_PCLK1Config(RCC_HCLK_Div2);    //APB1Ԥ��Ƶ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_x); //����PLLʱ����Դ���ͱ�Ƶ����
	RCC_PLLCmd(ENABLE); //����PLL
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET) //�ȴ�PLLʱ���ȶ�
		{
			
		}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //ѡ��PLLΪϵͳʱ��
	while(RCC_GetSYSCLKSource()!=0x08); //��ȡʱ���л�״̬λ��ȷ��PLLCLKΪϵͳʱ��
	}
	else
	{
		while(1);
	}
}

