#include "ntdll.h"
#include "pe.h"


NTSTATUS Initialize()
{
	NTSTATUS NtStatus = STATUS_UNSUCCESSFUL;
	HANDLE hFile = NULL;
	UNICODE_STRING FileName = { 0 };
	OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
	IO_STATUS_BLOCK IoStatusBlock = { 0 };
	FILE_STANDARD_INFORMATION StandardInformation = { 0 };
	LARGE_INTEGER ByteOffset = { 0 };

    RtlInitUnicodeString(&FileName, L"\\SystemRoot\\system32\\ntdll.dll");
    InitializeObjectAttributes(&ObjectAttributes, &FileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	NtStatus = ZwCreateFile(&hFile, GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if(!NT_SUCCESS(NtStatus))
	{
		DbgPrint("ZwCreateFile Error");
		return NtStatus;
	}

    NtStatus = ZwQueryInformationFile(hFile, &IoStatusBlock, &StandardInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
	if (!NT_SUCCESS(NtStatus))
	{
		DbgPrint("ZwQueryInformationFile Error");
		return NtStatus;
	}

    guFileSize = StandardInformation.EndOfFile.LowPart;
	DbgPrint("NtDll FileSize is %08X!\r\n", guFileSize);

	gpFileData = ExAllocatePool(NonPagedPool, guFileSize);
	if (!NT_SUCCESS(NtStatus))
	{
		DbgPrint("ExAllocatePool Error");
		return NtStatus;
	}

	RtlZeroMemory(gpFileData, guFileSize);

    NtStatus = ZwReadFile(hFile, NULL, NULL, NULL, &IoStatusBlock, gpFileData, guFileSize, &ByteOffset, NULL);
    if(!NT_SUCCESS(NtStatus))
    {
         ExFreePool(gpFileData);
		 DbgPrint("ZwReadFile Error");
		 return NtStatus;
    }

    ZwClose(hFile);

    return NtStatus;
}


void UnInitialize()
{
    RtlFreeMemory(gpFileData);
	gpFileData = NULL;
	guFileSize = 0;
}


int GetExportSsdtIndex(PCHAR pExportName)
{
    ULONG_PTR ExportOffset = GetExportOffset(gpFileData, guFileSize, pExportName);
    if(ExportOffset == PE_ERROR_VALUE)
        return -1;

    int SsdtOffset = -1;
    unsigned char* ExportData = gpFileData + ExportOffset;
    for(int i = 0; i < 32 && ExportOffset + i < guFileSize; i++)
    {
        if(ExportData[i] == 0xC2 || ExportData[i] == 0xC3)  //RET
            break;
        if(ExportData[i] == 0xB8)  //mov eax,X
        {
            SsdtOffset = *(int*)(ExportData + i + 1);
            break;
        }
    }

    if(SsdtOffset == -1)
    {
        DPRINT("[DeugMessage] SSDT Offset for %s not found...\r\n", pExportName);
    }

    return SsdtOffset;
}