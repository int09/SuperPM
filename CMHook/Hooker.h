#ifndef Hooker_h__
#define Hooker_h__



typedef map<LPCSTR,LPBYTE> HookRecord;
typedef pair<LPCSTR,LPBYTE> HookRecordPair;

class CCMHooker
{
public:
	CCMHooker(){}
	virtual ~CCMHooker(){}
	BOOL HookApi(LPCSTR szApiName,LPCTSTR szDllName,LPVOID Detours);
	BOOL UnHookApi(LPCSTR szApiName);
	BOOL UnAllHookApi();
	LPVOID GetApiSourceAddress(LPCSTR szApiName);
private:
	HookRecord m_HookRecord;
};


#endif // Hooker_h__