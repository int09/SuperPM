// CMHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

__declspec(dllexport) DWORD WINAPI  ExportProc()
{



	return 0;
}


#pragma warning(disable:4786)
#include <iostream>
#include <map>
using namespace std;


typedef map<LPCTSTR,LPBYTE> HookRecord;
typedef pair<LPCTSTR,LPBYTE> HookRecordPair;


class CCMHooker
{
public:
	CCMHooker(){}
	virtual ~CCMHooker(){}
	BOOL HookApi(LPCTSTR szApiName,LPCTSTR szDllName,LPVOID Detours);
	BOOL UnHookApi(LPCTSTR szApiName);
	BOOL UnAllHookApi();
private:
	HookRecord m_HookRecord;
};


BOOL CCMHooker::HookApi(LPCTSTR szApiName,
					   LPCTSTR szDllName,
					   LPVOID Detours)
{
	

	BOOL	bResult = FALSE;
	HMODULE hModule = NULL;
	LPBYTE   lpProcAddress = NULL;

	do 
	{

		if(NULL == (hModule = LoadLibrary(szDllName)))
			break;

		if (0 == (lpProcAddress = (LPBYTE)GetProcAddress(hModule,szApiName)))
			break;

		HookRecord::const_iterator iter;
		iter = m_HookRecord.find(szApiName);
		if(iter != m_HookRecord.end())
			break;

		//��ͳ�ƣ��������е�API ��֧�� Hot Patching��
		//�������֧��Hot Patching������ͻ�ʧ��.
		BYTE bMagic []={0x90,0x90,0x90,0x90,0x90,0x8B,0xFF};
		BYTE bCheck [sizeof(bMagic)];
		LPBYTE lpStartAddress = lpProcAddress- sizeof(bMagic)+sizeof(BYTE)*2;

		CopyMemory(bCheck,(LPVOID)lpStartAddress,sizeof(bCheck));
		if(0 != memcmp(bMagic,bCheck,sizeof(bCheck)))
			break;

		//ȥ���ڴ汣��,�����дȨ��
		DWORD flOldProtect;
		if(!VirtualProtect((LPVOID)lpProcAddress,sizeof(bMagic),PAGE_EXECUTE_READWRITE,&flOldProtect))
			break;

		//jmp short F9(-5)  ������: EB F9
		*lpProcAddress = 0xEB;
		*(lpProcAddress+1) = 0xF9;

		//jmp long Ŀ���ַ-��ǰ��ַ - 5 ������:E9
		DWORD dwJmpDetoursAddress = (DWORD)Detours - (DWORD)lpStartAddress - 5;
		*lpStartAddress = 0xE9;
		CopyMemory((LPVOID)(lpStartAddress+1),&dwJmpDetoursAddress,sizeof(DWORD));

		//�ָ��ڴ��������
		VirtualProtect((LPVOID)lpProcAddress,sizeof(bMagic),flOldProtect,&flOldProtect);
		
		//����Hook�������
		bResult= TRUE;

	} while (FALSE);

	//��һ����¼
	if(bResult )
	{
		m_HookRecord.insert(HookRecordPair(szApiName,lpProcAddress));
	}
	return bResult;
}


BOOL CCMHooker::UnHookApi(LPCTSTR szApiName)
{
	HookRecord::iterator iter;
	iter = m_HookRecord.find(szApiName);
	if(iter == m_HookRecord.end())
		return FALSE;
	
	DWORD flOldProtect;
	if(!VirtualProtect((LPVOID)iter->second,sizeof(BYTE)*2,PAGE_EXECUTE_READWRITE,&flOldProtect))
		return FALSE;

	LPBYTE lpAddress = iter->second;
	*lpAddress = 0x8B;
	*(lpAddress+1) = 0xFF;
	*(lpAddress - 1) = 0x90;
	*(lpAddress - 2) = 0x90;
	*(lpAddress - 3) = 0x90;
	*(lpAddress - 4) = 0x90;
	*(lpAddress - 5) = 0x90;
	VirtualProtect((LPVOID)iter->second,sizeof(BYTE)*2,flOldProtect,&flOldProtect);

	//ɾ�������¼
	m_HookRecord.erase(iter);
	return TRUE;
}

BOOL CCMHooker::UnAllHookApi()
{
	HookRecord::iterator iter;
	while(TRUE || m_HookRecord.size())
	{
		iter = m_HookRecord.begin();
		if(iter!=m_HookRecord.end())
			UnHookApi(iter->first);
		else
			break;
	}
	return TRUE;
}




CCMHooker CMHooker;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		CMHooker.HookApi("send","WS2_32.DLL",ExportProc);
		CMHooker.HookApi("recv","WS2_32.DLL",ExportProc);
		CMHooker.UnAllHookApi();
	
	}


    return TRUE;
}

