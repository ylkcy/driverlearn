#pragma once
#include <ntifs.h>
#include <intrin.h>


typedef struct EFLAGS
{
	unsigned int CF : 1;                 //��λ���߽�λ,��ͨ��STC��CLCָ�������ú����
	unsigned int Reverse1 : 1;           //����ֵΪ1
	unsigned int PF : 1;				 //������������ֽڰ���ż����1,�ñ�־Ϊ1
	unsigned int Reverse3 : 1;           //����ֵΪ0 
	unsigned int AF : 1;				 //������λ��־��������ֽڳ��н�λ���߽�λ
	unsigned int Reverse5 : 1;           //����ֵΪ0 
	unsigned int ZF : 1;				 //������Ϊ0���ñ�־Ϊ1
	unsigned int SF : 1;                 //���ű�־������Ϊ1
	unsigned int TF : 1;                 //�����־
	unsigned int IF : 1;				 //�жϱ�־��Ϊ0��ֹ��Ӧ�������жϣ���ͨ��STI��CLIָ�������ú����
	unsigned int DF : 1;				 //�����־��Ϊ1ʱ�ַ�������ָ��ÿ�β������ַ�ݼ���ַ�Ĵ���esi��edi����ͨ��STD��CLDָ�������ú����
	unsigned int OF : 1;				 //�����־��������������ı�ﷶΧΪ1
	unsigned int IOPL : 2;				 //��ʾ��ǰ�����I/OȨ�޼���
	unsigned int NT : 1;				 //����Ƕ�ױ�־��Ϊ1��ʾ��ǰ���������ӵ�ǰ��ִ�е�����ͨ���������жϻ����쳣������IDT���е�������
	unsigned int Reverse15 : 1;          //����ֵΪ0 
	unsigned int RF : 1;                 //���ƴ������Ե����쳣��#DB������Ӧ��Ϊ1ʱ��ʱ��ֹ����ָ��ϵ㣨ͨ�����ԼĴ������õ�ָ��ϵ㣩���µĵ����쳣
	unsigned int VM : 1;                 //Ϊ1����������8086ģʽ
	unsigned int AC : 1;                 //���øñ�־��CR0�Ĵ�����AM��־���������ڴ������
	unsigned int VIF : 1;                //��VIP��־һ��ʵ�����ڱ��ڴ���������������ڴ��жϻ���
	unsigned int VIP : 1;                //��VIF��־һ��ʵ�����ڱ��ڴ���������������ڴ��жϻ���
	unsigned int ID : 1;                 //���ڼ���Ƿ�֧��CPUIDָ��
	unsigned int Reverse22 : 10;         //����ֵΪ0
}EFLAGS;


typedef struct CR0
{
	unsigned int PE : 1;                //Ϊ1��������ģʽ��Ϊ0����ʵ��ַģʽ
	unsigned int MP : 1;                
	unsigned int EM : 1;                
	unsigned int TS : 1;                
	unsigned int ET : 1;
	unsigned int NE : 1;
	unsigned int REVERSE1 : 10;
	unsigned int WP : 1;               //Ϊ1ʱ����ֹ�ں˴���д�û�����ֻ���ڴ�ҳ��Ϊ0ʱ����
	unsigned int REVERSE2 : 1;
	unsigned int AM : 1;               //Ϊ1ʱ�����Զ��ڴ�����飬Ϊ0ʱ��ֹ
	unsigned int REVERSE : 10;
	unsigned int NW : 1;
	unsigned int CD : 1;
	unsigned int PG : 1;               //Ϊ1ʱ����ҳ���ƣ�Ϊ0ʱ��ֹ
}CR0;


typedef struct CR2
{
	unsigned int VirtualAddress;
}CR2;


typedef struct CR3
{
	unsigned int REVERSE1 : 2;
	unsigned int PWT : 1;               //����ҳĿ¼�Ļ��淽ʽ��Ϊ1ʱ����write-through��ʽ��Ϊ0����write-back��ʽ
	unsigned int PCD : 1;               //�����Ƿ�Ե�ǰҳĿ¼���и��ٻ��棬Ϊ1��ֹ��Ϊ0����
	unsigned int REVERSE2 : 7;
	unsigned int DirBase : 20;
}CR3;


typedef struct CR4
{
	unsigned int VME : 1;                //��������8086ģʽ�µ��жϺ��쳣������չ,���жϺ��쳣�ض���8086�Ĵ��������Լ��ٵ�������8086���ӳ���Ŀ���
	unsigned int PVI : 1;                //Ϊ1ʱ����Ӳ��֧�ֵ������жϱ�־��VIF��,Ϊ0��ֹVIF��־
	unsigned int TSD : 1;
	unsigned int DE : 1;                 //Ϊ1ʱ����DR4��DR5�Ĵ�����������Чָ�#UD���쳣��Ϊ0ʱ����DR4��DR5�ȼ�������DR6��DR7
	unsigned int PSE : 1;                //Ϊ1ʱ����4MB�ڴ�ҳ��Ϊ0ʱ�����ڴ�ҳΪ4KB
	unsigned int PAE : 1;                //Ϊ1ʱ֧��36λ��36λ���ϵ������ڴ��ַ��Ϊ0ʱ���������ַΪ32λ
	unsigned int MCE : 1;                //Ϊ1ʱ���û�������쳣��Ϊ0ʱ��ֹ
	unsigned int PGE : 1;                //Ϊ1ʱ����P6�����������ȫ��ҳ���ܣ�Ϊ0ʱ��ֹ
	unsigned int PCE : 1;
	unsigned int OSFXSR : 1;
	unsigned int OSXMME : 1;
	unsigned int REVSRSE1 : 2;
	unsigned int VMXE : 1;
	unsigned int SMXE : 1;
	unsigned int REVERSE2 : 16;
}CR4;


EFLAGS GetEFlags()
{
	EFLAGS eflags;
	InterlockedExchange((volatile LONG*)&eflags, __readeflags());
	return eflags;
}


CR0 GetCR0()
{
	CR0 cr0;
	InterlockedExchange((volatile LONG*)&cr0, __readcr0());
	return cr0;
}


CR2 GetCR2()
{
	CR2 cr2;
	InterlockedExchange((volatile LONG*)&cr2, __readcr2());
	return cr2;
}


CR3 GetCR3()
{
	CR3 cr3;
	InterlockedExchange((volatile LONG*)&cr3, __readcr3());
	return cr3;
}


CR4 GetCR4()
{
	CR4 cr4;
	InterlockedExchange((volatile LONG*)&cr4, __readcr4());
	return cr4;
}

