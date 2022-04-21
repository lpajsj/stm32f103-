#include "ange_timj.h"
uint32_t time=0;
static void ange_TIMj_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
	nvicinit.NVIC_IRQChannel=ange_TIMj_IRQ;
	nvicinit.NVIC_IRQChannelCmd=ENABLE;
	nvicinit.NVIC_IRQChannelPreemptionPriority=1;
	nvicinit.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvicinit);
}
static void ange_TIMj_Config()
{
	TIM_TimeBaseInitTypeDef timinit;    
	ange_TIMj_APBxClock(ange_TIMj_CLK,ENABLE);     //timʱ����apb1
	timinit.TIM_ClockDivision=TIM_CKD_DIV1;   //ckint,�������������Լ������˲�������ʱ��Ƶ�ʷ�Ƶ�� ������ʱ����
	timinit.TIM_Prescaler=71;  //ck_psc��Ƶ
	timinit.TIM_CounterMode=TIM_CounterMode_Up;  //��ʱ��������ʽ��������ʱ��ֻ������ģʽ
	timinit.TIM_Period=1000-1;     //��������װֵ
	timinit.TIM_RepetitionCounter=0;  //�ظ���������ֻ���ڸ߼���ʱ��
	TIM_TimeBaseInit(ange_TIMjx,&timinit);//��ʱ��������ʼ��
	TIM_ClearFlag(ange_TIMjx,TIM_FLAG_Update); //��������־
	TIM_ITConfig(ange_TIMjx,TIM_IT_Update,ENABLE); //ʹ������ж�
	TIM_Cmd(ange_TIMjx,ENABLE);  //ʹ��TIM
}

void ange_TIMj_init()
{
	ange_TIMj_NVIC_Config();
	ange_TIMj_Config();
//	TIM_SelectOutputTrigger(ange_TIMjx,TIM_TRGOSource_Update);
}
//void ange_TIMj_IRQHandler()
//{
//	if(TIM_GetITStatus(ange_TIMjx,TIM_IT_Update)==SET)
//	{
//		TIM_ClearITPendingBit(ange_TIMjx,TIM_IT_Update);
//		time++;
//	}
//}

