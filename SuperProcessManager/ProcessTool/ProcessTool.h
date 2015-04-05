#ifndef ProcessManager_h__
#define ProcessManager_h__

#if _MSC_VER ==1200 
#pragma warning(disable:4786)
#endif

#include "WDK.h"
#include <shlwapi.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;
#include <strsafe.h>



typedef struct _PROCESS_EXTEND_INFO
{
	TCHAR szPath[MAX_PATH];
	TCHAR szParentFileName[MAX_PATH];
	TCHAR szProcessUserName[1024];
	LPCTSTR lpszProcessPrority;
	DWORD  dwVirtualMemeorySize;
	TCHAR szTime[30];
	TCHAR szCommandLine[MAX_PATH];
}PROCESS_EXTEND_INFO,*LPPROCESS_EXTEND_INFO;


#define EXTEND_ALL					(EXTEND_PATH|EXTEND_PARENT_NAME|EXTEND_UERNAME|EXTEND_PRORITY|EXTEND_VM_SIZE|EXTEND_CREATE_TIME|EXTEND_FORMAT_PATH|EXTEND_COMMANLINE)
#define EXTEND_PATH					(0x0001)  
#define EXTEND_PARENT_NAME			(0x0002)  
#define EXTEND_UERNAME			    (0x0004)  
#define EXTEND_PRORITY				(0x0008)  
#define EXTEND_VM_SIZE				(0x0010)  
#define EXTEND_CREATE_TIME          (0x0020)  
#define EXTEND_FORMAT_PATH			(0x0040)  
#define EXTEND_COMMANLINE			(0x0080)
			



enum 
{
	PROCESS_NEW=1, //新进程
	PROCESS_EXIST//进程存在
};


class CProcessTool
{
public:
	DWORD GetProcessCount();
	DWORD GetThreadCount();
	DWORD GetHandleCount();

	void SetCurrentTotalProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset);
	void SetLastTotalProcessCpuTime();
	void SetSingleProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset);
	double GetSingleProcessCpuTime(PSYSTEM_PROCESS_INFORMATION Offset);
	void DeleteProcessTime(DWORD dwPID);
	
	BOOL TerminateProcess(DWORD dwPID,DWORD dwExitCode = 0);
	BOOL SetPriorityClass(DWORD dwPID,DWORD dwFlags);
	BOOL DebugBreakProcess(DWORD dwPID);

	BOOL ProcessExited(DWORD dwPID);
	LPCTSTR GetProcessPriorityClass(HANDLE hProcess);
	void GetExtendInfo(PSYSTEM_PROCESS_INFORMATION  Offset,LPPROCESS_EXTEND_INFO pExtendInfo,INT nMask=EXTEND_ALL);
	BOOL CreateSnapshotProcess();
	BOOL ProcessFirst(PSYSTEM_PROCESS_INFORMATION * Offset);
	BOOL ProcessNext(PSYSTEM_PROCESS_INFORMATION * Offset);
	INT CheckProcess(PSYSTEM_PROCESS_INFORMATION pSPI);
	BOOL Create();
	CProcessTool();
	virtual ~CProcessTool();
protected:
	
private:
	LPBYTE m_pQueryBuffer;
	ULONG m_QueryBufferSize;
	typedef DWORD (WINAPI *NtQuerySystemInformationT)(ULONG,PVOID,ULONG,PULONG); 
	NtQuerySystemInformationT NtQuerySystemInformation;
	typedef map<DWORD , BOOL> PROCESSMARK;

	typedef map<DWORD , LONGLONG> PROCESSCPUTIME;
	typedef pair<DWORD , LONGLONG> PROCESSCPUTIMEPAIR;

	typedef pair<DWORD,BOOL> PROCESSPAIR;
	//typedef map<DWORD ,string> PROUSERINFO;
	//typedef pair<DWORD , string> PROUSERPAIR;
	PROCESSMARK m_Mark;

	typedef BYTE (WINAPI *WinStationGetProcessSidT)(HANDLE,DWORD,FILETIME,PBYTE,PDWORD);
	typedef VOID (WINAPI *CachedGetUserFromSidT)(PSID,PWCHAR,PULONG);
	WinStationGetProcessSidT WinStationGetProcessSid;
	CachedGetUserFromSidT CachedGetUserFromSid;

	PROCESSCPUTIME m_LastCPUTime;
	LARGE_INTEGER m_TotalCpuTime;
	LARGE_INTEGER m_LastTotalCpuTime;
	DWORD m_dwThreadCount,m_dwHandleCount;
};




#endif // ProcessManager_h__