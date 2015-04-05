#include "StdAfx.h"
#include "Hooker.h"
#include <windows.h>



BOOL CCMHooker::HookApi(LPCSTR szApiName,
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

		//��ͳ�ƣ��������е�API ��֧�� hotfix��
		//�������֧��hotfix������ͻ�ʧ��.
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
		m_HookRecord.insert(HookRecordPair(szApiName,lpProcAddress+2/*EB F9*/));
	}
	return bResult;
}

BOOL CCMHooker::UnHookApi(LPCSTR szApiName)
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

LPVOID CCMHooker::GetApiSourceAddress(LPCSTR szApiName)
{
	HookRecord::iterator iter;
	iter = m_HookRecord.find(szApiName);
	if(iter != m_HookRecord.end())
		return iter->second;

	return NULL;
}
