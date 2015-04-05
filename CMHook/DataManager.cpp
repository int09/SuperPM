#include "StdAfx.h"
#include "DataManager.h"

CDataManager::CDataManager(): m_hMutex(NULL),m_hMapping(NULL),m_lpBaseAddress(NULL)
{


}

CDataManager::~CDataManager()
{
	if(m_hMapping)
		CloseHandle(m_hMapping);
	if(m_hMutex) 
		CloseHandle(m_hMutex);
}



void CDataManager::InitHeadData(LPHEAD_DATA lpHeadData)
{
	lpHeadData->dwPID = GetCurrentProcessId();
	lpHeadData->dwTID = GetCurrentThreadId();
	GetModuleFileName(NULL,lpHeadData->szModule,MAX_PATH);
}



BOOL CDataManager::Create()
{
	m_hMutex = OpenMutex(SYNCHRONIZE,NULL,MUTEX_NAME);

	if(m_hMutex == NULL)
		m_hMutex = CreateMutex(NULL,FALSE,MUTEX_NAME);
	
	if(m_hMutex == NULL)
	{
		CFormatMessage FormatMessage;
		FormatMessage.OutputDebugString(TEXT("同步互斥无法创建，无法进行任何数据发送。"));
		return FALSE;
	}

	m_hMapping = OpenFileMapping(FILE_MAP_WRITE,FALSE,MAP_NAME);	
	if(m_hMapping == NULL)
	{
		CFormatMessage FormatMessage;
		FormatMessage.OutputDebugString(TEXT("打开共享内存失败！将无法发送数据给超级任务管理器"));
		return FALSE;
	}

	m_lpBaseAddress = (LPBYTE)MapViewOfFile(m_hMapping,FILE_MAP_WRITE,0,0,SHARE_MEMEORY_SIZE);
	if(m_lpBaseAddress ==NULL)
	{
		CFormatMessage FormatMessage;
		FormatMessage.OutputDebugString(TEXT("映射共享内存失败！将无法发送数据给超级任务管理器"));
		CloseHandle(m_hMapping);
		return FALSE;
	}

	m_lpReserveMemory = (LPRESERVE_INFO)m_lpBaseAddress;
	m_lpBaseAddress+=RESERVED_MEMORY;
	return TRUE;
}



BOOL CDataManager::SendData(DWORD dwFormat,
			  LPVOID lpHeadData,
			  DWORD dwHeadSize,
			  LPVOID lpData,
			  DWORD dwDataSize,
			  WPARAM wParam,
			  LPARAM lParam
			  )
{
	HWND hWnd;
	UINT uMsg;

	__try
	{
		__try{
			WaitForSingleObject(m_hMutex,-1);
		
			if(dwFormat & FORMAT_NETWORKIO_DATA)
			{
				hWnd = m_lpReserveMemory->hNetworkIoDlg;
				uMsg = m_lpReserveMemory->uNetworkIoDlgMsg;
			}
			if(dwFormat & FORMAT_FILEIO_DATA)
			{
				hWnd = m_lpReserveMemory->hFileIoDlg;
				uMsg = m_lpReserveMemory->uFileIoDlgMsg;
			}
			if(dwFormat & FORMAT_REGISTRYIO_DATA)
			{
				hWnd = m_lpReserveMemory->hRegistryIoDlg;
				uMsg = m_lpReserveMemory->uRegistryIoDlgMsg;
			}
			
			CopyMemory(m_lpBaseAddress,lpHeadData,dwHeadSize);
			
			if(lpData && dwDataSize)
			{
				CopyMemory((m_lpBaseAddress + dwHeadSize),lpData,dwDataSize);
			}

			SendMessage(hWnd,uMsg,wParam,lParam);

		}__finally{
			ReleaseMutex(m_hMutex);
		}
	}__except(1)
	{
		return FALSE;
	}
	return TRUE;
}



LPBYTE CDataManager::GetBuffer(DWORD dwHeadSize)
{
	WaitForSingleObject(m_hMutex,-1);
	return (LPBYTE)(m_lpBaseAddress + dwHeadSize);
}

BOOL CDataManager::ReleaseBufferAndSend(DWORD dwFormat, 
										LPVOID lpHeadData, 
										DWORD dwHeadSize, 
										WPARAM wParam, 
										LPARAM lParam)
{
	HWND hWnd;
	UINT uMsg;
	__try
	{
		__try{

			if(dwFormat & FORMAT_NETWORKIO_DATA)
			{
				hWnd = m_lpReserveMemory->hNetworkIoDlg;
				uMsg = m_lpReserveMemory->uNetworkIoDlgMsg;
			}
			if(dwFormat & FORMAT_FILEIO_DATA)
			{
				hWnd = m_lpReserveMemory->hFileIoDlg;
				uMsg = m_lpReserveMemory->uFileIoDlgMsg;
			}
			if(dwFormat & FORMAT_REGISTRYIO_DATA)
			{
				hWnd = m_lpReserveMemory->hRegistryIoDlg;
				uMsg = m_lpReserveMemory->uRegistryIoDlgMsg;
			}
			
			CopyMemory(m_lpBaseAddress,lpHeadData,dwHeadSize);
			SendMessage(hWnd,uMsg,wParam,lParam);
			
		}__finally{
			ReleaseMutex(m_hMutex);
		}
	}__except(1)
	{
		return FALSE;
	}
	return TRUE;
}
