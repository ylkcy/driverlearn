#include <ntifs.h>


#define DEVICE_NAME L"\\device\\ntmodeldrv"
#define LINK_NAME L"\\dosdevices\\ntmodeldrv"


#define IOCTRL_BASE 0x800
#define MYIOCTRL_CODE(code) CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTRL_BASE + code, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define CTL_HELLO	MYIOCTRL_CODE(0)


NTSTATUS DispatchCommon(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchCommon ok!\n");

	pIrp->IoStatus.Status=STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchCreate(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchCreate ok!\n");

	pIrp->IoStatus.Status=STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchRead(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchRead ok!\n");

	ULONG uReadMin = 0;
	ULONG uReadLength = 0;
	PVOID pReadBuffer = NULL;
	PIO_STACK_LOCATION pStack = NULL;
	

	pReadBuffer = pIrp->AssociatedIrp.SystemBuffer;
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	uReadLength = pStack->Parameters.Read.Length;

	//todo


	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = uReadMin;
	
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchWrite(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchWrite ok!\n");

	ULONG uWriteMin = 0;
	ULONG uWriteLength = 0;
	PVOID pWriteBuff = NULL;
	PIO_STACK_LOCATION pStack = NULL;
	
	pWriteBuff = pIrp->AssociatedIrp.SystemBuffer;
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	uWriteLength = pStack->Parameters.Write.Length;

	//todo

	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = uWriteMin;
	
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchIoctrl(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchIoctrl ok!\n");

	ULONG uIoctrlCode = 0;
	PVOID pInputBuff = NULL;
	PVOID pOutputBuff = NULL;
	ULONG uInputLength = 0;
	ULONG uOutputLength = 0;
	PIO_STACK_LOCATION pStack = NULL;

	pInputBuff = pOutputBuff = pIrp->AssociatedIrp.SystemBuffer;
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	uInputLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
	uOutputLength = pStack->Parameters.DeviceIoControl.OutputBufferLength;
	uIoctrlCode = pStack->Parameters.DeviceIoControl.IoControlCode;

	switch(uIoctrlCode)
	{
	//todo
	default:
		break;
	}

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchClean(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchClean ok!\n");

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DispatchClose(PDEVICE_OBJECT pObject, PIRP pIrp)
{
	DbgPrint("DispatchClose ok!\n");

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING uLinkName = { 0 };
	RtlInitUnicodeString(&uLinkName, LINK_NAME);
	IoDeleteSymbolicLink(&uLinkName);
	IoDeleteDevice(pDriverObject->DeviceObject);

	DbgPrint("Driver unload ok!\n");
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	ULONG i = 0;
	NTSTATUS ntStatus = 0;
	UNICODE_STRING uDeviceName = { 0 };
	UNICODE_STRING uLinkName = { 0 };
	PDEVICE_OBJECT pDeviceObject = NULL;

	RtlInitUnicodeString(&uDeviceName, DEVICE_NAME);
	RtlInitUnicodeString(&uLinkName, LINK_NAME);

	ntStatus = IoCreateDevice(pDriverObject, 0, &uDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDeviceObject);
	if(!NT_SUCCESS(ntStatus))
	{
		DbgPrint("IoCreateDevice failed:%x", ntStatus);
		return ntStatus;
	}

	pDeviceObject->Flags |= DO_BUFFERED_IO;

	ntStatus = IoCreateSymbolicLink(&uLinkName, &uDeviceName);
	if(!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(pDeviceObject);
		DbgPrint("IoCreateSymbolicLink failed:%x\n", ntStatus);
		return ntStatus;
	}

	for(i=0;i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DispatchCommon;
	}

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchWrite;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctrl;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = DispatchClean;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;

	pDriverObject->DriverUnload = DriverUnload;

	DbgPrint("Driver load ok!\n");

	return STATUS_SUCCESS;
}
