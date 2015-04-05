#ifndef DataManager_h__
#define DataManager_h__

#define  MUTEX_NAME  TEXT("CMHOOK_MUTE_2012-11-30")


class CDataManager
{
public:
	CDataManager();
	virtual ~CDataManager();
	
	void InitHeadData(LPHEAD_DATA lpHeadData);

	BOOL Create();
	BOOL SendData(DWORD dwFormat,
				  LPVOID  lpHeadData,
				  DWORD dwHeadSize,
				  LPVOID lpData,
				  DWORD dwDataSize,
				  WPARAM wParam,
				  LPARAM lParam);


	LPBYTE GetBuffer(DWORD dwHeadSize);
	BOOL   ReleaseBufferAndSend(DWORD dwFormat,
								LPVOID lpHeadData,
								DWORD dwHeadSize,
								WPARAM wParam,
								LPARAM lParam);

private:
	HANDLE m_hMutex;
	HANDLE m_hMapping;
	LPBYTE m_lpBaseAddress;
	LPRESERVE_INFO m_lpReserveMemory;
};



#endif // DataManager_h__