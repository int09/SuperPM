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
	VOID OutputDebugString(LPCTSTR szFormat,...);
public:
	LPCTSTR szErrorText;
private:
	void FormatMessage();
	INT m_nLastError;
};





#endif // Common_h__