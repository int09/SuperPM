#include "StdAfx.h"
#include "ThreadTool.h"
#include <Psapi.h>
#pragma comment(lib,"psapi.lib")


CThreadTool::~CThreadTool()
{
	VirtualFree(m_pQueryBuffer,m_QueryBufferSize,MEM_DECOMMIT);
}


BOOL CThreadTool::Create()
{

	m_QueryBufferSize = 2 * 1024 * 1024;
	m_pQueryBuffer = (LPBYTE)VirtualAlloc(NULL,m_QueryBufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	if(m_pQueryBuffer == NULL)
		return FALSE;

	NtQuerySystemInformation=(NtQuerySystemInformationT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"NtQuerySystemInformation"); 
	
	if(NtQuerySystemInformation == NULL)
		return FALSE;

	ZwQueryInformationThread = (ZwQueryInformationThreadT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"ZwQueryInformationThread"); 

	if(ZwQueryInformationThread == NULL)
		return FALSE;

	return TRUE;
}


BOOL CThreadTool::CreateSnapshotThread(DWORD dwPID)
{
	m_pSPI = NULL;
	NTSTATUS status = 0;
	ULONG ulSize = 0;
	do
	{
		status = NtQuerySystemInformation(SystemProcessesAndThreadsInformation,
			m_pQueryBuffer, m_QueryBufferSize, &ulSize);

		if (status == STATUS_INFO_LENGTH_MISMATCH) 
		{
			VirtualFree(m_pQueryBuffer,m_QueryBufferSize,MEM_DECOMMIT);
			m_QueryBufferSize +=m_QueryBufferSize;
			m_pQueryBuffer = (LPBYTE)VirtualAlloc(NULL,m_QueryBufferSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
			
			if(m_pQueryBuffer == NULL) return FALSE;
		}
	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	m_pSPI = (PSYSTEM_PROCESS_INFORMATION)m_pQueryBuffer;
	
	while(m_pSPI->UniqueProcessId != dwPID)
	{
		if (m_pSPI->NextEntryOffset == 0)
			break;
		
		m_pSPI = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)m_pSPI + m_pSPI->NextEntryOffset);
	}
	return TRUE;
}



BOOL CThreadTool::Thread32First(LPTHREADINFO * Offset)
{
	*Offset =(LPTHREADINFO)&m_pSPI->TH[0];
	if(*Offset == NULL) return FALSE;
	
	m_nIndex = 0;
	m_nCount = m_pSPI->NumberOfThreads;
	return TRUE;
}


BOOL CThreadTool::Thread32Next(LPTHREADINFO * Offset)
{
	if(m_nIndex == m_nCount-1)
		return FALSE;

	(*Offset)++;
	m_nIndex++;
	return TRUE;
}

BOOL CThreadTool::GetThreadExtendInfo(LPTHREADINFO Offset,LPEXTENDTHREADINFO pExtendInfo)
{
	ZeroMemory(pExtendInfo,sizeof(EXTENDTHREADINFO));

	HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,FALSE,Offset->ClientId.UniqueThread);
	if(hThread == NULL)
		return FALSE;

	//�߳�״̬
	INT dwExitCode;
	if(Offset->State == StateWait && Offset->WaitReason == Suspended)
		pExtendInfo->lpszThreadStatus = TEXT("����");
	else if(GetExitCodeThread(hThread,(LPDWORD)&dwExitCode))
	{
		if(dwExitCode == STILL_ACTIVE)
			pExtendInfo->lpszThreadStatus =TEXT("����");
	}else
	{
		pExtendInfo->lpszThreadStatus =TEXT("�˳�");
	}
	//�߳����ȼ�
	switch(::GetThreadPriority(hThread))
	{
		case THREAD_PRIORITY_IDLE:
			pExtendInfo->lpszPrority=TEXT("����");
			break;
		case THREAD_PRIORITY_LOWEST:
			pExtendInfo->lpszPrority=TEXT("��");
			break;
		case THREAD_PRIORITY_BELOW_NORMAL:
			pExtendInfo->lpszPrority=TEXT("���ڱ�׼");
			break;
		case THREAD_PRIORITY_NORMAL:
			pExtendInfo->lpszPrority=TEXT("��׼");
			break;
		case THREAD_PRIORITY_ABOVE_NORMAL:
			pExtendInfo->lpszPrority=TEXT("���ڱ�׼");
			break;
		case THREAD_PRIORITY_HIGHEST:
			pExtendInfo->lpszPrority=TEXT("��");
			break;
		case THREAD_PRIORITY_TIME_CRITICAL:
			pExtendInfo->lpszPrority=TEXT("ʵʱ");
			break;
		default:
			pExtendInfo->lpszPrority=TEXT("��ȱ");
	}

	//�߳�����
	DWORD dwLenght=0;
	HDESK hDesk = GetThreadDesktop(Offset->ClientId.UniqueThread);
	GetUserObjectInformation(hDesk,UOI_NAME,pExtendInfo->szThreadDesktop,MAX_PATH,&dwLenght);


	//�߳���ڵ�ַ
	NTSTATUS Status = ZwQueryInformationThread(
		hThread,
		ThreadQuerySetWin32StartAddress,   
		&pExtendInfo->dwEntryAddress,   
		sizeof(DWORD),   
        NULL);
	
	if(!NT_SUCCESS(Status))
		return FALSE;

	
	//�߳�����ģ��
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,FALSE,Offset->ClientId.UniqueProcess);
	if(hProcess)
	{
		MODULEINFO mi={0};
		//�����Ƿ������߳�
		if(GetModuleInformation(hProcess,NULL,&mi,sizeof(MODULEINFO)))
		{
			if((DWORD)mi.EntryPoint == pExtendInfo->dwEntryAddress)
				pExtendInfo->bIsMainThread = TRUE;
		}
		GetMappedFileName(hProcess,(LPVOID)pExtendInfo->dwEntryAddress,pExtendInfo->szThreadModule,MAX_PATH);
	}
	if(hThread) CloseHandle(hThread);
	if(hProcess) CloseHandle(hProcess);

	return TRUE;
}

BOOL CThreadTool::SusppedProcess(DWORD dwPID)
{
	if(!CreateSnapshotThread(dwPID))
		return FALSE;
	
	LPTHREADINFO lpThreadInfo;
	Thread32First(&lpThreadInfo);
	HANDLE hThread = NULL;
	
	do
	{
		hThread = OpenThread( THREAD_SUSPEND_RESUME , FALSE,lpThreadInfo->ClientId.UniqueThread);
		if(hThread){
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	}while(Thread32Next(&lpThreadInfo));
	return TRUE;
}

BOOL CThreadTool::ResumeProcess(DWORD dwPID)
{
	if(!CreateSnapshotThread(dwPID))
		return FALSE;
	
	LPTHREADINFO lpThreadInfo;
	Thread32First(&lpThreadInfo);
	HANDLE hThread = NULL;
	
	do
	{
		hThread = OpenThread( THREAD_SUSPEND_RESUME , FALSE,lpThreadInfo->ClientId.UniqueThread);
		if(hThread)
		{
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	}while(Thread32Next(&lpThreadInfo));
	return TRUE;
}

DWORD CThreadTool::GetThreadEntryAddress(DWORD dwTID)
{
	HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,FALSE,dwTID);
	if(hThread == NULL)
		return 0;

	DWORD dwResult = 0;
	//�߳���ڵ�ַ
	NTSTATUS Status = ZwQueryInformationThread(
		hThread,
		ThreadQuerySetWin32StartAddress,   
		&dwResult,   
		sizeof(DWORD),   
        NULL);
	
	if(!NT_SUCCESS(Status))
		return 0;

	return dwResult;
}