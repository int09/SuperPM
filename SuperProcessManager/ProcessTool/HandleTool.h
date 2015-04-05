#ifndef HandleTool_h__
#define HandleTool_h__
#include "WDK.h"

#define  EXTENDHANDLEINFO_BUFFER_LEN  10240


typedef struct EXTENDHANDLEINFO
{
	HANDLE hHandle;
	ULONG uTypeVaule;
	ULONG AcCount;
	WCHAR szOjectType[EXTENDHANDLEINFO_BUFFER_LEN];
	WCHAR szOjectName[EXTENDHANDLEINFO_BUFFER_LEN];
	BOOL bInherit;
	BOOL bClose;
}EXTENDHANDLEINFO,*LPEXTENDHANDLEINFO;


#define DUPLICATE_CLOSE_SOURCE      0x00000001  
#define DUPLICATE_SAME_ACCESS       0x00000002  
#define DUPLICATE_SAME_ATTRIBUTES   0x00000004


class CHandleTool
{
public:
	BOOL Create();
	BOOL CreateSnapshotHandle(DWORD dwPID);
	BOOL Handle32First(PSYSTEM_HANDLE_INFORMATION * Offset);
	BOOL Handle32Next(PSYSTEM_HANDLE_INFORMATION * Offset);
	BOOL GetHandleExtendInfo(PSYSTEM_HANDLE_INFORMATION Offset,LPEXTENDHANDLEINFO lpExtendInfo);
	virtual ~CHandleTool();
private:
	typedef NTSTATUS (NTAPI *ZwQuerySystemInformationT)(SYSTEM_INFORMATION_CLASS,PVOID,ULONG,PULONG);
	ZwQuerySystemInformationT ZwQuerySystemInformation;
	
	typedef NTSTATUS (NTAPI *ZwQueryObjectT)(HANDLE,OBJECT_INFORMATION_CLASS,PVOID,ULONG,PULONG);
	ZwQueryObjectT ZwQueryObject;

	typedef NTSTATUS (NTAPI *ZwDuplicateObjectT)(HANDLE,HANDLE,HANDLE,PHANDLE,ACCESS_MASK,ULONG,ULONG);
	ZwDuplicateObjectT ZwDuplicateObject;

	ULONG m_nCount;
	ULONG m_nIndex;
	DWORD m_dwPID;
	PSYSTEM_HANDLE_INFORMATION m_pHandleInfo;
private:
	PULONG m_pQuerySystemInfoBuffer;
	ULONG m_QuerySystemInfoBufferSize;
	
	LPBYTE m_pObjectTypeBuffer;
	ULONG m_ObjectTypeBufferSize;

	LPBYTE m_pObjectNameBuffer;
	ULONG m_ObjectNameBufferSize;
};


#endif // HandleTool_h__