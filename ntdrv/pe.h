#ifndef _PE_H
#define _PE_H

#include <ntddk.h>
#include <ntimage.h>

#define PE_ERROR_VALUE 0xFFFFFFFF

ULONG RvaToSection(IMAGE_NT_HEADERS* pNtHdr, ULONG dwRVA);
ULONG RvaToOffset(PIMAGE_NT_HEADERS pnth, ULONG Rva, ULONG FileSize);
ULONG GetExportOffset(PCHAR* pFileData, ULONG uFileSize, PCHAR pExportName);


#endif