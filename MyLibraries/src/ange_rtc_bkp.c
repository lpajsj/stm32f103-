#include "ange_rtc_bkp.h"
#include "ange_usart.h"
/*��λ�󣬶Ա��ݼĴ�����RTC�ķ��ʱ���ֹ�����ұ����򱻱����Է�ֹ���ܴ��ڵ������д��
����ִ�����²�������ʹ�ܶԱ��ݼĴ�����RTC�ķ��ʡ�
�� ͨ�����üĴ���RCC_APB1ENR��PWREN��BKPENλ���򿪵�Դ�ͺ󱸽ӿڵ�ʱ��
�� ��Դ���ƼĴ���(PWR_CR)��DBPλ��ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʡ�*/
/*���ݼĴ�����42��16λ�ļĴ������������洢84���ֽڵ��û�Ӧ�ó������ݡ����Ǵ��ڱ�����
���VDD��Դ���жϣ�������Ȼ��VBATά�ֹ���*/
/*RTC��PRL��CNT,ALR,��RCC��BDTR��λ�ڱ�����*/
/*BKP��ʼ��*/
void ange_BKP_init()
{
	//����PWRʱ�Ӻͺ�����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	//�����������Դ
	PWR_BackupAccessCmd(ENABLE);
	
}
//void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);  //д���ݼĴ���
//uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);             //�����ݼĴ���
//������
//ʹ��ʱ�������ʼ����NVIC��ʼ���������ж�
//�������¼����������ݱ��ݼĴ������������
void ange_BKP_Tamper_init()
{
	//�����������ƽ
	BKP_TamperPinLevelConfig(BKP_TamperPinLevel_High);
	BKP_TamperPinCmd(ENABLE);
	BKP_ClearFlag();
}
void ange_BKP_Tamper_NVIC_init()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvicinit.NVIC_IRQChannel=TAMPER_IRQn;
	nvicinit.NVIC_IRQChannelPreemptionPriority=1;
	nvicinit.NVIC_IRQChannelSubPriority=1;
	nvicinit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicinit);
	BKP_ITConfig(ENABLE);
	
}
//void BKP_ITConfig(FunctionalState NewState) �����ж�����
void TAMPER_IRQHandler()
{
	if(BKP_GetITStatus()==SET)
	{
		//�����¼���־λ�������BAT��������»�һֱ����
		BKP_ClearFlag();
		BKP_ClearITPendingBit();
		printf("Tamper is high level");
	}
}

/*RTC*/
/*����PRL��CNT,ALR�Ĵ���Ҫ������CNF�����ȴ�����������ɡ�һ��ʼ��ʱ��Ҫͬ��ʱ��*/
/*ִ�����²�����ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʣ�
�� ���üĴ���RCC_APB1ENR��PWREN��BKPENλ��ʹ�ܵ�Դ�ͺ󱸽ӿ�ʱ��
�� ���üĴ���PWR_CR��DBPλ��ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʡ�*/
/*
����NVIC
ange_RTC_init() ����RTC ���ó�ʼֵ������ֵ����ʹ���ж�
*/
#define ange_RTCCLK_LSE
//#define ange_RTCCLK_LSI
void ange_RTC_init()
{
	//����PWRʱ�Ӻͺ�����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	//�����������Դ
	PWR_BackupAccessCmd(ENABLE);
	//��λ������������Ƶ���RCC��BDCR�Ĵ���
	BKP_DeInit();
#ifdef ange_RTCCLK_LSI
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	//RCC��RTC��LSE����λ�ڱ�������ζ�����BAT���������¸�λ����Բ�������,��LSI��Ҫ����
	//RTCʱ����Դѡ����ʹ��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	//�ȴ�ͬ��
	RTC_WaitForSynchro();
	//�ȴ��������
	RTC_WaitForLastTask();
	//����RTCʱ��Ԥ��Ƶ
	RTC_SetPrescaler(40000-1);
	RTC_WaitForLastTask();
	RTC_SetCounter(1000);
	RTC_WaitForLastTask();
#endif
#ifdef ange_RTCCLK_LSE
  RCC_LSEConfig(RCC_LSE_ON);
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY));
	//RCC��RTC��LSE����λ�ڱ�������ζ�����BAT���������¸�λ����Բ�������,��LSI��Ҫ����
	//RTCʱ����Դѡ����ʹ��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
		//�ȴ�ͬ��
	RTC_WaitForSynchro();
	//�ȴ��������
	RTC_WaitForLastTask();
  //����RTCʱ��Ԥ��Ƶ
	RTC_SetPrescaler(32768-1);
	RTC_WaitForLastTask();
	RTC_SetCounter(1000);
	RTC_WaitForLastTask();
#endif
//�������ж�
RTC_SetAlarm(1060); //�ڷ���ʱ���ּ�ʹд�룬�Ĵ���ֵ��Ȼ���Ϊ0����д���ֵ���ջ��ǻ���Ч
//һ��Ҫ�ȴ���һ�β������
RTC_WaitForLastTask();
RTC_ITConfig(RTC_IT_SEC,ENABLE);
RTC_WaitForLastTask();
RTC_ITConfig(RTC_IT_ALR,ENABLE);
}
void ange_RTC_NVIC_init()
{
	NVIC_InitTypeDef nvicinit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvicinit.NVIC_IRQChannel=RTC_IRQn;
	nvicinit.NVIC_IRQChannelPreemptionPriority=1;
	nvicinit.NVIC_IRQChannelSubPriority=1;
	nvicinit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicinit);
}
void RTC_IRQHandler()
{
	if(RTC_GetITStatus(RTC_IT_SEC)==SET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		printf("%d ",RTC_GetCounter());
		return;
	}
	else if(RTC_GetITStatus(RTC_IT_ALR)==SET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);
		printf("���Ӳ���");
	}
}





