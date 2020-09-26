#pragma once
#include <ntifs.h>
#include <intrin.h>


typedef struct RFLAGS
{
	unsigned __int64 CF : 1;                 //进位或者借位,可通过STC和CLC指令来设置和清除
	unsigned __int64 REVERSE1 : 1;           //保留值为1
	unsigned __int64 PF : 1;				 //计算结果的最低字节包含偶数个1,该标志为1
	unsigned __int64 REVERSE2 : 1;           //保留值为0 
	unsigned __int64 AF : 1;				 //辅助进位标志，当半个字节出有进位或者借位
	unsigned __int64 REVERSE3 : 1;           //保留值为0 
	unsigned __int64 ZF : 1;				 //计算结果为0，该标志为1
	unsigned __int64 SF : 1;                 //符号标志，负数为1
	unsigned __int64 TF : 1;                 //陷阱标志
	unsigned __int64 IF : 1;				 //中断标志，为0禁止相应可屏蔽中断，可通过STI和CLI指令来设置和清除
	unsigned __int64 DF : 1;				 //方向标志，为1时字符串操作指令每次操作后地址递减变址寄存器esi和edi，可通过STD和CLD指令来设置和清除
	unsigned __int64 OF : 1;				 //溢出标志，结果超出机器的表达范围为1
	unsigned __int64 IOPL : 2;				 //表示当前任务的I/O权限级别
	unsigned __int64 NT : 1;				 //任务嵌套标志，为1表示当前任务是链接到前面执行的任务，通常是由于中断或者异常触发了IDT表中的任务门
	unsigned __int64 REVERSE4 : 1;           //保留值为0 
	unsigned __int64 RF : 1;                 //控制处理器对调试异常（#DB）的响应，为1时暂时禁止由于指令断点（通过调试寄存器设置的指令断点）导致的调试异常
	unsigned __int64 VM : 1;                 //为1是启用虚拟8086模式
	unsigned __int64 AC : 1;                 //设置该标志和CR0寄存器的AM标志可以启用内存对齐检查
	unsigned __int64 VIF : 1;                //与VIP标志一起实现用于奔腾处理器引入的虚拟内存中断机制
	unsigned __int64 VIP : 1;                //与VIF标志一起实现用于奔腾处理器引入的虚拟内存中断机制
	unsigned __int64 ID : 1;                 //用于检测是否支持CPUID指令
	unsigned __int64 REVERSE5 : 42;          //保留值为0
}RFLAGS;


typedef struct CR0
{
	unsigned __int64 PE : 1;                //为1开启保护模式，为0代表实地址模式
	unsigned __int64 MP : 1;
	unsigned __int64 EM : 1;
	unsigned __int64 TS : 1;
	unsigned __int64 ET : 1;
	unsigned __int64 NE : 1;
	unsigned __int64 REVERSE1 : 10;
	unsigned __int64 WP : 1;               //为1时，禁止内核代码写用户级的只读内存页，为0时允许
	unsigned __int64 REVERSE2 : 1;
	unsigned __int64 AM : 1;               //为1时启用自动内存对齐检查，为0时禁止
	unsigned __int64 REVERSE3 : 10;
	unsigned __int64 NW : 1;
	unsigned __int64 CD : 1;
	unsigned __int64 PG : 1;               //为1时启用页机制，为0时禁止
	unsigned __int64 REVERSE4 : 32;
}CR0;


typedef struct CR2
{
	unsigned __int64 VirtualAddress;       //记录发生缺页异常非法访问的内存地址
}CR2;


typedef struct CR3
{
	union
	{
		struct  
		{                                           //CR4.PCIDE = 0
			unsigned __int64 REVERSE1 : 2;
			unsigned __int64 PWT : 1;               //控制页目录的缓存方式，为1时启用write-through方式，为0启用write-back方式
			unsigned __int64 PCD : 1;               //控制是否对当前页目录进行高速缓存，为1禁止，为0允许
			unsigned __int64 REVERSE2 : 7;
		}s;
		unsigned __int64 PCID : 12;                 //CR4.PCIDE = 1,代表进程上下文ID号
	}u;
	
	unsigned __int64 DirBase : 52;
}CR3;


typedef struct CR4
{
	unsigned __int64 VME : 1;                //启用虚拟8086模式下的中断和异常处理扩展,将中断和异常重定向到8086的处理例程以减少调用虚拟8086监视程序的开销
	unsigned __int64 PVI : 1;                //为1时启用硬件支持的虚拟中断标志（VIF）,为0禁止VIF标志
	unsigned __int64 TSD : 1;
	unsigned __int64 DE : 1;                 //为1时引用DR4和DR5寄存器将导致无效指令（#UD）异常，为0时引用DR4和DR5等价于引用DR6和DR7
	unsigned __int64 PSE : 1;                //为1时启用4MB内存页，为0时限制内存页为4KB
	unsigned __int64 PAE : 1;                //为1时支持36位或36位以上的物理内存地址，为0时限制物理地址为32位
	unsigned __int64 MCE : 1;                //为1时启用机器检查异常，为0时禁止
	unsigned __int64 PGE : 1;                //为1时启用P6处理器引入的全局页功能，为0时禁止
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



