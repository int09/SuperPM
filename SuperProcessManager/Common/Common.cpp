#include "StdAfx.h"
#include "Common.h"
#include <shlwapi.h>
#include <strsafe.H>

HWND COutputRunlog::m_hWnd=NULL;
UINT COutputRunlog::m_Msg=NULL;

void COutputRunlog::Create(HWND hWnd,UINT nMsg)
{
	m_hWnd = hWnd;
	m_Msg = m_Msg;
}

void  COutputRunlog:: operator()(LPCTSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	TCHAR szParamText[4096]={NULL};
	StringCbVPrintf(szParamText,sizeof(szParamText),szFormat,vagrc);
	va_end(vagrc);
	::SendMessage(m_hWnd,m_Msg,(WPARAM)szParamText,NULL);
}



BOOL EnableDebugPrivilege(LPCTSTR szToken)
{
	
	HANDLE hToken;
	LUID Luid;
	TOKEN_PRIVILEGES tkp;
	
	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return false;
	

	if ( ! LookupPrivilegeValue( NULL, szToken, &Luid ) )
	{
		CloseHandle( hToken );
		return false;
	}
	
	
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = Luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL ) )
		CloseHandle( hToken );
	
	return true;
}

CString GetFileCreationTime(CString szAppName)
{
	WIN32_FIND_DATA ffd ;  
    HANDLE hFind = FindFirstFile(szAppName,&ffd);  
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return CString("NULL");
	}
	SYSTEMTIME stUTC, stLocal;  
    FileTimeToSystemTime(&(ffd.ftCreationTime ),&stUTC);  
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);  
	CString strTmp;  
    strTmp.Format(TEXT("%d-%d-%d,%d:%d"),stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute);      
    FindClose(hFind);
	return strTmp; 
}

CString DosDeviceToFilePath(CString &szTarget)
{
	TCHAR  szPartition[MAX_PATH],szDosDevice[MAX_PATH];
	
	for(TCHAR c='A';c<='Z';c++)
	{	
		ZeroMemory(szPartition,MAX_PATH);
		szPartition[0]=c;
		StringCbCat(szPartition,MAX_PATH,TEXT(":"));
		
		if(QueryDosDevice(szPartition,szDosDevice,MAX_PATH))
		{
			if(0==StrCmpNI((LPCTSTR)szTarget,szDosDevice,_tcslen(szDosDevice)))
			{
				szTarget.Replace(szDosDevice,szPartition);
				break;
			}
		}
	}
	return szTarget;
}

BOOL GetDefaultWebBrowser(LPTSTR szPath,DWORD dwSize)
{

	HKEY hKey;
	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT,TEXT("http\\shell\\open\\command"),
		NULL,KEY_QUERY_VALUE,&hKey))
		return FALSE;


	DWORD dwType = REG_SZ;
	if(ERROR_SUCCESS != RegQueryValueEx(hKey,TEXT(""),NULL,&dwType,(LPBYTE)szPath,&dwSize))
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
	return TRUE;
}

BOOL OpenUrl(LPCTSTR szURL)
{
	TCHAR szWebBrowserPath[MAX_PATH];
	CString szText;

	if(!GetDefaultWebBrowser(szWebBrowserPath,sizeof(szWebBrowserPath)))
	{
		CFormatMessage FormatMessage;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(NULL,TEXT("无法获取默认浏览器地址。"),szText,MB_OK|MB_ICONERROR);
		return FALSE;
	}
	
	CString szCommand = szWebBrowserPath;
	szCommand.Replace(TEXT("%1"),szURL);
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;
	
	if(!CreateProcess(NULL,szCommand.GetBuffer(0),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,
		NULL,NULL,&si,&pi))
	{
		CFormatMessage FormatMessage;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(NULL,TEXT("无法运行浏览器！"),szText,MB_OK|MB_ICONERROR);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return TRUE;
}

/*
#pragma pack (1)
#pragma pack (push)
typedef struct _INJECTCODE
{

	BYTE pushfd;
	BYTE pushad;
	
	struct _LoadLibraryCode
	{
		BYTE push;
		DWORD dwPushAddress;

		BYTE call;
		DWORD dwCallAddress;
	}LoadLibraryCode;
	
	BYTE popad;
	BYTE popfd;

	BYTE JmpEIP;
	DWORD dwEipAddress;
}INJECTCODE,*LPINJECTCODE;
#pragma pack(pop)
*/

#pragma pack (1)
#pragma pack (push)

typedef struct _PUSHCODE
{
	BYTE bCode;
	DWORD dwAddress;
}PUSHCODE,LPPUSHCODE;

typedef struct _CALLCODE
{
	BYTE bCode;
	DWORD dwAddress;
}CALLCODE,LPCALLCODE;


typedef struct _INJECTCODE
{
	PUSHCODE PushDllAddress;
	CALLCODE CallLoadLibrary;

	PUSHCODE PushSleepTime;
	CALLCODE CallSleep;
	
	BYTE bRetnCode[4];
}INJECTCODE,*LPINJECTCODE;
#pragma pack(pop)


/*
BOOL InjectProcess(LPPROCESS_INFORMATION lpProcessInfo,LPCTSTR szDllPath)
{
	
	LPBYTE lpDllPathAddr =NULL;
	LPBYTE lpCodeAddress = NULL;
	CONTEXT ConText;
	BOOL   bRet = FALSE;
	DWORD  dwByteSize = (_tcslen(szDllPath)+1) * sizeof(TCHAR);

	do
	{
		//申请内存
		lpDllPathAddr = (LPBYTE)VirtualAllocEx(lpProcessInfo->hProcess,NULL,dwByteSize,MEM_COMMIT, PAGE_READWRITE); 
		if(lpDllPathAddr == NULL)
			break;

		//写入DLL参数
		if(!WriteProcessMemory(lpProcessInfo->hProcess,lpDllPathAddr,(void*)szDllPath,dwByteSize,NULL))
			break;
		
		lpCodeAddress = (LPBYTE)VirtualAllocEx(lpProcessInfo->hProcess,NULL,sizeof(INJECTCODE),MEM_COMMIT, PAGE_READWRITE); 
		if(lpCodeAddress == NULL)
			break;

		ConText.ContextFlags = CONTEXT_CONTROL;
		if(!GetThreadContext(lpProcessInfo->hThread,&ConText))
			break;

		//构建指令
		INJECTCODE InjectCode;
		InjectCode.pushfd = 0x9C;
		InjectCode.pushad = 0x60;

		//push
		InjectCode.LoadLibraryCode.push = 0x68;
		InjectCode.LoadLibraryCode.dwPushAddress  = (DWORD)lpDllPathAddr;
				
		InjectCode.LoadLibraryCode.call = 0xE8;
		//目标地址-当前地址 - 5
		DWORD dwCallTarget = (DWORD)lpCodeAddress+  (DWORD)&InjectCode.LoadLibraryCode.call - (DWORD)&InjectCode;
		InjectCode.LoadLibraryCode.dwCallAddress = (DWORD)LoadLibrary - dwCallTarget -  5;
		InjectCode.popad = 0x61;
		InjectCode.popfd = 0x9D;

		InjectCode.JmpEIP = 0xE9;
		DWORD dwJmpTarget = (DWORD)lpCodeAddress+  (DWORD)&InjectCode.JmpEIP - (DWORD)&InjectCode;
		InjectCode.dwEipAddress = ConText.Eip - dwJmpTarget - 5;
		if(!WriteProcessMemory(lpProcessInfo->hProcess,lpCodeAddress,&InjectCode,sizeof(INJECTCODE),NULL))
			break;
		
		ConText.ContextFlags = CONTEXT_CONTROL;
		ConText.Eip = (DWORD)lpCodeAddress;
		if(!SetThreadContext(lpProcessInfo->hThread,&ConText))
			break;
		


		bRet = TRUE;

	}while(FALSE);

	return bRet;
}
*/



BOOL InjectProcess(LPPROCESS_INFORMATION lpProcessInfo,LPCTSTR szDllPath,HANDLE & hWaitThread)
{
	LPBYTE lpDllPathAddr =NULL;
	LPBYTE lpCodeAddress = NULL;
	BOOL   bRet = FALSE;
	DWORD  dwByteSize = (_tcslen(szDllPath)+1) * sizeof(TCHAR);
	
	do
	{
		//申请内存
		lpDllPathAddr = (LPBYTE)VirtualAllocEx(lpProcessInfo->hProcess,NULL,dwByteSize,MEM_COMMIT, PAGE_READWRITE); 
		if(lpDllPathAddr == NULL)
			break;
		
		//写入DLL参数
		if(!WriteProcessMemory(lpProcessInfo->hProcess,lpDllPathAddr,(void*)szDllPath,dwByteSize,NULL))
			break;
		
		lpCodeAddress = (LPBYTE)VirtualAllocEx(lpProcessInfo->hProcess,NULL,sizeof(INJECTCODE),MEM_COMMIT,
			PAGE_EXECUTE_READWRITE); 
		
		if(lpCodeAddress == NULL)
			break;
		
		//构建指令
		INJECTCODE InjectCode;
		InjectCode.PushDllAddress.bCode = 0x68;
		InjectCode.PushDllAddress.dwAddress = (DWORD)lpDllPathAddr;
		InjectCode.CallLoadLibrary.bCode = 0xE8;
		InjectCode.CallLoadLibrary.dwAddress = (DWORD)LoadLibrary - \
			((DWORD)lpCodeAddress+  (DWORD)&InjectCode.CallLoadLibrary.bCode - (DWORD)&InjectCode) -  5;

		InjectCode.PushSleepTime.bCode = 0x68;
		InjectCode.PushSleepTime.dwAddress = 10000;
		InjectCode.CallSleep.bCode = 0xE8;
		InjectCode.CallSleep.dwAddress = (DWORD)Sleep - \
			((DWORD)lpCodeAddress+  (DWORD)&InjectCode.CallSleep.bCode - (DWORD)&InjectCode) -  5;

		InjectCode.bRetnCode[0] = 0xC2;
		InjectCode.bRetnCode[1] = 0x04;
		InjectCode.bRetnCode[2] = 0x00;
		InjectCode.bRetnCode[3] = 0xCC;

		if(!WriteProcessMemory(lpProcessInfo->hProcess,lpCodeAddress,&InjectCode,sizeof(INJECTCODE),NULL))
			break;

		hWaitThread = CreateRemoteThread(lpProcessInfo->hProcess,NULL,NULL,(LPTHREAD_START_ROUTINE)lpCodeAddress,NULL,NULL,NULL);
		if(hWaitThread == NULL)
			break;

		bRet = TRUE;
	}while(FALSE);

	return bRet;
}




BOOL InjectProcess(DWORD dwPID,LPCTSTR szDllPath,HANDLE & hWaitThread)
{
	HANDLE hProcess = NULL;
	DWORD dwLastError = 0;
	HANDLE hThread = NULL;
	LPBYTE lpDllPathAddr =NULL;
	BOOL   bRet = FALSE;
	DWORD  dwByteSize = (_tcslen(szDllPath)+1) * sizeof(TCHAR);

	do 
	{
		hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,
			FALSE,dwPID);

		if(hProcess == NULL)
			break;

		lpDllPathAddr = (LPBYTE)VirtualAllocEx(hProcess,NULL,dwByteSize,MEM_COMMIT, PAGE_READWRITE); 
		if(lpDllPathAddr == NULL)
			break;

		if(!WriteProcessMemory(hProcess,lpDllPathAddr,(void*)szDllPath,dwByteSize,NULL))
			break;

		LPTHREAD_START_ROUTINE lsr =(LPTHREAD_START_ROUTINE)LoadLibrary;
		hThread = CreateRemoteThread(hProcess,NULL,NULL,lsr,lpDllPathAddr,NULL,NULL);
		if(hThread == NULL)
			break;

		bRet = TRUE;

	} while (FALSE);

	dwLastError = GetLastError();
	

	/* 算了吧，万一释放了，远程线程还没执行到这里.后果不可想象呀。
	if(lpDllPathAddr) 
	{
		VirtualFreeEx(hProcess,lpDllPathAddr,_tcslen(szDllPath)+1,MEM_DECOMMIT);
	}*/

	if(hProcess)
		CloseHandle(hProcess);
	
	if(hThread && bRet)
		hWaitThread = hThread;

	SetLastError(dwLastError);
	return bRet;
}

double STOF(LPCTSTR s)
{
	double val,power;
	int i,sign;
	
	for(i=0;isspace(s[i]);i++)//跳过空白
		;
	sign=(s[i]=='-')?-1:1;
	
	if(s[i]=='+'||s[i]=='-')
		i++;
	
	for(val=0.0;isdigit(s[i]);i++)//这个是什么意思
		val=10.0*val+(s[i]-'0');
	
	if(s[i]=='.')
		i++;
	
	for(power=1.0;isdigit(s[i]);i++)//这个是什么意思
	{
		val=10.0*val+(s[i]-'0');
		power*=10.0;
	}
	return sign*val/power;
}