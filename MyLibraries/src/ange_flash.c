#include "ange_flash.h"
#include "ange_usart.h"
void ange_flash_write32(u32 adress,u32 *x,u32 number)
{
	u32 numpage,i;
	//Ҫ��ʼ��ֵҪ��Ȼ�������ѭ����forѭ�����ж�
	FLASH_Status flashstatus=FLASH_COMPLETE;
	//����
	FLASH_Unlock();
	//�����־λ
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	//����Ҫ������ҳ��
	numpage=4*number/ange_flash_size+1;
	//����ҳ
	for(i=0;(i<numpage)&&(flashstatus==FLASH_COMPLETE);i++)
	{
		flashstatus=FLASH_ErasePage((u32)(adress+ange_flash_size*i));
	}
	//д������
	for(i=0;(i<number)&&(flashstatus==FLASH_COMPLETE);i++)
	{
		flashstatus=FLASH_ProgramWord((adress+4*i),*x);
		x++;
	}
	//
  FLASH_Lock();
}
//��ȡflash�����ݣ�
/*
���������
flash��ַ-
���������ַ
��ȡ������
*/
void ange_flash_read32(u32 adress,u32 *x,u32 number)
{
	u32 i;
	for(i=0;i<number;i++)
	{
		*x=*(u32*)(adress);
		x++;
		adress+=4;
	}
}
void ange_flash_write16(u32 adress,__IO u16 x[],u32 number)
{
	u32 numpage,i;
	//Ҫ��ʼ��ֵҪ��Ȼ�������ѭ����forѭ�����ж�
	FLASH_Status flashstatus=FLASH_COMPLETE;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	numpage=2*number/ange_flash_size+1;
	for(i=0;(i<numpage)&&(flashstatus==FLASH_COMPLETE);i++)
	{
		flashstatus=FLASH_ErasePage((u32)(adress+ange_flash_size*i));
	}
	for(i=0;(i<number)&&(flashstatus==FLASH_COMPLETE);i++)
	{
		flashstatus=FLASH_ProgramHalfWord((adress+2*i),x[i]);
//		x++;
	}
  FLASH_Lock();
}
void ange_flash_read16(u32 adress,u16 *x,u32 number)
{
	u32 i;
	for(i=0;i<number;i++)
	{
		*x=*(u16*)(adress);
		x++;
		adress+=2;
	}
}
//��д�����Ǹı�ѡ���ֽڣ�����RDP�Ĵ����Ƕ���������д��0xA5���������
//ERP�Ĵ����ж�������ԶԲ�ͬ��ҳ��ʵʩд����
/*
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg For @b STM32_Low-density_devices: value between FLASH_WRProt_Pages0to3 and FLASH_WRProt_Pages28to31  
  *     @arg For @b STM32_Medium-density_devices: value between FLASH_WRProt_Pages0to3
  *       and FLASH_WRProt_Pages124to127
  *     @arg For @b STM32_High-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to255
  *     @arg For @b STM32_Connectivity_line_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to127    
  *     @arg For @b STM32_XL-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to511
  *     @arg FLASH_WRProt_AllPages
0�ǽ��д����������ѡ���ֽڻὫRDP��д��0xA5�Ӷ����������
*/
void ange_flash_writeprotect(uint32_t FLASH_Pages)
{
	FLASH_Unlock();
	//����ѡ���ֽ�,�ὫRDP��д��0xA5�Ӷ����������
	FLASH_EraseOptionBytes();
	//ʹ��д����
	FLASH_EnableWriteProtection(FLASH_Pages);
	//�����λ
	NVIC_SystemReset();
}
/*
����������ʱ���Զ����д����
*/
void ange_flash_readprotect(FunctionalState NewState)
{
	FLASH_Unlock();
	//����ѡ���ֽ�
	FLASH_EraseOptionBytes();
	//������������������flash����
	FLASH_ReadOutProtection(NewState);
	//�ϵ縴λ
}
//�����ǰFLASH����״̬
void ange_flash_examine()
{
	if(FLASH_GetReadOutProtectionStatus()==SET)
		printf("оƬ���ڶ�����\n");
	else
		printf("оƬ�޶�����");
	if(FLASH_GetWriteProtectionOptionByte()!=0xffffffff)
		printf("оƬ����д����");
		else
		printf("оƬ��д����");
}





