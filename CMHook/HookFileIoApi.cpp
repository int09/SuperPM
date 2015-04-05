#include "StdAfx.h"
#include "HookFileIoApi.h"

extern CCMHooker CMHooker;
extern CDataManager DataMgr;


HANDLE
WINAPI
CreateFileW_Detours(
    __in     LPCWSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
    )
{

	typedef HANDLE(WINAPI * CreateFileWT)(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
	CreateFileWT lpCreateFileW = (CreateFileWT)CMHooker.GetApiSourceAddress("CreateFileW");
	if(lpCreateFileW == NULL) return INVALID_HANDLE_VALUE;
	
	HANDLE hResult = lpCreateFileW(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,
		dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);

	FILEIO_API_DATA FileIoData;
	ZeroMemory(&FileIoData,sizeof(FILEIO_API_DATA));







	return hResult;
}