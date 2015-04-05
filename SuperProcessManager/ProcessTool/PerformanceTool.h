#ifndef PerformanceTool_h__
#define PerformanceTool_h__

#include "WDK.h"

#define SystemPerformanceInformation 2 
#define SystemTimeInformation 3
#define SystemProcessorPerformanceInformation 8

typedef struct _PROCESSORS_USE_TIME 
{
    double dbOldIdleTime;	// save old total time
	double dbOldCurrentTime;
	double dbIdleTime;		// save time after calc
	double dbCurrentTime;
	float fUse;
}PROCESSORS_USE_TIME,*LPPROCESSORS_USE_TIME;

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION 
{
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER DpcTime;
    LARGE_INTEGER InterruptTime;
    ULONG Reserved2;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION ,*LPSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))
 

class CPerformanceTool
{
public:
	BOOL GetPageFileUsag(double & dwTotalSize,double & dwUsageSize);
	BOOL GetCPUUsage(double & dTotal,DWORD *pSingleCpu,DWORD dwSize);
	BOOL Create();
	CPerformanceTool();
	DWORD GetCPUNum();
private:
	typedef DWORD (WINAPI *NtQuerySystemInformationT)(ULONG,PVOID,ULONG,PULONG); 
	NtQuerySystemInformationT NtQuerySystemInformation;
	INT m_nNumberProcessors;

	double m_dbIdleTime ; 
	double m_dbSystemTime; 
	double m_alldbIdleTime;

	LARGE_INTEGER liOldIdleTime; 
	LARGE_INTEGER liOldSystemTime;

	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
	SYSTEM_TIME_INFORMATION SysTimeInfo; 
	SYSTEM_BASIC_INFORMATION SysBaseInfo; 

	LPPROCESSORS_USE_TIME m_PUT;
	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * m_pSPPI;
};



#endif // PerformanceTool_h__