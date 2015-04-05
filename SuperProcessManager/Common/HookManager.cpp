#include "StdAfx.h"
#include "HookManager.h"
#include <strsafe.H>
#include <TLHELP32.H>

CHookManager::CHookManager()
{
	m_hModule = NULL;
}

CHookManager::~CHookManager()
{
	if(m_hModule)
		FreeLibrary(m_hModule);
}


BOOL CHookManager::Create(LPCTSTR szDllPath)
{
	m_hModule = LoadLibraryEx(szDllPath,NULL,DONT_RESOLVE_DLL_REFERENCES);
	if(m_hModule == NULL)
		return FALSE;
	
	StringCbCopy(m_szDllPath,sizeof(m_szDllPath),szDllPath);

	m_SelectHookRecord = (SelectHookRecordT)GetProcAddress(m_hModule,"SelectHookRecord");
	m_InsertHookRecord = (InsertHookRecordT)GetProcAddress(m_hModule,"InsertHookRecord");
	m_UpdateHookRecord = (UpdateHookRecordT)GetProcAddress(m_hModule,"UpdateHookRecord");
	m_DeleteHookRecord = (DeleteHookRecordT)GetProcAddress(m_hModule,"DeleteHookRecord");
	m_UnHookNetworkIo  = (UnHookNetworkIoT)GetProcAddress(m_hModule,"UnHookNetworkIo");
	m_HookNetworkIo	   = (HookNetworkIoT)GetProcAddress(m_hModule,"HookNetworkIo");
	return TRUE;
}


LPHOOK_RECORD WINAPI CHookManager::SelectHookRecord(DWORD dwPID)
{
	if(m_SelectHookRecord == NULL)
	{
		SetLastError(487);
		return NULL;
	}
	return m_SelectHookRecord(dwPID);
}
BOOL WINAPI CHookManager::InsertHookRecord(LPHOOK_RECORD lpRecord)
{
	if(m_InsertHookRecord == NULL)
	{
		SetLastError(487);
		return NULL;
	}
	return m_InsertHookRecord(lpRecord);
}
BOOL WINAPI CHookManager::UpdateHookRecord(LPHOOK_RECORD lpRecord)
{
	if(m_UpdateHookRecord == NULL)
	{
		SetLastError(487);
		return NULL;
	}
	return m_UpdateHookRecord(lpRecord);
}
BOOL WINAPI CHookManager::DeleteHookRecord(LPHOOK_RECORD lpRecord)
{
	if(m_DeleteHookRecord == NULL)
	{
		SetLastError(487);
		return NULL;
	}
	return m_DeleteHookRecord(lpRecord);
}



BOOL WINAPI CHookManager::CreateRemoteThread(DWORD dwPID,LPTHREAD_START_ROUTINE lsr,LPVOID lpParameter)
{
	HANDLE hProcess = NULL;
	DWORD dwLastError = 0;
	HANDLE hThread = NULL;
	BOOL   bRet = FALSE;

	do 
	{
		hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,
			FALSE,dwPID);
		if(hProcess == NULL)
			break;

		hThread = ::CreateRemoteThread(hProcess,NULL,NULL,lsr,NULL,NULL,NULL);
		if(hThread == NULL)
			break;

		bRet = TRUE;
	} while (FALSE);

	dwLastError = GetLastError();

	if( hProcess) CloseHandle(hProcess);
	if( hThread ) CloseHandle(hThread);
	SetLastError(dwLastError);
	return bRet;
}

DWORD WINAPI CHookManager::GetModuleBase(DWORD dwPID,LPCTSTR szDllPath)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD dwBaseAddress = 0;
	MODULEENTRY32 ModuleData;
	ZeroMemory(&ModuleData,sizeof(MODULEENTRY32));
	ModuleData.dwSize = sizeof(MODULEENTRY32);
	
	if(!::Module32First(hSnapshot,&ModuleData))
		return FALSE;

	do{
		
		if(0 == _tcscmp(szDllPath,ModuleData.szExePath))
		{
			dwBaseAddress = (DWORD)ModuleData.modBaseAddr;
			break;
		}
	}while(Module32Next(hSnapshot,&ModuleData));

	CloseHandle(hSnapshot);
	return dwBaseAddress;
}

BOOL WINAPI CHookManager::HookNetworkIo(DWORD dwPID)
{
	//先计算偏移
	DWORD dwTarget = GetModuleBase(dwPID,m_szDllPath);
	DWORD dwOffset = (DWORD)m_HookNetworkIo - (DWORD)m_hModule;
	return CreateRemoteThread(dwPID,(LPTHREAD_START_ROUTINE)(dwTarget+dwOffset));
}


BOOL WINAPI CHookManager::UnHookNetworkIo(DWORD dwPID)
{
	//先计算偏移
	DWORD dwTarget = GetModuleBase(dwPID,m_szDllPath);
	DWORD dwOffset = (DWORD)m_UnHookNetworkIo - (DWORD)m_hModule;
	return CreateRemoteThread(dwPID,(LPTHREAD_START_ROUTINE)(dwTarget+dwOffset));
}
