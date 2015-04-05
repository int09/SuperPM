#include "StdAfx.h"
#include "DataManager.h"
#include <strsafe.h>


CDataManager::CDataManager():m_hFile(INVALID_HANDLE_VALUE),m_hMapping(NULL),m_lpAddress(NULL),
m_lpReserveMemory(NULL)
{


}

CDataManager::~CDataManager()
{
	if(m_hMapping!=NULL)
	{
		CloseHandle(m_hMapping);
		m_hMapping=NULL;
	}

	if(m_hFile!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile=NULL;
	}
}



BOOL CDataManager::Create()
{
	TCHAR szTemp[MAX_PATH]={NULL};
	GetTempPath(MAX_PATH,szTemp);
	StringCbCat(szTemp,sizeof(szTemp),MAP_NAME);
	BOOL bRet = FALSE;
	
	do
	{
		m_hFile=CreateFile(szTemp,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

		if(m_hFile == INVALID_HANDLE_VALUE)
			break;

		m_hMapping = CreateFileMapping(m_hFile,NULL,PAGE_READWRITE,0,SHARE_MEMEORY_SIZE,MAP_NAME);
		if(m_hMapping == NULL)
			break;

		m_lpAddress = (LPBYTE)MapViewOfFile(m_hMapping,FILE_MAP_READ|FILE_MAP_WRITE,0,0,SHARE_MEMEORY_SIZE);
		if(m_lpAddress == NULL)
			break;

		bRet = TRUE;

	}while(FALSE);

	if(!bRet)
	{
		DWORD dwErrorCode = GetLastError();

		if(m_hMapping!=NULL)
		{
			CloseHandle(m_hMapping);
			m_hMapping=NULL;
		}

		if(m_hFile!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		m_lpAddress = NULL;
		SetLastError(dwErrorCode);
	}else
	{
		m_lpReserveMemory = (LPRESERVE_INFO)m_lpAddress;
		ZeroMemory(m_lpAddress,SHARE_MEMEORY_SIZE);
		m_lpAddress+=RESERVED_MEMORY;
	}
	return bRet;
}




