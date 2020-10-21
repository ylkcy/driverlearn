#include "pe.h"


ULONG RvaToSection(IMAGE_NT_HEADERS* pNtHdr, ULONG dwRVA)
{
	ULONG i = 0;
    USHORT wSections = 0;
    PIMAGE_SECTION_HEADER pSectionHdr = NULL;
    pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);
    wSections = pNtHdr->FileHeader.NumberOfSections;
	for (i = 0; i < wSections; i++)
    {
		if (pSectionHdr[i].VirtualAddress <= dwRVA && dwRVA < (pSectionHdr[i].VirtualAddress + pSectionHdr[i].Misc.VirtualSize))
		{  
			return i;
        }
    }
	return PE_ERROR_VALUE;
}


ULONG RvaToOffset(PIMAGE_NT_HEADERS pnth, ULONG Rva, ULONG uFileSize)
{
	ULONG i = 0;
    PIMAGE_SECTION_HEADER psh = IMAGE_FIRST_SECTION(pnth);
    USHORT NumberOfSections = pnth->FileHeader.NumberOfSections;
    for(i = 0; i < NumberOfSections; i++)
    {
		if (psh->VirtualAddress <= Rva && Rva < (psh->VirtualAddress + psh->Misc.VirtualSize))
        {
            Rva -= psh->VirtualAddress;
            Rva += psh->PointerToRawData;
			return Rva < uFileSize ? Rva : PE_ERROR_VALUE;
        }
        psh++;
    }
	return PE_ERROR_VALUE;
}


ULONG GetExportOffset(PCHAR pFileData, ULONG uFileSize, PCHAR pExportName)
{
	ULONG i = 0;
	ULONG uExportOffset = 0;
	ULONG uCurrentNameOffset = 0;
	ULONG uCurrentFunctionRva = 0;

	ULONG uExportDirRva = 0;
	ULONG uExportDirSize = 0;
	ULONG uExportDirOffset = 0;

	ULONG uNumberOfNames = 0;
	ULONG uAddressOfFunctionsOffset = 0;
	ULONG uAddressOfNameOrdinalsOffset = 0;
	ULONG uAddressOfNamesOffset = 0;
	PULONG pAddressOfFunctions = NULL;
	PUSHORT pAddressOfNameOrdinals = NULL;
	PULONG pAddressOfNames = NULL;

	PCHAR pCurrentFunName = NULL;
	PIMAGE_DOS_HEADER pDosHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	PIMAGE_DATA_DIRECTORY pDataDir = NULL;
	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;

	pDosHeader = (PIMAGE_DOS_HEADER)pFileData;
	if (IMAGE_DOS_SIGNATURE != pDosHeader->e_magic)
    {
        DbgPrint("Invalid IMAGE_DOS_SIGNATURE!\r\n");
        return PE_ERROR_VALUE;
    }

	pNtHeader = (PIMAGE_NT_HEADERS)(pFileData + pDosHeader->e_lfanew);
	if (IMAGE_NT_SIGNATURE != pNtHeader->Signature)
    {
        DbgPrint("Invalid IMAGE_NT_SIGNATURE!\r\n");
        return PE_ERROR_VALUE;
    }

	if (IMAGE_NT_OPTIONAL_HDR64_MAGIC == pNtHeader->OptionalHeader.Magic)
	{
		pDosHeader = ((PIMAGE_NT_HEADERS64)pNtHeader)->OptionalHeader.DataDirectory;
	}   
	else
	{
		pDosHeader = ((PIMAGE_NT_HEADERS32)pNtHeader)->OptionalHeader.DataDirectory;
	}
       
    uExportDirRva = pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    uExportDirSize = pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    ULONG uExportDirOffset = RvaToOffset(pNtHeader, uExportDirRva, uFileSize);
	if (PE_ERROR_VALUE == uExportDirOffset)
    {
        DbgPrint("Invalid Export Directory!\r\n");
        return PE_ERROR_VALUE;
    }

    pExportDir = (PIMAGE_EXPORT_DIRECTORY)(pFileData + uExportDirOffset);
	uNumberOfNames = pExportDir->NumberOfNames;
    uAddressOfFunctionsOffset = RvaToOffset(pNtHeader, pExportDir->AddressOfFunctions, uFileSize);
	uAddressOfNameOrdinalsOffset = RvaToOffset(pNtHeader, pExportDir->AddressOfNameOrdinals, uFileSize);
	uAddressOfNamesOffset = RvaToOffset(pNtHeader, pExportDir->AddressOfNames, uFileSize);

	if (PE_ERROR_VALUE == uAddressOfFunctionsOffset
		|| PE_ERROR_VALUE == uAddressOfNameOrdinalsOffset
		|| PE_ERROR_VALUE == uAddressOfNamesOffset)
    {
        DbgPrint("Invalid Export Directory Contents!\r\n");
        return PE_ERROR_VALUE;
    }

    pAddressOfFunctions = (PULONG)(pFileData + uAddressOfFunctionsOffset);
    pAddressOfNameOrdinals = (PUSHORT)(pFileData + uAddressOfNameOrdinalsOffset);
    pAddressOfNames = (PULONG)(pFileData + uAddressOfNamesOffset);

    uExportOffset = PE_ERROR_VALUE;
    for(i = 0; i < uNumberOfNames; i++)
    {
        uCurrentNameOffset = RvaToOffset(pNtHeader, pAddressOfNames[i], uFileSize);
		if (PE_ERROR_VALUE == uCurrentNameOffset)
		{
			continue;
		}
         
		pCurrentFunName = (PCHAR)(pFileData + uCurrentNameOffset);
        uCurrentFunctionRva = pAddressOfFunctions[pAddressOfNameOrdinals[i]];
		if (uCurrentFunctionRva >= uExportDirRva && uCurrentFunctionRva < uExportDirRva + uExportDirSize)
		{//ignore forwarded exports
			continue; 
		}
            
        if(!strcmp(pCurrentFunName, pExportName))
        {
            uExportOffset = RvaToOffset(pNtHeader, uCurrentFunctionRva, uFileSize);
            break;
        }
    }

    if(uExportOffset == PE_ERROR_VALUE)
    {
        DbgPrint("Export %s not found in export table!\r\n", pExportName);
    }

    return uExportOffset;
}
