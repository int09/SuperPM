#ifndef HookManager_h__
#define HookManager_h__

class CHookManager
{
public:
	CHookManager();
	virtual ~CHookManager();
	BOOL Create(LPCTSTR szDllPath);
	LPHOOK_RECORD WINAPI SelectHookRecord(DWORD dwPID);
	BOOL WINAPI InsertHookRecord(LPHOOK_RECORD lpRecord);
	BOOL WINAPI UpdateHookRecord(LPHOOK_RECORD lpRecord);
	BOOL WINAPI DeleteHookRecord(LPHOOK_RECORD lpRecord);
	BOOL WINAPI HookNetworkIo(DWORD dwPID);
	BOOL WINAPI UnHookNetworkIo(DWORD dwPID);
	BOOL WINAPI CreateRemoteThread(DWORD dwPID,LPTHREAD_START_ROUTINE lsr,LPVOID lpParameter=NULL);
	DWORD WINAPI GetModuleBase(DWORD dwPID,LPCTSTR szDllPath);
private:
	SelectHookRecordT	m_SelectHookRecord;
	InsertHookRecordT	m_InsertHookRecord;
	UpdateHookRecordT	m_UpdateHookRecord;
	DeleteHookRecordT	m_DeleteHookRecord;
	HookNetworkIoT		m_HookNetworkIo;
	UnHookNetworkIoT	m_UnHookNetworkIo;
	HMODULE  m_hModule;
	TCHAR    m_szDllPath[MAX_PATH];
};


#endif // HookManager_h__