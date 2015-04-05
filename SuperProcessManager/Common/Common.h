#ifndef Common_h__
#define Common_h__



class CFormatMessage
{
public:
	CFormatMessage() : m_nLastError(::GetLastError()),szErrorText(NULL){ FormatMessage(); }
	CFormatMessage(INT nLaseError) : m_nLastError(nLaseError),szErrorText(NULL){ FormatMessage();}
	~CFormatMessage(){ if(szErrorText) LocalFree((LPVOID)szErrorText);}	
	DWORD GetLastError(){return m_nLastError;}
	LPCTSTR operator() (){return szErrorText;}
public:
	LPCTSTR szErrorText;
private:
	void FormatMessage()
	{
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			m_nLastError, //接收错误码
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //默认语言
			(LPTSTR)&szErrorText, //接收错误信息
			0,
			NULL);
	}
	INT m_nLastError;
};

class COutputRunlog
{
public: 
	void Create(HWND hWnd,UINT nMsg);
	void operator()(LPCTSTR szFormat,...);
private:
	static HWND m_hWnd;
	static UINT m_Msg;
};

BOOL EnableDebugPrivilege(LPCTSTR szToken);
CString GetFileCreationTime(CString szAppName);
CString DosDeviceToFilePath(CString &szTarget);
BOOL GetDefaultWebBrowser(LPTSTR szPath,DWORD dwSize);
BOOL OpenUrl(LPCTSTR szURL);
BOOL InjectProcess(DWORD dwPID,LPCTSTR szDllPath,HANDLE & hWaitThread);
BOOL InjectProcess(LPPROCESS_INFORMATION lpProcessInfo,LPCTSTR szDllPath,HANDLE & hWaitThread);
double STOF(LPCTSTR s);
#endif // Common_h__