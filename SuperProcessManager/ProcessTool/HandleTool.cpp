#include "StdAfx.h"
#include "HandleTool.h"
#include <strsafe.h>

CHandleTool::~CHandleTool()
{
	VirtualFree(m_pQuerySystemInfoBuffer,m_QuerySystemInfoBufferSize,MEM_DECOMMIT);
	VirtualFree(m_pObjectTypeBuffer,m_ObjectTypeBufferSize,MEM_DECOMMIT);
	VirtualFree(m_pObjectNameBuffer,m_ObjectNameBufferSize,MEM_DECOMMIT);
}



BOOL CHandleTool::Create()
{
	//申请BUFFER
	m_QuerySystemInfoBufferSize = 2 * 1024 * 1024;
	m_pQuerySystemInfoBuffer = (PULONG)VirtualAlloc(NULL,m_QuerySystemInfoBufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	if(m_pQuerySystemInfoBuffer == NULL)
		return FALSE;

	m_ObjectTypeBufferSize =1 * 1024 * 1024;
	m_pObjectTypeBuffer = (LPBYTE)VirtualAlloc(NULL,m_ObjectTypeBufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	if(m_pObjectTypeBuffer == NULL)
		return FALSE;

	m_ObjectNameBufferSize = 4 * 1024 * 1024;
	m_pObjectNameBuffer = (LPBYTE)VirtualAlloc(NULL,m_ObjectNameBufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	if(m_pObjectNameBuffer == NULL)
		return FALSE;


	//获取Native API 地址
	ZwQuerySystemInformation = (ZwQuerySystemInformationT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"ZwQuerySystemInformation");
	if(ZwQuerySystemInformation == NULL)
		return FALSE;
	
	ZwQueryObject = (ZwQueryObjectT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"ZwQueryObject");
	if(ZwQueryObject == NULL)
		return FALSE;

	ZwDuplicateObject = (ZwDuplicateObjectT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"ZwDuplicateObject");

	if(ZwDuplicateObject == NULL)
		return FALSE;

	return TRUE;
}

BOOL CHandleTool::CreateSnapshotHandle(DWORD dwPID)
{
	NTSTATUS Status		= 0;
	DWORD dwSize		= 0;
	
	do
	{
		Status = ZwQuerySystemInformation(SystemHandleInformation,
			m_pQuerySystemInfoBuffer,
			m_QuerySystemInfoBufferSize,&dwSize);
		
		//缓冲区不能容纳数据 重新申请
		if (Status == STATUS_INFO_LENGTH_MISMATCH) 
		{
			//先释放内存
			VirtualFree(m_pQuerySystemInfoBuffer,m_QuerySystemInfoBufferSize,MEM_DECOMMIT);
			m_QuerySystemInfoBufferSize +=m_QuerySystemInfoBufferSize;
			m_pQuerySystemInfoBuffer = (PULONG)VirtualAlloc(NULL,m_QuerySystemInfoBufferSize, MEM_COMMIT, PAGE_READWRITE);
		}

	} while (Status == STATUS_INFO_LENGTH_MISMATCH);

	if(!NT_SUCCESS(Status))
		return FALSE;

	m_pHandleInfo = (PSYSTEM_HANDLE_INFORMATION)(m_pQuerySystemInfoBuffer+1);
	m_dwPID = dwPID;
	return TRUE;
}



BOOL CHandleTool::Handle32First(PSYSTEM_HANDLE_INFORMATION * Offset)
{
	*Offset =(PSYSTEM_HANDLE_INFORMATION)m_pHandleInfo;
	if(*Offset == NULL) return FALSE;
	m_nIndex = 0;
	m_nCount = *m_pQuerySystemInfoBuffer;
	return TRUE;
}


BOOL CHandleTool::Handle32Next(PSYSTEM_HANDLE_INFORMATION * Offset)
{
	for(m_nIndex;m_nIndex<m_nCount;m_nIndex++)
	{
		if ( (*Offset)[m_nIndex].ProcessId == m_dwPID)
		{
			m_nIndex++;
			return TRUE;
		}
	}
	return FALSE;
}



BOOL CHandleTool::GetHandleExtendInfo(PSYSTEM_HANDLE_INFORMATION Offset,LPEXTENDHANDLEINFO lpExtendInfo)
{
	//初始化
	ZeroMemory(lpExtendInfo,sizeof(EXTENDHANDLEINFO));
	ZeroMemory(m_pObjectTypeBuffer,m_ObjectTypeBufferSize);
	ZeroMemory(m_pObjectNameBuffer,m_ObjectNameBufferSize);

	BOOL bRet = FALSE,bNameQueryResult=FALSE;
	NTSTATUS Status;
	ULONG nReturnLength=0;
	HANDLE hProcess = 0,hObject = 0;
	PUBLIC_OBJECT_BASIC_INFORMATION ObjectBase;


	do 
	{
		//尝试打开进程
		hProcess = OpenProcess(PROCESS_DUP_HANDLE,FALSE,m_dwPID);
		if(hProcess == NULL) break;

		//复制句柄
		Status = ZwDuplicateObject(hProcess,
									(HANDLE)Offset[m_nIndex].Handle, 
									GetCurrentProcess(), 
									&hObject,                         
									0, 
									0,
									DUPLICATE_SAME_ATTRIBUTES);
	
		if(!NT_SUCCESS(Status))
			break;

		Status = ZwQueryObject(hObject,ObjectBasicInformation, &ObjectBase,sizeof(ObjectBase),&nReturnLength);
		if(!NT_SUCCESS(Status))
			break;

		//查询句柄类型名
		Status = ZwQueryObject(hObject,
			ObjectTypeInformation, 
			m_pObjectTypeBuffer,
			m_ObjectTypeBufferSize,
			&nReturnLength);

		if(!NT_SUCCESS(Status))
			break;

		//type 查询成功，就算成功了。
		bRet = TRUE;

		
		//查询句柄类型名对于的字符串值
		/*
		nReturnLength=0;
		Status = ZwQueryObject(hObject,
			ObjectNameInformation, 
			m_pObjectNameBuffer,
			m_ObjectNameBufferSize,
			&nReturnLength);

		if(!NT_SUCCESS(Status))
			break;

		bNameQueryResult = TRUE;
		*/


	} while (FALSE);

	//收集信息
	if(bRet)
	{
		if(((POBJECT_NAME_INFORMATION)m_pObjectTypeBuffer)->Name.Buffer)
		StringCbCopyW(lpExtendInfo->szOjectType,
			EXTENDHANDLEINFO_BUFFER_LEN,
			((POBJECT_NAME_INFORMATION)m_pObjectTypeBuffer)->Name.Buffer);

		/*
		if(bNameQueryResult && ((POBJECT_NAME_INFORMATION)m_pObjectNameBuffer)->Name.Buffer) {
			StringCbCopyW(lpExtendInfo->szOjectName,EXTENDHANDLEINFO_BUFFER_LEN,
				((POBJECT_NAME_INFORMATION)m_pObjectNameBuffer)->Name.Buffer);
		}*/

		StringCbCopyW(lpExtendInfo->szOjectName,EXTENDHANDLEINFO_BUFFER_LEN,TEXT("R3下ZwQueryObject有几率会挂起进程，该功能开发中，下个版本R0实现"));


		lpExtendInfo->AcCount = ObjectBase.PointerCount;
		lpExtendInfo->hHandle = (HANDLE)Offset[m_nIndex].Handle;
		lpExtendInfo->uTypeVaule = (ULONG)Offset[m_nIndex].ObjectTypeNumber;
		
		DWORD dwFlags = 0;
 		GetHandleInformation(hObject,&dwFlags);
		lpExtendInfo->bInherit = dwFlags & HANDLE_FLAG_INHERIT;
 		lpExtendInfo->bClose = dwFlags & HANDLE_FLAG_PROTECT_FROM_CLOSE;
	}

	if(hProcess) CloseHandle(hProcess);
	if(hObject)	CloseHandle(hObject);
	return bRet;
}


