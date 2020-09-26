#pragma once
#include <ntifs.h>
#include <intrin.h>


typedef struct RFLAGS
{
	unsigned __int64 CF : 1;                 //��λ���߽�λ,��ͨ��STC��CLCָ�������ú����
	unsigned __int64 REVERSE1 : 1;           //����ֵΪ1
	unsigned __int64 PF : 1;				 //������������ֽڰ���ż����1,�ñ�־Ϊ1
	unsigned __int64 REVERSE2 : 1;           //����ֵΪ0 
	unsigned __int64 AF : 1;				 //������λ��־��������ֽڳ��н�λ���߽�λ
	unsigned __int64 REVERSE3 : 1;           //����ֵΪ0 
	unsigned __int64 ZF : 1;				 //������Ϊ0���ñ�־Ϊ1
	unsigned __int64 SF : 1;                 //���ű�־������Ϊ1
	unsigned __int64 TF : 1;                 //�����־
	unsigned __int64 IF : 1;				 //�жϱ�־��Ϊ0��ֹ��Ӧ�������жϣ���ͨ��STI��CLIָ�������ú����
	unsigned __int64 DF : 1;				 //�����־��Ϊ1ʱ�ַ�������ָ��ÿ�β������ַ�ݼ���ַ�Ĵ���esi��edi����ͨ��STD��CLDָ�������ú����
	unsigned __int64 OF : 1;				 //�����־��������������ı�ﷶΧΪ1
	unsigned __int64 IOPL : 2;				 //��ʾ��ǰ�����I/OȨ�޼���
	unsigned __int64 NT : 1;				 //����Ƕ�ױ�־��Ϊ1��ʾ��ǰ���������ӵ�ǰ��ִ�е�����ͨ���������жϻ����쳣������IDT���е�������
	unsigned __int64 REVERSE4 : 1;           //����ֵΪ0 
	unsigned __int64 RF : 1;                 //���ƴ������Ե����쳣��#DB������Ӧ��Ϊ1ʱ��ʱ��ֹ����ָ��ϵ㣨ͨ�����ԼĴ������õ�ָ��ϵ㣩���µĵ����쳣
	unsigned __int64 VM : 1;                 //Ϊ1����������8086ģʽ
	unsigned __int64 AC : 1;                 //���øñ�־��CR0�Ĵ�����AM��־���������ڴ������
	unsigned __int64 VIF : 1;                //��VIP��־һ��ʵ�����ڱ��ڴ���������������ڴ��жϻ���
	unsigned __int64 VIP : 1;                //��VIF��־һ��ʵ�����ڱ��ڴ���������������ڴ��жϻ���
	unsigned __int64 ID : 1;                 //���ڼ���Ƿ�֧��CPUIDָ��
	unsigned __int64 REVERSE5 : 42;          //����ֵΪ0
}RFLAGS;


typedef struct CR0
{
	unsigned __int64 PE : 1;                //Ϊ1��������ģʽ��Ϊ0����ʵ��ַģʽ
	unsigned __int64 MP : 1;
	unsigned __int64 EM : 1;
	unsigned __int64 TS : 1;
	unsigned __int64 ET : 1;
	unsigned __int64 NE : 1;
	unsigned __int64 REVERSE1 : 10;
	unsigned __int64 WP : 1;               //Ϊ1ʱ����ֹ�ں˴���д�û�����ֻ���ڴ�ҳ��Ϊ0ʱ����
	unsigned __int64 REVERSE2 : 1;
	unsigned __int64 AM : 1;               //Ϊ1ʱ�����Զ��ڴ�����飬Ϊ0ʱ��ֹ
	unsigned __int64 REVERSE3 : 10;
	unsigned __int64 NW : 1;
	unsigned __int64 CD : 1;
	unsigned __int64 PG : 1;               //Ϊ1ʱ����ҳ���ƣ�Ϊ0ʱ��ֹ
	unsigned __int64 REVERSE4 : 32;
}CR0;


typedef struct CR2
{
	unsigned __int64 VirtualAddress;       //��¼����ȱҳ�쳣�Ƿ����ʵ��ڴ��ַ
}CR2;


typedef struct CR3
{
	union
	{
		struct  
		{                                           //CR4.PCIDE = 0
			unsigned __int64 REVERSE1 : 2;
			unsigned __int64 PWT : 1;               //����ҳĿ¼�Ļ��淽ʽ��Ϊ1ʱ����write-through��ʽ��Ϊ0����write-back��ʽ
			unsigned __int64 PCD : 1;               //�����Ƿ�Ե�ǰҳĿ¼���и��ٻ��棬Ϊ1��ֹ��Ϊ0����
			unsigned __int64 REVERSE2 : 7;
		}s;
		unsigned __int64 PCID : 12;                 //CR4.PCIDE = 1,�������������ID��
	}u;
	
	unsigned __int64 DirBase : 52;
}CR3;


typedef struct CR4
{
	unsigned __int64 VME : 1;                //��������8086ģʽ�µ��жϺ��쳣������չ,���жϺ��쳣�ض���8086�Ĵ��������Լ��ٵ�������8086���ӳ���Ŀ���
	unsigned __int64 PVI : 1;                //Ϊ1ʱ����Ӳ��֧�ֵ������жϱ�־��VIF��,Ϊ0��ֹVIF��־
	unsigned __int64 TSD : 1;
	unsigned __int64 DE : 1;                 //Ϊ1ʱ����DR4��DR5�Ĵ�����������Чָ�#UD���쳣��Ϊ0ʱ����DR4��DR5�ȼ�������DR6��DR7
	unsigned __int64 PSE : 1;                //Ϊ1ʱ����4MB�ڴ�ҳ��Ϊ0ʱ�����ڴ�ҳΪ4KB
	unsigned __int64 PAE : 1;                //Ϊ1ʱ֧��36λ��36λ���ϵ������ڴ��ַ��Ϊ0ʱ���������ַΪ32λ
	unsigned __int64 MCE : 1;                //Ϊ1ʱ���û�������쳣��Ϊ0ʱ��ֹ
	unsigned __int64 PGE : 1;                //Ϊ1ʱ����P6�����������ȫ��ҳ���ܣ�Ϊ0ʱ��ֹ
	unsigned __int64 PCE : 1;
	unsigned __int64 OSFXSR : 1;
	unsigned __int64 OSXMME : 1;
	unsigned __int64 UMIP : 1;               //User-Mode Instruction Prevention (SGDT, SIDT, SLDT, SMSW, and STR are disabled in user mode)
	unsigned __int64 REVSRSE1 : 1;
	unsigned __int64 VMXE : 1;
	unsigned __int64 SMXE : 1;
	unsigned __int64 REVERSE2 : 2;
	unsigned __int64 PCIDE : 1;              //PCID Enable
	unsigned __int64 OSXSAVE : 1;            //XSAVE And Processor Extended States Enable
	unsigned __int64 REVERSE3 : 1;
	unsigned __int64 SMEP : 1;               //Supervisor Mode Executions Protection Enable
	unsigned __int64 SMAP : 1;               //Supervisor Mode Access Protection Enable
	unsigned __int64 REVERSE4 : 42;       
}CR4;



RFLAGS GetRFlags()
{
	RFLAGS rflags;
	InterlockedExchange64((volatile LONGLONG*)&rflags, __readeflags());
	return rflags;
}



CR0 GetCR0()
{
	CR0 cr0;
	InterlockedExchange64((volatile LONGLONG*)&cr0, __readcr0());
	return cr0;
}


CR2 GetCR2()
{
	CR2 cr2;
	InterlockedExchange64((volatile LONGLONG*)&cr2, __readcr2());
	return cr2;
}


CR3 GetCR3()
{
	CR3 cr3;
	InterlockedExchange64((volatile LONGLONG*)&cr3, __readcr3());
	return cr3;
}


CR4 GetCR4()
{
	CR4 cr4;
	InterlockedExchange64((volatile LONGLONG*)&cr4, __readcr4());
	return cr4;
}



