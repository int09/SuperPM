// CMHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "HookNetworkApi.h"
#pragma comment(linker,"/section:ShareData,RWS")

CCMHooker CMHooker;
CDataManager DataMgr;


#define HOOK_RECORD_LEN		1024

#pragma data_seg("ShareData")

HOOK_RECORD g_Record[HOOK_RECORD_LEN]={NULL};

#pragma data_seg()


LPHOOK_RECORD WINAPI SelectHookRecord(DWORD dwPID)
{
	for(INT i = 0; i< HOOK_RECORD_LEN ; i++)
	{
		if(g_Record[i].dwPID == dwPID)
			return &g_Record[i];
	}
	return NULL;
}

BOOL WINAPI InsertHookRecord(LPHOOK_RECORD lpRecord)
{
	//检测下参数
	if(lpRecord->dwPID == 0)
		return FALSE;
	
	for(INT i = 0; i< HOOK_RECORD_LEN ; i++)
	{
		if(g_Record[i].dwPID == 0)
		{
			CopyMemory(&g_Record[i],lpRecord,sizeof(HOOK_RECORD));
			return TRUE;
		}
	}

	return FALSE;
}

BOOL WINAPI UpdateHookRecord(LPHOOK_RECORD lpRecord)
{
	//检测下参数
	if(lpRecord->dwPID == 0)
		return FALSE;

	for(INT i = 0; i< HOOK_RECORD_LEN ; i++)
	{
		if(g_Record[i].dwPID == lpRecord->dwPID)
		{
			CopyMemory(&g_Record[i],lpRecord,sizeof(HOOK_RECORD));
			return TRUE;
		}
	}
	return FALSE;
}

BOOL WINAPI DeleteHookRecord(LPHOOK_RECORD lpRecord)
{
	//检测下参数
	if(lpRecord->dwPID == 0)
		return FALSE;

	for(INT i = 0; i< HOOK_RECORD_LEN ; i++)
	{
		if(g_Record[i].dwPID == lpRecord->dwPID)
		{
			ZeroMemory(&g_Record[i],sizeof(HOOK_RECORD));
			return TRUE;
		}
	}
	return FALSE;
}


DWORD WINAPI HookNetworkIo(LPVOID lpParameter)
{
	OutputDebugString(TEXT("HookNetworkIo"));
	CMHooker.HookApi("getaddrinfo",TEXT("ws2_32.dll"),getaddrinfo_Detours);
	CMHooker.HookApi("gethostbyname",TEXT("ws2_32.dll"),gethostbyname_Detours);
 	CMHooker.HookApi("send",TEXT("WS2_32.DLL"),send_Detours);
 	CMHooker.HookApi("recv",TEXT("WS2_32.DLL"),recv_Detours);
	CMHooker.HookApi("sendto",TEXT("WS2_32.DLL"),sendto_Detours);
 	CMHooker.HookApi("recvfrom",TEXT("WS2_32.DLL"),recvfrom_Detours);
 	CMHooker.HookApi("WSASend",TEXT("WS2_32.DLL"),WSASend_Detours);
 	CMHooker.HookApi("WSARecv",TEXT("WS2_32.DLL"),WSARecv_Detours);
 	CMHooker.HookApi("WSASendTo",TEXT("WS2_32.DLL"),WSASendTo_Detours);
 	CMHooker.HookApi("WSARecvFrom",TEXT("WS2_32.DLL"),WSARecvFrom_Detours);
	return 0;
}

DWORD WINAPI UnHookNetworkIo(LPVOID lpParameter)
{
	OutputDebugString(TEXT("UnHookNetworkIo"));
	CMHooker.UnHookApi("getaddrinfo");
	CMHooker.UnHookApi("gethostbyname");
	CMHooker.UnHookApi("send");
	CMHooker.UnHookApi("recv");
	CMHooker.UnHookApi("sendto");
	CMHooker.UnHookApi("recvfrom");
	CMHooker.UnHookApi("WSASend");
	CMHooker.UnHookApi("WSARecv");
	CMHooker.UnHookApi("WSASendTo");
	CMHooker.UnHookApi("WSARecvFrom");
	return 0;
}



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if(!DataMgr.Create()) FreeLibraryAndExitThread((HMODULE)hModule,0);
		OutputDebugString(TEXT("CM_HOOK_DLL_PROCESS_ATTACH"));
	}
	if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		OutputDebugString(TEXT("CM_HOOK_DLL_PROCESS_DETACH"));
		CMHooker.UnAllHookApi();
	}
    return TRUE;
}
