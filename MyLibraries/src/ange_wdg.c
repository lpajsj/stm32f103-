#include "ange_wdg.h"
#include "bsp_led.h"  
//Ӳ�����Ź���λ����Զ���ʼ����
/*�������Ź�*/
/*
�������������Ź���ʼ����
������  
presalder:�������Ź�Ԥ��Ƶ LSI 40khz/presaler
  *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
  *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
  *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
  *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
  *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
  *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
  *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
	counter: Ԥװ��ֵ
����ֵ����
*/
//��������RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET�ж��Ƿ���IWDG��λ
//��λ���Ź���ʧЧ���������ú���Ч��iwdg���ÿ�ʱ�� 
void ange_iwdg_init(u8 presaler,u16 counter)
{
	//�������Ź��ڸı�Ԥ��Ƶ�Լ�������ֵʱһ��Ҫ���ڼ��Ĵ�����д��0x55
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//����LSI���������Ź���ʱ�ӷ�Ƶ���ӣ�LSIʱ��Ϊ40KHZ
	IWDG_SetPrescaler(presaler);
	//���ü�����Ԥװ��ֵ���ֵ��0-4095
	IWDG_SetReload(counter);
	//Ҫ��ֵ���ص�������
	IWDG_ReloadCounter();
	//ʹ��IWDG
	IWDG_Enable();
}
void ange_iwdg_feed()
{
	IWDG_ReloadCounter();
}
/*���ڿ��Ź�*/
//���ڿ��Ź��ڼ���������0x40ʱ������жϣ�Ȼ��λ�����������ֵ�ڴ��ڴ���ֵʱ�����¸�ֵ��Ҳ�ᷢ����λ
//����ֵ��CFR�Ĵ����ĵ���λ��������ֵ��CR�Ĵ����ĵ���λ//û�м���Ԥװ��
void ange_wwdg_nvic_init()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvicinit.NVIC_IRQChannel=WWDG_IRQn;
	nvicinit.NVIC_IRQChannelCmd=ENABLE;
	nvicinit.NVIC_IRQChannelPreemptionPriority=1;
	nvicinit.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvicinit);
}
//���ڿ��Ź���λ����,ֻ�����־���ǻḴλ
//��λ�������������������������ü�������ֵ�������÷�������츴λ
// WWDG �жϸ����������������˴��жϣ���ʾ�����Ѿ������˹��ϣ�
// ����һ����ǰ�жϡ��ڴ��жϷ��������Ӧ�ø�����Ҫ���£�
// ���籣����Ҫ�����ݵȣ����ʱ������ж೤��Ҫ
// ��WDGTB��ֵ������
// WDGTB:0   113us
// WDGTB:1   227us
// WDGTB:2   455us
// WDGTB:3   910us
void WWDG_IRQHandler()
{
	WWDG_ClearFlag();
//	LED_GREEN;
}
/*
���������ڿ��Ź���ʼ��������NVIC���жϿ���
������
prescaler��wwdg��ʱ��ΪAPB1clk/4096/prescaler
  *     @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1/4096)/1  113us  
  *     @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1/4096)/2  227us
  *     @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1/4096)/4  455us
  *     @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1/4096)/8  910us
counter:��������ֵ
window������ֵ����������ֵ�����ڴ���ֵʱ������װ�ؽ��ᷢ����λ
����ֵ�� ��
wr=WWDG->CFR&0x7f; //ȡ���ڿ��ſڵ�ֵ65-127
*/
void ange_wwdg_init(u32 prescaler,u8 counter,u8 window)
{
	//�������ڿ��Ź�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	//����Ԥ��Ƶ
	WWDG_SetPrescaler(prescaler);
	//������
	WWDG_SetCounter(counter);
	//����ֵ
	WWDG_SetWindowValue(window);
	ange_wwdg_nvic_init();
	//ʹ��,�����︳ֵʱ���ڴ�ʱ��ֵ������ֵ���ڴ���ֵ�����ֱ�ӽ��жϱ�־λ��һ
	WWDG_Enable(counter);
	WWDG_ClearFlag();
		//����0x40�ж�
	WWDG_EnableIT();
}
//��ȡ��λ״̬��������������
//	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)
//	{
//		LED_RED;
//	}
//	else
//	{
//		LED_BLUE;
//	}
//	RCC_ClearFlag();
//	ange_wwdg_init(WWDG_Prescaler_8,0x7f,0x5f);
//	wr=WWDG->CFR&0x7f;
//    while (1)
//		{
////			tr=WWDG->CR&0x7f;
////			if(tr<wr)
////			{
////					LED_GREEN;
//			SysTick_delayms(40);  
//					WWDG_SetCounter(0x7f);
////			}
//		}


