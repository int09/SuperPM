#ifndef ThreadTool_h__
#define ThreadTool_h__

#include "WDK.h"

typedef struct EXTENDTHREADINFO
{
	BOOL  bIsMainThread;
	DWORD dwEntryAddress;
	TCHAR  szThreadModule[MAX_PATH];
	LPCTSTR lpszPrority;
	TCHAR szThreadDesktop[MAX_PATH];
	LPCTSTR lpszThreadStatus;
}EXTENDTHREADINFO,*LPEXTENDTHREADINFO;




class CThreadTool
{
public:
	DWORD GetThreadEntryAddress(DWORD dwTID);
	BOOL SusppedProcess(DWORD dwPID);
	BOOL ResumeProcess(DWORD dwPID);
	virtual ~CThreadTool();
	BOOL Create();
	BOOL GetThreadExtendInfo(LPTHREADINFO Offset,LPEXTENDTHREADINFO pExtendInfo);
	BOOL CreateSnapshotThread(DWORD dwPID);
	BOOL Thread32First(LPTHREADINFO * Offset);
	BOOL Thread32Next(LPTHREADINFO * Offset);
protected:
	
private:
	typedef DWORD (WINAPI *NtQuerySystemInformationT)(ULONG,PVOID,ULONG,PULONG); 
	NtQuerySystemInformationT NtQuerySystemInformation;
	typedef LONG (WINAPI  *ZwQueryInformationThreadT)  (HANDLE,THREADINFOCLASS,PVOID,ULONG,PULONG);
	ZwQueryInformationThreadT ZwQueryInformationThread;


	LPBYTE m_pQueryBuffer;
	ULONG m_QueryBufferSize;

	PSYSTEM_PROCESS_INFORMATION m_pSPI;
	INT m_nIndex;
	INT m_nCount;
};

#endif // ThreadTool_h__