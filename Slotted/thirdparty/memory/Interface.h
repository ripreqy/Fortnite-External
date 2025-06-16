#pragma once

typedef struct _TransferData
{
	int Code;
	int TargetPID;

	long x;
	long y;
	unsigned short button_flags;
	ULONGLONG* address;
	uintptr_t readaddress;
	uintptr_t Buffer;
	uintptr_t Size;
} TransferData, * PTransferData;
enum MouseFlags
{
	None = 0,
	LeftButtonDown = 1,
	LeftButtonUp = 2,
	RightButtonDown = 4,
	RightButtonUp = 8,
	MiddleButtonDown = 16,
	MiddleButtonUp = 32,
	XButton1Down = 64,
	XButton1Up = 128,
	XButton2Down = 256,
	XButton2Up = 512,
	MouseWheel = 1024,
	MouseHorizontalWheel = 2048
};


#define CodeGetBase 0xc8f6
#define CodeGetCR3 0xc8f7
#define CodeReadPhysical 0xc8f8
#define CodeMouse 0xc8f9


#define IOGetPEB CTL_CODE(FILE_DEVICE_UNKNOWN, CodeGetBase, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOGetCR3 CTL_CODE(FILE_DEVICE_UNKNOWN, CodeGetCR3, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOReadPhysical CTL_CODE(FILE_DEVICE_UNKNOWN, CodeReadPhysical, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOMouse CTL_CODE(FILE_DEVICE_UNKNOWN, CodeMouse, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)


typedef struct ___IO_STATUS_BLOCK
{
	union
	{
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} __IO_STATUS_BLOCK, * __PIO_STATUS_BLOCK;

typedef void(__stdcall* __PIO_APC_ROUTINE)(PVOID ApcContext, __PIO_STATUS_BLOCK IoStatusBlock, ULONG Reserved);

extern "C" __int64 __device_io_control_file(HANDLE FileHandle, HANDLE Event, __PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, __PIO_STATUS_BLOCK IoStatusBlock, ULONG IoControlCode, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength);