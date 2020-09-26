#pragma once
#include <ntifs.h>
#include <intrin.h>


typedef struct EFLAGS
{
	unsigned int CF : 1;                 //进位或者借位,可通过STC和CLC指令来设置和清除
	unsigned int Reverse1 : 1;           //保留值为1
	unsigned int PF : 1;				 //计算结果的最低字节包含偶数个1,该标志为1
	unsigned int Reverse3 : 1;           //保留值为0 
	unsigned int AF : 1;				 //辅助进位标志，当半个字节出有进位或者借位
	unsigned int Reverse5 : 1;           //保留值为0 
	unsigned int ZF : 1;				 //计算结果为0，该标志为1
	unsigned int SF : 1;                 //符号标志，负数为1
	unsigned int TF : 1;                 //陷阱标志
	unsigned int IF : 1;				 //中断标志，为0禁止相应可屏蔽中断，可通过STI和CLI指令来设置和清除
	unsigned int DF : 1;				 //方向标志，为1时字符串操作指令每次操作后地址递减变址寄存器esi和edi，可通过STD和CLD指令来设置和清除
	unsigned int OF : 1;				 //溢出标志，结果超出机器的表达范围为1
	unsigned int IOPL : 2;				 //表示当前任务的I/O权限级别
	unsigned int NT : 1;				 //任务嵌套标志，为1表示当前任务是链接到前面执行的任务，通常是由于中断或者异常触发了IDT表中的任务门
	unsigned int Reverse15 : 1;          //保留值为0 
	unsigned int RF : 1;                 //控制处理器对调试异常（#DB）的响应，为1时暂时禁止由于指令断点（通过调试寄存器设置的指令断点）导致的调试异常
	unsigned int VM : 1;                 //为1是启用虚拟8086模式
	unsigned int AC : 1;                 //设置该标志和CR0寄存器的AM标志可以启用内存对齐检查
	unsigned int VIF : 1;                //与VIP标志一起实现用于奔腾处理器引入的虚拟内存中断机制
	unsigned int VIP : 1;                //与VIF标志一起实现用于奔腾处理器引入的虚拟内存中断机制
	unsigned int ID : 1;                 //用于检测是否支持CPUID指令
	unsigned int Reverse22 : 10;         //保留值为0
}EFLAGS;


typedef struct CR0
{
	unsigned int PE : 1;                //为1开启保护模式，为0代表实地址模式
	unsigned int MP : 1;                
	unsigned int EM : 1;                
	unsigned int TS : 1;                
	unsigned int ET : 1;
	unsigned int NE : 1;
	unsigned int REVERSE1 : 10;
	unsigned int WP : 1;               //为1时，禁止内核代码写用户级的只读内存页，为0时允许
	unsigned int REVERSE2 : 1;
	unsigned int AM : 1;               //为1时启用自动内存对齐检查，为0时禁止
	unsigned int REVERSE : 10;
	unsigned int NW : 1;
	unsigned int CD : 1;
	unsigned int PG : 1;               //为1时启用页机制，为0时禁止
}CR0;


typedef struct CR2
{
	unsigned int VirtualAddress;
}CR2;


typedef struct CR3
{
	unsigned int REVERSE1 : 2;
	unsigned int PWT : 1;               //控制页目录的缓存方式，为1时启用write-through方式，为0启用write-back方式
	unsigned int PCD : 1;               //控制是否对当前页目录进行高速缓存，为1禁止，为0允许
	unsigned int REVERSE2 : 7;
	unsigned int DirBase : 20;
}CR3;


typedef struct CR4
{
	unsigned int VME : 1;                //启用虚拟8086模式下的中断和异常处理扩展,将中断和异常重定向到8086的处理例程以减少调用虚拟8086监视程序的开销
	unsigned int PVI : 1;                //为1时启用硬件支持的虚拟中断标志（VIF）,为0禁止VIF标志
	unsigned int TSD : 1;
	unsigned int DE : 1;                 //为1时引用DR4和DR5寄存器将导致无效指令（#UD）异常，为0时引用DR4和DR5等价于引用DR6和DR7
	unsigned int PSE : 1;                //为1时启用4MB内存页，为0时限制内存页为4KB
	unsigned int PAE : 1;                //为1时支持36位或36位以上的物理内存地址，为0时限制物理地址为32位
	unsigned int MCE : 1;                //为1时启用机器检查异常，为0时禁止
	unsigned int PGE : 1;                //为1时启用P6处理器引入的全局页功能，为0时禁止
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

