#ifndef DataManager_h__
#define DataManager_h__

class CDataManager
{
public:
	void GetHeadData(LPVOID lpHeadData,DWORD dwSize){
		CopyMemory(lpHeadData,m_lpAddress,dwSize);
	}
	void GetData(LPBYTE * lpData,DWORD dwOffset){
		*lpData = (LPBYTE) (m_lpAddress+dwOffset);
	}


	LPRESERVE_INFO GetReserveInfo(){ return m_lpReserveMemory;}
	BOOL Create();
	CDataManager();
	~CDataManager();
private:
	HANDLE m_hFile;
	HANDLE m_hMapping;
	LPBYTE m_lpAddress;
	LPRESERVE_INFO m_lpReserveMemory;
};


#endif // DataManager_h__