#include "ange_ad.h"
#include "ange_timg.h"
#include "main.h"

/*ADC�ⲿ�������������Դﵽ750KHZ-800KHZ*/
u16 adc1zhi11;
#if ange_ADC_Mode_Independent
/*����ģʽADC_Mode_Independent*/
/*����ģʽ��ǰΪ�ⲿT3-TRGO�źŴ�����DMA�����������*/
/*
ʹ��ʱ��������
ange_ADC_init();  //
ange_ADC_DMA_Config(); //����ΪAD���ݽ�Ҫ���������
ange_ADC_TIM_init;   //ע���޸�DMA��������
DMA��ʼ��
��ʱ����ʼ��
*/
#if ange_ADC_IT
void ange_ADC_GPIO_Config()  //adcͨ��11 pc1�Ķ˿ڳ�ʼ��
{
	GPIO_InitTypeDef gpioinit;
	ange_ADC_GPIO_APBxClock(ange_ADC_GPIO_CLK,ENABLE);
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Pin=ange_ADC_GPIO_Pin0;//|ange_ADC_GPIO_Pin1|ange_ADC_GPIO_Pin2|ange_ADC_GPIO_Pin3|ange_ADC_GPIO_Pin4
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC_GPIO_PORT,&gpioinit);
	
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Pin=ange_ADC_GPIO_Pin1;//|ange_ADC_GPIO_Pin1|ange_ADC_GPIO_Pin2|ange_ADC_GPIO_Pin3|ange_ADC_GPIO_Pin4
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC_GPIO_PORT,&gpioinit);

//	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
//	gpioinit.GPIO_Pin=ange_ADC_GPIO_Pin2;//|ange_ADC_GPIO_Pin1|ange_ADC_GPIO_Pin2|ange_ADC_GPIO_Pin3|ange_ADC_GPIO_Pin4
//	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(ange_ADC_GPIO_PORT,&gpioinit);

//	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
//	gpioinit.GPIO_Pin=ange_ADC_GPIO_Pin3;//|ange_ADC_GPIO_Pin1|ange_ADC_GPIO_Pin2|ange_ADC_GPIO_Pin3|ange_ADC_GPIO_Pin4
//	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(ange_ADC_GPIO_PORT,&gpioinit);	
	return;
}
void ange_ADC_Config()  //adc �������
{
	ADC_InitTypeDef adcinit;
	ange_ADC_APBxClock(ange_ADC_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_Independent;   //
	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
//	adcinit.ADC_ScanConvMode=ENABLE;        //��ͨ��
	adcinit.ADC_ContinuousConvMode=DISABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=2;                  //adcͨ����Ŀѡ��
	ADC_Init(ange_ADCx,&adcinit);               //adc��ʼ��
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);           //adc��Ƶ
	///////����ͨ��
	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_10,1,ADC_SampleTime_239Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_11,2,ADC_SampleTime_239Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
//	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_12,3,ADC_SampleTime_1Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
//	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_13,4,ADC_SampleTime_1Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
//	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_14,5,ADC_SampleTime_1Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
//	ADC_RegularChannelConfig(ange_ADCx,ange_ADC_Channel_15,6,ADC_SampleTime_1Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
	
	ADC_Cmd(ange_ADCx,ENABLE);  //��adc
	ADC_StartCalibration(ange_ADCx);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADCx)); //У׼���
		//��Ҫ����Ӧ�÷ŵ��������֮ǰ��ȫ�����������ʹ��
  ADC_DMACmd(ange_ADCx,ENABLE);     //һ��ʹ��dma���ý��޷��޸ģ��Ҵ�ʹ�ܲ�Ҫ�����������֮�󣬶�ͨ�������������֮������dmaͨ�����ݴ�λ
  //	ADC_SoftwareStartConvCmd(ange_ADCx,ENABLE);  //adc���ʹ��,�������ת��
//	ADC_ITConfig(ange_ADCx,ADC_IT_EOC,ENABLE);  //adc�жϿ�
  ADC_ExternalTrigConvCmd(ange_ADCx,ENABLE);
}
void ange_ADC_DMA_Config(u16 *m)//���赽�洢����16λ����
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_ADC_DMA_Clock,ENABLE);
	DMA_DeInit(ange_ADC_DMA_Channel);
	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
	dmainit.DMA_PeripheralBaseAddr=(u32)&(ADC1->DR);   //�����ַ
	dmainit.DMA_MemoryBaseAddr=(u32)m;   //�ڴ��ַ
	dmainit.DMA_BufferSize=2;  //dma���ݴ�����
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //�����ַ������
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;  //�ڴ��ַ�Ƿ����
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;   //�������ݿ��
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;  //�ڴ����ݿ��
//	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Mode=DMA_Mode_Circular;  //ѭ����������
	dmainit.DMA_Priority=DMA_Priority_Medium;  //dma���ȼ� ����low medium high veryhigh
	dmainit.DMA_M2M=DMA_M2M_Disable;   //�洢�����洢��ʱѡ��
	DMA_Init(ange_ADC_DMA_Channel,&dmainit);  //��ʼ��ʱҪѡ��ͨ��
	DMA_Cmd(ange_ADC_DMA_Channel,ENABLE);
}
void ange_ADC_init()
{
	ange_ADC_GPIO_Config();
	ange_ADC_NVIC_Config();
	ange_ADC_Config();
}
void ange_ADC_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //�����ж����ȼ�����
	nvicinit.NVIC_IRQChannel=ange_ADC_IRQ;       //�ж�Դѡ��
	nvicinit.NVIC_IRQChannelPreemptionPriority=1; //������ռ���ȼ�
	nvicinit.NVIC_IRQChannelSubPriority=1;       //���������ȼ�
	nvicinit.NVIC_IRQChannelCmd=ENABLE;          //�ж�ʹ��
	NVIC_Init(&nvicinit);           //nvic��ʼ������
}
void ange_ADC_IRQHandler()
{
	if(ADC_GetITStatus(ange_ADCx,ADC_IT_EOC))  ///adcת�������ж�
	{
		adc1zhi11=ADC_GetConversionValue(ange_ADCx);
		ADC_ClearITPendingBit(ange_ADCx,ADC_IT_EOC);
	}
}
/*/�������*/
/*
���������ȡһ��ADֵ
ange_ADC_easy_init();
ange_ADC_easy_getvalue();
*/
void ange_ADC_easy_GPIO_Config()  
{
	GPIO_InitTypeDef gpioinit;
	ange_ADC_EASY_GPIO_APBxClock(ange_ADC_EASY_GPIO_CLK,ENABLE);
	gpioinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpioinit.GPIO_Pin=ange_ADC_EASY_GPIO_Pin;//|ange_ADC_GPIO_Pin1|ange_ADC_GPIO_Pin2|ange_ADC_GPIO_Pin3|ange_ADC_GPIO_Pin4
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC_EASY_GPIO_PORT,&gpioinit);
	return;
}
void ange_ADC_easy_Config()  //adc �������
{
	ADC_InitTypeDef adcinit;
	ange_ADC_EASY_APBxClock(ange_ADC_EASY_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_Independent;   //
//	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
	adcinit.ADC_ScanConvMode=ENABLE;        //��ͨ��
	adcinit.ADC_ContinuousConvMode=DISABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=1;                  //adcͨ����Ŀѡ��
	ADC_Init(ange_ADCx_EASY,&adcinit);               //adc��ʼ��
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);           //adc��Ƶ
	ADC_RegularChannelConfig(ange_ADCx_EASY,ange_ADC_EASY_Channel,1,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��  //�������ǲ���ʱ��
	/////////
//	ADC_ITConfig(ange_ADCx,ADC_IT_EOC,ENABLE);  //adc�жϿ�
	ADC_Cmd(ange_ADCx_EASY,ENABLE);  //��adc
	ADC_StartCalibration(ange_ADCx_EASY);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADCx_EASY)); //У׼���
		//��Ҫ����Ӧ�÷ŵ��������֮ǰ��ȫ�����������ʹ��
}
void ange_ADC_easy_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //�����ж����ȼ�����
	nvicinit.NVIC_IRQChannel=ange_ADC_EASY_IRQ;       //�ж�Դѡ��
	nvicinit.NVIC_IRQChannelPreemptionPriority=1; //������ռ���ȼ�
	nvicinit.NVIC_IRQChannelSubPriority=1;       //���������ȼ�
	nvicinit.NVIC_IRQChannelCmd=ENABLE;          //�ж�ʹ��
	NVIC_Init(&nvicinit);           //nvic��ʼ������
}
void ange_ADC_easy_init()
{
	ange_ADC_easy_GPIO_Config();
//	ange_ADC_NVIC_Config();
	ange_ADC_easy_Config();
}
u16 ange_ADC_easy_getvalue()
{
	  u16 x;
		ADC_SoftwareStartConvCmd(ange_ADCx_EASY,ENABLE);
		while(ADC_GetFlagStatus(ange_ADCx_EASY,ADC_FLAG_EOC)!=SET);
		ADC_ClearFlag(ange_ADCx_EASY,ADC_FLAG_EOC);
		x=ADC_GetConversionValue(ange_ADCx_EASY);
	  return x;
}
#endif
#endif
#if ange_ADC_TIM_TRGO
/*ADC�ⲿ����TIM3����*/
void ange_ADC_TIM_Config()
{
	TIM_TimeBaseInitTypeDef timinit;    
	ange_ADC_TIM_APBxClock(ange_ADC_TIM_CLK,ENABLE);     //timʱ����apb1
	timinit.TIM_ClockDivision=TIM_CKD_DIV1;   //ckint,�������������Լ������˲�������ʱ��Ƶ�ʷ�Ƶ�� ������ʱ����
	timinit.TIM_Prescaler=72-1;  //ck_psc��Ƶ
	timinit.TIM_CounterMode=TIM_CounterMode_Up;  //��ʱ��������ʽ��������ʱ��ֻ������ģʽ
	timinit.TIM_Period=(1000000/1024)-1;     //��������װֵ
	timinit.TIM_RepetitionCounter=0;  //�ظ���������ֻ���ڸ߼���ʱ��
	TIM_TimeBaseInit(ange_ADC_TIMx,&timinit);//��ʱ��������ʼ��
	TIM_ClearFlag(ange_ADC_TIMx,TIM_FLAG_Update); //��������־
	TIM_Cmd(ange_ADC_TIMx,ENABLE);  //ʹ��TIM
	
}
void ange_ADC_TIM_init()
{
	ange_ADC_TIM_Config();
	TIM_SelectOutputTrigger(ange_ADC_TIMx,TIM_TRGOSource_Update);
}
#endif
////////////////////////////////////////////////////////////////////
#if ange_ADC_Mode_RegSimult
/*ͬ������ģʽADC_Mode_RegSimult*/ 
/*9.10ͬ������ʱ��������������*/
void ange_ADC12_GPIO_Config()  //adcͨ��11 pc1�Ķ˿ڳ�ʼ��
{
	GPIO_InitTypeDef gpioinit;
	ange_ADC1_GPIO_APBxClock(ange_ADC1_GPIO_CLK|ange_ADC2_GPIO_CLK,ENABLE);
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Pin=ange_ADC1_Channel1_GPIO_Pin;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC1_Channel1_GPIO_PORT,&gpioinit);
	gpioinit.GPIO_Pin=ange_ADC2_Channel1_GPIO_Pin;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC2_Channel1_GPIO_PORT,&gpioinit);
	return;
}
void ange_ADC12_Config()  //adc �������
{
	ADC_InitTypeDef adcinit;
	ange_ADC1_APBxClock(ange_ADC1_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_RegSimult;   //
//	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
	adcinit.ADC_ScanConvMode=DISABLE;        //
	adcinit.ADC_ContinuousConvMode=DISABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=1;                  //adcͨ����Ŀѡ��
	ADC_Init(ange_ADC1,&adcinit);               //adc��ʼ��
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);           //adc��Ƶ
	ADC_RegularChannelConfig(ange_ADC1,ange_ADC1_Channel1,1,ADC_SampleTime_71Cycles5);//����ͨ��ʹ��
	/////////
//	ADC_ITConfig(ange_ADC1,ADC_IT_EOC,ENABLE);  //adc�жϿ�
	ADC_Cmd(ange_ADC1,ENABLE);  //��adc
	ADC_ResetCalibration(ange_ADC1);
	while(ADC_GetResetCalibrationStatus(ange_ADC1));
	ADC_StartCalibration(ange_ADC1);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADC1)); //У׼���
//adc2���á�����������������
	ange_ADC2_APBxClock(ange_ADC2_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_RegSimult;   //
//	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
	adcinit.ADC_ScanConvMode=ENABLE;        //
	adcinit.ADC_ContinuousConvMode=ENABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=1;                  //adcͨ����Ŀѡ��
	ADC_Init(ange_ADC2,&adcinit);               //adc��ʼ��
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);           //adc��Ƶ
	ADC_RegularChannelConfig(ange_ADC2,ange_ADC2_Channel1,1,ADC_SampleTime_71Cycles5);//����ͨ��ʹ��
	/////////
//	ADC_ITConfig(ange_ADC2,ADC_IT_EOC,ENABLE);  //adc�жϿ�
	ADC_Cmd(ange_ADC2,ENABLE);  //��adc
	ADC_ResetCalibration(ange_ADC2);
	while(ADC_GetResetCalibrationStatus(ange_ADC2));
	ADC_StartCalibration(ange_ADC2);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADC2)); //У׼���
	ADC_ExternalTrigConvCmd(ange_ADC1,ENABLE);
	ADC_ExternalTrigConvCmd(ange_ADC2,ENABLE);
//	ADC_SoftwareStartConvCmd(ange_ADC2,ENABLE);  //adc���ʹ��,�������ת��
//	ADC_SoftwareStartConvCmd(ange_ADC1,ENABLE);  //adc���ʹ��,�������ת��
}
void ange_ADC12_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //�����ж����ȼ�����
	nvicinit.NVIC_IRQChannel=ange_ADC12_IRQ;       //�ж�Դѡ��
	nvicinit.NVIC_IRQChannelPreemptionPriority=1; //������ռ���ȼ�
	nvicinit.NVIC_IRQChannelSubPriority=1;       //���������ȼ�
	nvicinit.NVIC_IRQChannelCmd=ENABLE;          //�ж�ʹ��
	NVIC_Init(&nvicinit);           //nvic��ʼ������
}

void ange_ADC12_init()
{
	ange_ADC12_GPIO_Config();
//	ange_ADC12_NVIC_Config();
	ange_ADC12_Config();
}
void ange_ADC12_IRQHandler()
{
	if(ADC_GetITStatus(ange_ADC1,ADC_IT_EOC))  ///adcת�������ж�
	{
		adc1zhi11=ADC_GetConversionValue(ange_ADC1);
		ADC_ClearITPendingBit(ange_ADC1,ADC_IT_EOC);
	}
}
void ange_ADC12_DMA_Config(u32 *x)
{
	DMA_InitTypeDef dmainit;
	RCC_AHBPeriphClockCmd(ange_ADC12_DMA_CLK,ENABLE);
	DMA_DeInit(ange_ADC12_DMA_Channel);
	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
	dmainit.DMA_PeripheralBaseAddr=ange_ADC12_DMA_wai;   //�����ַ
	dmainit.DMA_MemoryBaseAddr=(u32)(x);   //�ڴ��ַ
	dmainit.DMA_BufferSize=ange_ADC12_DMA_BufferSize;  //dma���ݴ�����
	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //�����ַ������
	dmainit.DMA_MemoryInc=DMA_MemoryInc_Enable;  //�ڴ��ַ�Ƿ����
	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;   //�������ݿ��
	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;  //�ڴ����ݿ��
//	dmainit.DMA_Mode=DMA_Mode_Normal;
	dmainit.DMA_Mode=DMA_Mode_Circular;  //ѭ����������
	dmainit.DMA_Priority=DMA_Priority_Medium;  //dma���ȼ� ����low medium high veryhigh
	dmainit.DMA_M2M=DMA_M2M_Disable;   //�洢�����洢��ʱѡ��
	DMA_Init(ange_ADC12_DMA_Channel,&dmainit);  //��ʼ��ʱҪѡ��ͨ��
	ADC_DMACmd(ange_ADC1,ENABLE);
	DMA_Cmd(ange_ADC12_DMA_Channel,ENABLE);
}
void ange_ADC12_DMA_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //�����ж����ȼ�����
	nvicinit.NVIC_IRQChannel=ange_ADC12_DMA_IRQ;       //�ж�Դѡ��
	nvicinit.NVIC_IRQChannelPreemptionPriority=1; //������ռ���ȼ�
	nvicinit.NVIC_IRQChannelSubPriority=0;       //���������ȼ�
	nvicinit.NVIC_IRQChannelCmd=ENABLE;          //�ж�ʹ��
	NVIC_Init(&nvicinit);           //nvic��ʼ������
}
u8 adc12_dma_finish=0;
void ange_ADC12_DMA_IRQHandler()
{
//	u32 i;
//	float x,y;
	if(DMA_GetITStatus(ange_ADC12_DMA_IT_FLAG))
	{
		TIM_Cmd(ange_ADC_TIMx,DISABLE);
		DMA_ClearITPendingBit(ange_ADC12_DMA_IT_FLAG);
//		for(i=0;i<FFTdianshu;i++)
//		{
//			adcValue1_u16[i]=(float)(adcValue_u32[i]&0xffff)*3.3/4095;
//			adcValue2_u16[i]=(float)(adcValue_u32[i]>>16)*3.3/4095;
//		}
//		arm_mean_f32(adcValue1_u16,FFTdianshu,&x);
//		arm_mean_f32(adcValue2_u16,FFTdianshu,&y);
//		for(i=0;i<FFTdianshu;i++)
//		{
//			adcValue1_u16[i]=adcValue1_u16[i]-x;
//			adcValue2_u16[i]=adcValue2_u16[i]-y;
//		}
		adc12_dma_finish=1;
		
	}
}
#endif
/////////////////////////////////
#if ange_ADC_Mode_InjecSimult
/*ͬ��ע��ģʽADC_Mode_InjecSimult������ͬ��������*/
void ange_ADC12_GPIO_Config()  //adcͨ��11 pc1�Ķ˿ڳ�ʼ��
{
	GPIO_InitTypeDef gpioinit;
	ange_ADC1_GPIO_APBxClock(ange_ADC1_GPIO_CLK|ange_ADC2_GPIO_CLK,ENABLE);
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioinit.GPIO_Pin=ange_ADC1_Channel1_GPIO_Pin;
	GPIO_Init(ange_ADC1_Channel1_GPIO_PORT,&gpioinit);
	///
	gpioinit.GPIO_Pin=ange_ADC1_Channel2_GPIO_Pin;
	GPIO_Init(ange_ADC2_Channel2_GPIO_PORT,&gpioinit);
	//
	gpioinit.GPIO_Pin=ange_ADC1_Channel3_GPIO_Pin;
	GPIO_Init(ange_ADC2_Channel3_GPIO_PORT,&gpioinit);
	///
	gpioinit.GPIO_Pin=ange_ADC2_Channel1_GPIO_Pin;
	GPIO_Init(ange_ADC2_Channel1_GPIO_PORT,&gpioinit);
	////
	gpioinit.GPIO_Pin=ange_ADC2_Channel2_GPIO_Pin;
	GPIO_Init(ange_ADC2_Channel2_GPIO_PORT,&gpioinit);
	//
	gpioinit.GPIO_Pin=ange_ADC2_Channel2_GPIO_Pin;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ange_ADC2_Channel2_GPIO_PORT,&gpioinit);
	return;
}
void ange_ADC12_Config()  //adc �������
{
	ADC_InitTypeDef adcinit;
	ange_ADC1_APBxClock(ange_ADC1_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_InjecSimult;   //
//	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
	adcinit.ADC_ScanConvMode=ENABLE;        //
	adcinit.ADC_ContinuousConvMode=DISABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=1;                  //������adcͨ����Ŀѡ��
	ADC_Init(ange_ADC1,&adcinit);               //adc��ʼ��
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);           //adc��Ƶ
	ADC_InjectedSequencerLengthConfig(ange_ADC1,3);
	ADC_InjectedChannelConfig(ange_ADC1,ange_ADC1_Channel1,1,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
	ADC_InjectedChannelConfig(ange_ADC1,ange_ADC1_Channel2,2,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
		ADC_InjectedChannelConfig(ange_ADC1,ange_ADC1_Channel3,3,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
	ADC_Cmd(ange_ADC1,ENABLE);  //��adc
	ADC_ResetCalibration(ange_ADC1);
	while(ADC_GetResetCalibrationStatus(ange_ADC1));
	ADC_StartCalibration(ange_ADC1);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADC1)); //У׼���
//adc2���á�����������������
	ange_ADC2_APBxClock(ange_ADC2_CLK,ENABLE);  ///����adcʱ��
	adcinit.ADC_Mode=ADC_Mode_InjecSimult;   //
//	adcinit.ADC_ScanConvMode=DISABLE;      //��ͨ��ɨ��ģʽ���ǵ�ͨ��
	adcinit.ADC_ScanConvMode=ENABLE;        //
	adcinit.ADC_ContinuousConvMode=DISABLE;     //����ת�����ǵ���ת��
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //adc�ⲿ������ʽ
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;   //���ݶ��䷽ʽ
	adcinit.ADC_NbrOfChannel=1;                  //adcͨ����Ŀѡ��
	ADC_Init(ange_ADC2,&adcinit);               //adc��ʼ��
	ADC_InjectedSequencerLengthConfig(ange_ADC1,3);
	//adcʱ�Ӳ��ܳ���14Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);           //adc��Ƶ
	ADC_InjectedChannelConfig(ange_ADC2,ange_ADC2_Channel3,1,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
	ADC_InjectedChannelConfig(ange_ADC2,ange_ADC2_Channel1,2,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
	ADC_InjectedChannelConfig(ange_ADC2,ange_ADC2_Channel2,3,ADC_SampleTime_55Cycles5);//����ͨ��ʹ��
	ADC_Cmd(ange_ADC2,ENABLE);  //��adc
	ADC_ResetCalibration(ange_ADC2);
	while(ADC_GetResetCalibrationStatus(ange_ADC2));
	ADC_StartCalibration(ange_ADC2);  //adc��ʼУ׼
	while(ADC_GetCalibrationStatus(ange_ADC2)); //У׼���
	ADC_ExternalTrigInjectedConvConfig(ange_ADC1,ADC_ExternalTrigInjecConv_T4_TRGO);
	ADC_ExternalTrigInjectedConvCmd(ange_ADC1,ENABLE);
	ADC_ExternalTrigInjectedConvCmd(ange_ADC2,ENABLE);
}
void ange_ADC12_NVIC_Config()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //�����ж����ȼ�����
	nvicinit.NVIC_IRQChannel=ange_ADC12_IRQ;       //�ж�Դѡ��
	nvicinit.NVIC_IRQChannelPreemptionPriority=1; //������ռ���ȼ�
	nvicinit.NVIC_IRQChannelSubPriority=1;       //���������ȼ�
	nvicinit.NVIC_IRQChannelCmd=ENABLE;          //�ж�ʹ��
	NVIC_Init(&nvicinit);           //nvic��ʼ������
}
//void ange_ADC12_DMA_Config(u32 *x)
//{
//	DMA_InitTypeDef dmainit;
//	RCC_AHBPeriphClockCmd(ange_ADC12_DMA_CLK,ENABLE);
//	DMA_DeInit(ange_ADC12_DMA_Channel);
//	dmainit.DMA_DIR=DMA_DIR_PeripheralSRC;  //SRC ���赽�洢�� DST �浽��
//	dmainit.DMA_PeripheralBaseAddr=ange_ADC12_DMA_wai;   //�����ַ
//	dmainit.DMA_MemoryBaseAddr=(u32)(x);   //�ڴ��ַ
//	dmainit.DMA_BufferSize=ange_ADC12_DMA_BufferSize;  //dma���ݴ�����
//	dmainit.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //�����ַ������
//	dmainit.DMA_MemoryInc=DMA_PeripheralInc_Disable;  //�ڴ��ַ�Ƿ����
//	dmainit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;   //�������ݿ��
//	dmainit.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;  //�ڴ����ݿ��
////	dmainit.DMA_Mode=DMA_Mode_Normal;
//	dmainit.DMA_Mode=DMA_Mode_Circular;  //ѭ����������
//	dmainit.DMA_Priority=DMA_Priority_Medium;  //dma���ȼ� ����low medium high veryhigh
//	dmainit.DMA_M2M=DMA_M2M_Disable;   //�洢�����洢��ʱѡ��
//	DMA_Init(ange_ADC12_DMA_Channel,&dmainit);  //��ʼ��ʱҪѡ��ͨ��
//	DMA_Cmd(ange_ADC12_DMA_Channel,ENABLE);
//	ADC_DMACmd(ange_ADC1,ENABLE);
//}
void ange_ADC12_init()
{
	ange_ADC12_GPIO_Config();
//	ange_ADC12_NVIC_Config();
	ange_ADC12_Config();
}
//void ange_ADC12_IRQHandler()
//{
//	if(ADC_GetITStatus(ange_ADC1,ADC_IT_EOC))  ///adcת�������ж�
//	{
//		adc1zhi11=ADC_GetConversionValue(ange_ADC1);
//		ADC_ClearITPendingBit(ange_ADC1,ADC_IT_EOC);
//	}
//}
//	ange_ADC12_init();
//	ange_ADC12_DMA_Config(&ad);
//	ange_TIMj_init();
#endif
/* ADC_Mode_InjecSimult ͬ��ע�룬������ADCͬʱת��ע��ͨ���飬
****************/
/* ADC_Mode_RegSimultͬ�������ǣ�������ADCͬʱת������ͨ���飬
****************/
/*����ģʽADC_Mode_Independent
*************************/
/*ADC_Mode_FastInterl ���ٽ��津��
ģʽֻ�����ڹ���ͨ����(ͨ��Ϊһ��ͨ��)���ⲿ��������ADC1�Ĺ���ͨ����·���ء��ⲿ
����������
�� ADC2������������
�� ADC1���ӳ�7��ADCʱ�����ں�����
����������ʱ��<7��ADCCLK���ڣ�����ADC1��ADC2ת����ͬͨ��ʱ���������������ڵ�
�ص���
*************************/
/*ADC_Mode_SlowInterl  ���ٽ��津��
14��ADCʱ������
*************************/
/*ADC_Mode_AlterTrig  ���津��
��ģʽֻ������ע��ͨ���顣�ⲿ����Դ����ADC1��ע��ͨ����·���ء�
�� ����һ����������ʱ�� ADC1�ϵ�����ע����ͨ����ת����
�� ���ڶ�����������ʱ�� ADC2�ϵ�����ע����ͨ����ת����
*************************/
/*ADC_Mode_RegInjecSimult ͬ������ͬ��ע��
������ͬ��ת�����Ա��жϣ�������ע�����ͬ��ת����
*************************/
/*ADC_Mode_RegSimult_AlterTrig  ͬ������ӽ��津��
������ͬ��ת�����Ա��жϣ�������ע���齻�津��ת��
*************************/
/*ADC_Mode_InjecSimult_FastInterl  ע��ͬ���ӿ��ٽ���
*************************/
/*ADC_Mode_InjecSimult_SlowInterl  ע��ͬ�������ٽ���
*************************/
//  ���Ź�
//   /* Configure high and low analog watchdog thresholds */
//  ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);
//  /* Configure channel14 as the single analog watchdog guarded channel */
//  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_14);
//  /* Enable analog watchdog on one regular channel */
//  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
