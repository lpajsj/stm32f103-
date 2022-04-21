#include "ange_TIMt.h"
//********************************************************//
/*
��·PWM���
ange_TIMt_OC_PWM_init()
*/
void ange_TIMt_OC_PWM_GPIO_Config()
{
	//��ͨ����ӦGPIO��ʱ��
	RCC_APB2PeriphClockCmd(ange_TIMt_OC_CH1_CLK|ange_TIMt_OC_CH2_CLK|ange_TIMt_OC_CH3_CLK|ange_TIMt_OC_CH4_CLK,ENABLE);  
	GPIO_InitTypeDef gpioinit;
	gpioinit.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	//ͨ��ch1��ʼ��
	gpioinit.GPIO_Pin=ange_TIMt_OC_CH1_Pin;
	GPIO_Init(ange_TIMt_OC_CH1_PORT,&gpioinit); ////  
	//ͨ��ch2��ʼ��
	gpioinit.GPIO_Pin=ange_TIMt_OC_CH2_Pin;
	GPIO_Init(ange_TIMt_OC_CH2_PORT,&gpioinit); /////////
	//ͨ��ch3��ʼ��
	gpioinit.GPIO_Pin=ange_TIMt_OC_CH3_Pin;
	GPIO_Init(ange_TIMt_OC_CH3_PORT,&gpioinit); /////
	//ͨ��ch4��ʼ��
	gpioinit.GPIO_Pin=ange_TIMt_OC_CH4_Pin;
	GPIO_Init(ange_TIMt_OC_CH4_PORT,&gpioinit); ////
}
void ange_TIMt_OC_PWM_Config()
{
		//��TIMʱ��
	ange_TIMt_OC_APBxClock(ange_TIMt_OC_CLK,ENABLE);
	//��ʱ��������ʼ��
	TIM_TimeBaseInitTypeDef timinit;
	//ʱ�ӷ�Ƶ������ck����int�������������Լ������˲�����Ƶ��
	timinit.TIM_ClockDivision=TIM_CKD_DIV1; 
		//��ʱ��������ʽup down centeraligned
//	timinit.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
		timinit.TIM_CounterMode=TIM_CounterMode_Up;
	//ckpsc��Ƶ��
	timinit.TIM_Prescaler=ange_TIMt_OC_Prescaler;
	//��װֵ
	timinit.TIM_Period=ange_TIMt_OC_Period;
	//�ظ�����ֵ
	timinit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(ange_TIMt_OCx,&timinit);
	///////
	TIM_OCInitTypeDef timinit1;
	//�������ģʽ
	timinit1.TIM_OCMode=TIM_OCMode_PWM1;
	//�Ƚ����ʹ��
	timinit1.TIM_OutputState=TIM_OutputState_Enable;
	//�������ʹ�ܣ�ͨ�ö�ʱ��������
	timinit1.TIM_OutputNState=TIM_OutputNState_Enable;
	//�����Ч��ƽ
	timinit1.TIM_OCPolarity=TIM_OCPolarity_High;
	//�߼���ʱ������
	timinit1.TIM_OCNPolarity=TIM_OCNPolarity_High;
	timinit1.TIM_OCIdleState=TIM_OCIdleState_Set;
	timinit1.TIM_OCNIdleState=TIM_OCNIdleState_Set;
	/////ͨ��1���
	timinit1.TIM_Pulse=ange_TIMt_OC_PWM1_Pulse;
	TIM_OC1Init(ange_TIMt_OCx,&timinit1);
	TIM_OC1PreloadConfig(ange_TIMt_OCx,TIM_OCPreload_Enable);
	//ͨ��2���
	timinit1.TIM_Pulse=ange_TIMt_OC_PWM2_Pulse;
	TIM_OC2Init(ange_TIMt_OCx,&timinit1);
	TIM_OC2PolarityConfig(ange_TIMt_OCx,TIM_OCPreload_Enable);
	//ͨ��3���
	timinit1.TIM_Pulse=ange_TIMt_OC_PWM3_Pulse;
	TIM_OC3Init(ange_TIMt_OCx,&timinit1);
	TIM_OC3PreloadConfig(ange_TIMt_OCx,TIM_OCPreload_Enable);
	///ͨ��4���
	timinit1.TIM_Pulse=ange_TIMt_OC_PWM4_Pulse;
	timinit1.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OC4Init(ange_TIMt_OCx,&timinit1);
	TIM_OC4PreloadConfig(ange_TIMt_OCx,TIM_OCPreload_Enable);
	//
	TIM_Cmd(ange_TIMt_OCx,ENABLE);
}
void ange_TIMt_OC_PWM_init()
{
	ange_TIMt_OC_PWM_GPIO_Config();
	ange_TIMt_OC_PWM_Config();
}
//�ǲ���************************************************************/
void ange_TIMt_CC_IC_GPIO_Config()
{
	RCC_APB2PeriphClockCmd(ange_TIMt_CC_CH1_CLK,ENABLE);
	GPIO_InitTypeDef gpioinit;
	gpioinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioinit.GPIO_Pin=ange_TIMt_CC_CH1_Pin;
	GPIO_Init(ange_TIMt_CC_CH1_PORT,&gpioinit);
}
void ange_TIMt_CC_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvicinit.NVIC_IRQChannel=ange_TIMt_CC_IRQn;
	nvicinit.NVIC_IRQChannelPreemptionPriority=0;
	nvicinit.NVIC_IRQChannelSubPriority=2;
	nvicinit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicinit);
}

void ange_TIMt_CC_IC_Config()
{
	ange_TIMt_CC_APBxClock(ange_TIMt_CC_CLK,ENABLE);
	TIM_TimeBaseInitTypeDef timinit;
	timinit.TIM_ClockDivision=TIM_CKD_DIV1;
	timinit.TIM_CounterMode=TIM_CounterMode_Up;
	timinit.TIM_Prescaler=ange_TIMt_CC_Prescaler;
	timinit.TIM_Period=ange_TIMt_CC_Period;
	timinit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(ange_TIMt_CCx,&timinit);
	///
	TIM_ICInitTypeDef timinit2;
	timinit2.TIM_Channel=ange_TIMt_CC_Channelx;
	///�˲���ϵ����0-f֮��
	timinit2.TIM_ICFilter=0;
	//Ԥ��Ƶ
	timinit2.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	//����
	timinit2.TIM_ICPolarity=ange_TIMt_CC_Channel_Polarity1;
	//ͨ��ѡ��
	timinit2.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(ange_TIMt_CCx,&timinit2);
	TIM_ClearFlag(ange_TIMt_CCx,TIM_IT_Update|ange_TIMt_CC_IT_CCx);
	TIM_ITConfig(ange_TIMt_CCx,TIM_IT_Update|ange_TIMt_CC_IT_CCx,ENABLE);
	TIM_Cmd(ange_TIMt_CCx,ENABLE);
}
ange_TIMt_CC_ICValue_TypeDef TIMt_IC_Value;  //���岶��ֵ�ṹ��
void ange_TIMt_CC_IRQHandler()
{
	//������
	if(TIM_GetITStatus(ange_TIMt_CCx,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(ange_TIMt_CCx,TIM_IT_Update);
		//���������һ
		TIMt_IC_Value.Capture_Period++;
	}
	if(TIM_GetITStatus(ange_TIMt_CCx,ange_TIMt_CC_IT_CCx)==SET)
	{
		TIM_ClearITPendingBit(ange_TIMt_CCx,ange_TIMt_CC_IT_CCx);
		//��һ�β���
		if(TIMt_IC_Value.Capture_ci==0)
		{
      TIM_SetCounter(ange_TIMt_CCx,0);
			TIMt_IC_Value.Capture_ci=1;
			//���񵽵�һ�������غ�ʼ������ʱ
			TIMt_IC_Value.Capture_Period=0;
//			TIMt_IC_Value.Capture_value=0;
			//��ʼ����½���
			TIM_OC1PolarityConfig(ange_TIMt_CCx,TIM_ICPolarity_Falling);
		}
		//�ڶ��β���
		else if(TIMt_IC_Value.Capture_ci==1)
		{
			TIMt_IC_Value.Capture_ci=0;
			TIMt_IC_Value.Capture_value=TIM_GetCapture1(ange_TIMt_CCx);
			TIMt_IC_Value.Capture_finish=1;
			TIM_OC1PolarityConfig(ange_TIMt_CCx,TIM_ICPolarity_Rising);
		}
	}
}
void ange_TIMt_CC_IC_init()
{
	ange_TIMt_CC_IC_GPIO_Config();
	ange_TIMt_CC_NVIC_Config();
	ange_TIMt_CC_IC_Config();
}

