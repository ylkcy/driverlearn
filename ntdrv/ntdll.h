#ifndef _NTDLL_H
#define _NTDLL_H


#include <ntddk.h>


NTSTATUS Initialize();
VOID UnInitialize();
ULONG GetExportSsdtIndex(const char* ExportName);

PCHAR* gpFileData = NULL;
ULONG guFileSize = 0;


#endif //_NTDLL_H