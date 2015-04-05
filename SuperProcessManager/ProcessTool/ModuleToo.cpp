#include "StdAfx.h"
#include "ModuleToo.h"



CModuleTool::CModuleTool()
{
	m_hSnapshot = NULL;
}

CModuleTool::~CModuleTool()
{
	if(m_hSnapshot) CloseHandle(m_hSnapshot);
}




BOOL CModuleTool::CreateSnapshotThread(DWORD dwPID)
{
	m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);
	if(m_hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}

BOOL CModuleTool::Module32First(LPMODULEENTRY32 ModuleData)
{
	ZeroMemory(ModuleData,sizeof(MODULEENTRY32));
	ModuleData->dwSize = sizeof(MODULEENTRY32);
	
	if(!::Module32First(m_hSnapshot,ModuleData))
		return FALSE;

	return TRUE;
}


BOOL CModuleTool::Module32Next(LPMODULEENTRY32 ModuleData)
{
	if(!::Module32Next(m_hSnapshot,ModuleData))
	{
		if(m_hSnapshot) {CloseHandle(m_hSnapshot);m_hSnapshot=NULL;}
		return FALSE;
	}
	return TRUE;
}

