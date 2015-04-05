#include "StdAfx.h"
#include "ProcessTool.h"
#include <Psapi.h>
#pragma comment(lib,"psapi.lib")


CProcessTool::CProcessTool()
{
	m_TotalCpuTime.HighPart = 0;
	m_TotalCpuTime.LowPart = 0;
	m_dwThreadCount = 0;
}



CProcessTool::~CProcessTool()
{
	VirtualFree(m_pQueryBuffer,m_QueryBufferSize,MEM_DECOMMIT);
}


void CProcessTool::SetCurrentTotalProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset)
{
	m_TotalCpuTime.QuadPart=0;
	while(TRUE)
	{

		m_TotalCpuTime.QuadPart +=  Offset->KernelTime.QuadPart + Offset->UserTime.QuadPart;
		if ((Offset)->NextEntryOffset == 0)
			return;
		Offset = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)Offset + (Offset)->NextEntryOffset);
	}
}

void CProcessTool::SetLastTotalProcessCpuTime()
{
	m_LastTotalCpuTime = m_TotalCpuTime;
}


void CProcessTool::SetSingleProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset)
{
	PROCESSCPUTIME::iterator it;
	while(TRUE)
	{

		it = m_LastCPUTime.find(Offset->UniqueProcessId);
		if(it == m_LastCPUTime.end())
			m_LastCPUTime.insert(PROCESSCPUTIMEPAIR(Offset->UniqueProcessId,Offset->KernelTime.QuadPart + Offset->UserTime.QuadPart));
		else
			it->second = Offset->KernelTime.QuadPart + Offset->UserTime.QuadPart;

		if ((Offset)->NextEntryOffset == 0)
			return;

		Offset = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)Offset + (Offset)->NextEntryOffset);
	}

}

double CProcessTool::GetSingleProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset)
{
	PROCESSCPUTIME::iterator it;
	it = m_LastCPUTime.find(Offset->UniqueProcessId);
	if(it == m_LastCPUTime.end())
		return 0;

	LARGE_INTEGER CurrentProcessTime;
	CurrentProcessTime.QuadPart= (Offset->KernelTime.QuadPart + Offset->UserTime.QuadPart);
	CurrentProcessTime.QuadPart -= it->second;
	return (double)CurrentProcessTime.QuadPart / (m_TotalCpuTime.QuadPart-m_LastTotalCpuTime.QuadPart) * 100.0;
}

VOID CProcessTool::DeleteProcessTime(DWORD dwPID)
{
	PROCESSCPUTIME::iterator it;
	it = m_LastCPUTime.find(dwPID);
	if(it == m_LastCPUTime.end())
		return;

	m_LastCPUTime.erase(it);
}



BOOL CProcessTool::Create()
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

	WinStationGetProcessSid = (WinStationGetProcessSidT)GetProcAddress(
		LoadLibrary(TEXT("Winsta.dll")),
		"WinStationGetProcessSid"); 

	CachedGetUserFromSid = (CachedGetUserFromSidT)GetProcAddress(
		LoadLibrary(TEXT("utildll.dll")),
		"CachedGetUserFromSid");

	return TRUE;
}



BOOL CProcessTool::CreateSnapshotProcess()
{
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

	for(PROCESSMARK::iterator iter=m_Mark.begin(); iter!=m_Mark.end(); ++iter)
		iter->second = FALSE;
	 
	return TRUE;
}

INT CProcessTool::CheckProcess(PSYSTEM_PROCESS_INFORMATION pSPI)
{
	INT nResult = PROCESS_NEW;
	PROCESSMARK::iterator iter =m_Mark.find(pSPI->UniqueProcessId);

	if(iter != m_Mark.end())
	{
		nResult = PROCESS_EXIST;
		iter->second = TRUE;
	}else
	{
		m_Mark.insert(PROCESSPAIR(pSPI->UniqueProcessId,TRUE));
	}
	return nResult;
}

BOOL CProcessTool::ProcessExited(DWORD dwPID)
{
	PROCESSMARK::iterator iter =m_Mark.find(dwPID);
	if(iter == m_Mark.end())
		return FALSE;

	if(iter->second ==FALSE)
	{
		m_Mark.erase(iter);
		return TRUE;
	}
	return FALSE;
}



BOOL CProcessTool::ProcessFirst(PSYSTEM_PROCESS_INFORMATION * Offset)
{
	*Offset =(PSYSTEM_PROCESS_INFORMATION)m_pQueryBuffer;
	m_dwHandleCount = (*Offset)->HandleCount;
	m_dwThreadCount = (*Offset)->NumberOfThreads;
	return TRUE;
}


BOOL CProcessTool::ProcessNext(PSYSTEM_PROCESS_INFORMATION * Offset)
{
	if ((*Offset)->NextEntryOffset == 0)
		return FALSE;
	m_dwThreadCount+=(*Offset)->NumberOfThreads;
	m_dwHandleCount += (*Offset)->HandleCount;
	*Offset = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)*Offset + (*Offset)->NextEntryOffset);
	return TRUE;
}

DWORD CProcessTool::GetProcessCount()
{
	return m_Mark.size();
}

DWORD CProcessTool::GetThreadCount()
{
	return m_dwThreadCount;
}
DWORD CProcessTool::GetHandleCount()
{
	return m_dwHandleCount;
}


LPCTSTR CProcessTool::GetProcessPriorityClass(HANDLE hProcess)
{
	LPCTSTR lpszText=NULL;
	switch(::GetPriorityClass(hProcess))
	{
	case BELOW_NORMAL_PRIORITY_CLASS:
		lpszText=TEXT("低于标准");
		break;
	case IDLE_PRIORITY_CLASS:
		lpszText=TEXT("低");
		break;
	case NORMAL_PRIORITY_CLASS:
		lpszText=TEXT("标准");
		break;
	case ABOVE_NORMAL_PRIORITY_CLASS:
		lpszText=TEXT("高于标准");
		break;
	case HIGH_PRIORITY_CLASS:
		lpszText=TEXT("高");
		break;
	case REALTIME_PRIORITY_CLASS:
		lpszText=TEXT("实时");
		break;
	default:
		lpszText=TEXT("暂缺");
	}
	return lpszText;
}



void CProcessTool::GetExtendInfo(PSYSTEM_PROCESS_INFORMATION  pSPI,LPPROCESS_EXTEND_INFO pExtendInfo,INT nMask)
{
	static TCHAR szBuffer[1024]={NULL};
	ZeroMemory(pExtendInfo,sizeof(PROCESS_EXTEND_INFO));
	
	//处理下特殊路径
	if(pSPI->ImageName.Length == 0  && pSPI->UniqueProcessId == 0)
		pSPI->ImageName.Buffer = L"System Idle Process";
	
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,FALSE,pSPI->UniqueProcessId);
	
	//获取进程路径
	if(nMask & EXTEND_PATH && hProcess)
	{
		GetModuleFileNameEx(hProcess,NULL,pExtendInfo->szPath,MAX_PATH);
	}

	//命令行
	if(nMask & EXTEND_COMMANLINE && hProcess)
	{
		DWORD dwAddr = *(DWORD*)((DWORD)GetCommandLine+1); 
		DWORD dwVer = GetVersion(); 
		DWORD dwRead = 0; 
		if(dwVer < 0x80000000) // Windows NT/2000/XP
		{
			// 读取到命令行字符串的存储地址
			if(ReadProcessMemory(hProcess,(LPVOID)dwAddr, &dwAddr, 4, &dwRead )) 
				ReadProcessMemory(hProcess,(LPVOID)dwAddr, pExtendInfo->szCommandLine,MAX_PATH,&dwRead);
		}
	}
	
	//获取进程优先级
	if(nMask & EXTEND_PRORITY)
		pExtendInfo->lpszProcessPrority = GetProcessPriorityClass(hProcess);

	
	//获取进程的虚拟内存使用
	if(nMask & EXTEND_VM_SIZE && hProcess)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		if(GetProcessMemoryInfo(hProcess,&pmc, sizeof(PROCESS_MEMORY_COUNTERS)))
			pExtendInfo->dwVirtualMemeorySize = pmc.PagefileUsage;
	}
	
	//获取父进程路径
	HANDLE hParent=NULL;
	if((nMask & EXTEND_PARENT_NAME) && (hParent=OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,FALSE,
		pSPI->InheritedFromUniqueProcessId)))
	{
		
		GetModuleFileNameEx(hParent,NULL,szBuffer,sizeof(szBuffer));
		StringCbCopy(pExtendInfo->szParentFileName,MAX_PATH,PathFindFileName(szBuffer));
		CloseHandle(hParent);
	}
	
	//获取进程用户名
	if(nMask & EXTEND_UERNAME && WinStationGetProcessSid && WinStationGetProcessSid)
	{
		DWORD dwSize = sizeof(szBuffer)/sizeof(TCHAR);
		FILETIME    ftStartTime;
		CopyMemory(&ftStartTime,&pSPI->CreateTime,sizeof(FILETIME));
		if(WinStationGetProcessSid(NULL, pSPI->UniqueProcessId,ftStartTime,(LPBYTE)szBuffer, &dwSize))
			CachedGetUserFromSid(szBuffer, pExtendInfo->szProcessUserName,&dwSize);
	}
	
	//启动时间
	if(nMask & EXTEND_CREATE_TIME)
	{
		static SYSTEMTIME st;
		ZeroMemory(&st,sizeof(SYSTEMTIME));
		FileTimeToSystemTime((FILETIME *)&pSPI->CreateTime,&st);
		CTime Time(st);
		Time += CTimeSpan( 0, 8, 0, 0 );
		StringCbCopy(pExtendInfo->szTime,30*2,Time.Format(TEXT("%Y-%m-%d %H:%M:%S")));
	}

	//处理下特殊路径
	if(nMask &	EXTEND_FORMAT_PATH)
	{
		if( ( 0==StrCmpI(pSPI->ImageName.Buffer,TEXT("csrss.exe") ) ||
			0==StrCmpI( pSPI->ImageName.Buffer,TEXT("winlogon.exe") ) ) && 
			0==StrCmpNI( pExtendInfo->szPath,TEXT("\\??"), _tcslen(TEXT("\\??"))))
		{
			TCHAR szPath[MAX_PATH]={NULL};
			StringCbCopy(szPath,sizeof(szPath),&pExtendInfo->szPath[sizeof("\\??")]);
			StringCbCopy(pExtendInfo->szPath,MAX_PATH,szPath);
		}
		if(0==StrCmpI(pSPI->ImageName.Buffer,TEXT("smss.exe")) &&0==StrCmpNI(pExtendInfo->szPath,TEXT("\\systemroot"),_tcslen(TEXT("\\systemroot"))))
		{
			GetSystemDirectory(pExtendInfo->szPath,MAX_PATH);
			StringCbCat(pExtendInfo->szPath,MAX_PATH,TEXT("\\smss.exe"));
		}
	}

	CloseHandle(hProcess);
}

BOOL CProcessTool::TerminateProcess(DWORD dwPID,DWORD dwExitCode /* = 0 */)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,dwPID);
	if(hProcess == NULL) return FALSE;
	BOOL bRet = ::TerminateProcess(hProcess,dwExitCode);
	DWORD dwLastError = GetLastError();
	CloseHandle(hProcess);
	SetLastError(dwLastError);
	return bRet;
}

BOOL CProcessTool::SetPriorityClass(DWORD dwPID,DWORD dwFlags)
{
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION,FALSE,dwPID);
	if(hProcess == NULL) return FALSE;
	BOOL bRet = ::SetPriorityClass(hProcess,dwFlags);
	DWORD dwLastError = GetLastError();
	CloseHandle(hProcess);
	SetLastError(dwLastError);
	return bRet;
}

BOOL CProcessTool::DebugBreakProcess(DWORD dwPID)
{
	HKEY hkDebug;
	BOOL bRet = FALSE;
	TCHAR szDebugger[MAX_PATH * 2];
	DWORD cbString = sizeof(szDebugger);
	
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug"),
		0, KEY_READ, &hkDebug))
		return bRet;
	
	if (ERROR_SUCCESS != RegQueryValueEx(hkDebug, TEXT("Debugger"), NULL, 
		NULL, (LPBYTE) szDebugger, &cbString))
	{
		DWORD dwLastError = GetLastError();
		RegCloseKey(hkDebug);
		SetLastError(dwLastError);
		return FALSE;
	}
	
	// Find the first token (which is the debugger exe name/path)
	LPTSTR pszCmdLine = szDebugger;
	if ( *pszCmdLine == TEXT('\"') ) 
	{
		while ( *++pszCmdLine && (*pszCmdLine != TEXT('\"')) )
		{
			NULL;
		}
		if ( *pszCmdLine == TEXT('\"') )
		{
			pszCmdLine++;
		}
	}
	else 
	{
		while (*pszCmdLine > TEXT(' '))
		{
			pszCmdLine++;
		}
	}

	*pszCmdLine = TEXT('\0');   // Don't need the rest of the args, etc

	STARTUPINFO sinfo;
	ZeroMemory(&sinfo,sizeof(STARTUPINFO));
	sinfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pinfo;

	// If the doctor is in, we don't allow the Debug action
	if (lstrlen(szDebugger) && lstrcmpi(szDebugger, TEXT("drwtsn32")) && lstrcmpi(szDebugger, TEXT("drwtsn32.exe")))
	{
		TCHAR szCmdline[MAX_PATH * 2];
		StringCbPrintf(szCmdline,sizeof(szCmdline),TEXT("%s -p %ld"), szDebugger, dwPID);
		bRet = CreateProcess(NULL,szCmdline,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&sinfo,&pinfo);
	}
	
	if(bRet)
	{
		CloseHandle(pinfo.hThread);
        CloseHandle(pinfo.hProcess);
	}
	RegCloseKey(hkDebug);
	return bRet;
}



