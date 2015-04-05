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

		//经统计，几乎所有的API 都支持 hotfix。
		//这里假设支持hotfix，否则就会失败.
		BYTE bMagic []={0x90,0x90,0x90,0x90,0x90,0x8B,0xFF};
		BYTE bCheck [sizeof(bMagic)];
		LPBYTE lpStartAddress = lpProcAddress- sizeof(bMagic)+sizeof(BYTE)*2;

		CopyMemory(bCheck,(LPVOID)lpStartAddress,sizeof(bCheck));
		if(0 != memcmp(bMagic,bCheck,sizeof(bCheck)))
			break;

		//去掉内存保护,申请读写权限
		DWORD flOldProtect;
		if(!VirtualProtect((LPVOID)lpProcAddress,sizeof(bMagic),PAGE_EXECUTE_READWRITE,&flOldProtect))
			break;

		//jmp short F9(-5)  机器码: EB F9
		*lpProcAddress = 0xEB;
		*(lpProcAddress+1) = 0xF9;

		//jmp long 目标地址-当前地址 - 5 机器码:E9
		DWORD dwJmpDetoursAddress = (DWORD)Detours - (DWORD)lpStartAddress - 5;
		*lpStartAddress = 0xE9;
		CopyMemory((LPVOID)(lpStartAddress+1),&dwJmpDetoursAddress,sizeof(DWORD));

		//恢复内存访问属性
		VirtualProtect((LPVOID)lpProcAddress,sizeof(bMagic),flOldProtect,&flOldProtect);
		
		//整个Hook过程完成
		bResult= TRUE;

	} while (FALSE);

	//做一个记录
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

	//删除这个记录
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
