#include "StdAfx.h"
#include "PerformanceTool.h"
#include <Psapi.h>
#pragma comment(lib,"PSAPI.LIB")


CPerformanceTool::CPerformanceTool()
{
	 m_dbIdleTime = 0 ; 
	 m_dbSystemTime = 0; 
	 m_alldbIdleTime = 0;
	 liOldIdleTime.QuadPart = 0;
	 liOldSystemTime.QuadPart = 0;
	 m_PUT = NULL;
	 m_pSPPI = NULL;
}



DWORD CPerformanceTool::GetCPUNum()
{
	SYSTEM_BASIC_INFORMATION SysBaseInfo;

	NTSTATUS status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (!NT_SUCCESS(status)) 
		return -1;

	return SysBaseInfo.bKeNumberProcessors;
}

BOOL CPerformanceTool::Create()
{
	NtQuerySystemInformation=(NtQuerySystemInformationT)GetProcAddress(
		GetModuleHandle(TEXT("ntdll.dll")),
		"NtQuerySystemInformation"); 
	
	if(NtQuerySystemInformation == NULL)
		return FALSE;
	
	m_nNumberProcessors = GetCPUNum();

	//malloc and point
	m_pSPPI = new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[m_nNumberProcessors];
	m_PUT = new PROCESSORS_USE_TIME[m_nNumberProcessors];

	return TRUE;
}

BOOL CPerformanceTool::GetPageFileUsag(double & dwTotalSize,double & dwUsageSize)
{
	PERFORMANCE_INFORMATION performance = { sizeof(performance) };
	if ( ::GetPerformanceInfo( &performance, sizeof(performance) ) )
	{
		DWORD pageSize = performance.PageSize/1024;
		dwUsageSize = performance.CommitTotal*pageSize/1024.0;  //MB
		dwTotalSize = performance.CommitLimit*pageSize/1024.0;  //MB
		return TRUE;
	}
	return FALSE;
}




BOOL CPerformanceTool::GetCPUUsage(double & dTotal, DWORD *pSingleCpu, DWORD dwSize)
{

	NTSTATUS status;
	// get number of processors in the system 
	status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (!NT_SUCCESS(status)) 
		return FALSE;
 
	// get new system time 
	status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0); 
	if (!NT_SUCCESS(status)) 
		return FALSE;
 
	// get new CPU's idle time 
	status =NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if (!NT_SUCCESS(status)) 
		return FALSE;
	
	// get ProcessorPer time 
	status =NtQuerySystemInformation(SystemProcessorPerformanceInformation, m_pSPPI, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * m_nNumberProcessors, NULL); 
	if (!NT_SUCCESS(status)) 
		return FALSE;

	// if it's a first call - skip it 
	if (liOldIdleTime.QuadPart != 0) 
	{ 
		// CurrentValue = NewValue - OldValue 
		m_dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
 
		m_dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);
 
		// CurrentCpuIdle = IdleTime / SystemTime 
		m_dbIdleTime = m_dbIdleTime / m_dbSystemTime;
 
		// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
		m_dbIdleTime = 100.0 - m_dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;
 
		//calc Processors
		if(pSingleCpu && dwSize)
		for (int i = 0; i < m_nNumberProcessors; i++)
		{
			m_PUT[i].dbCurrentTime = Li2Double(m_pSPPI[i].KernelTime) + Li2Double(m_pSPPI[i].UserTime) + 
										Li2Double(m_pSPPI[i].DpcTime) + Li2Double(m_pSPPI[i].InterruptTime) - m_PUT[i].dbOldCurrentTime;
			m_PUT[i].dbIdleTime = Li2Double(m_pSPPI[i].IdleTime) - m_PUT[i].dbOldIdleTime;
 
			// CurrentCpuIdle = IdleTime / SystemTime 
			m_PUT[i].dbIdleTime = m_PUT[i].dbIdleTime / m_PUT[i].dbCurrentTime;
 
			// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
			m_PUT[i].dbIdleTime = 100.0 - m_PUT[i].dbIdleTime * 100.0 + 0.5;
 
		}
	}

 
	// store new CPU's idle and system time 
	liOldIdleTime = SysPerfInfo.liIdleTime; 
	liOldSystemTime = SysTimeInfo.liKeSystemTime;
 

	if(pSingleCpu && dwSize)
	for (int i = 0; i < m_nNumberProcessors; i++)
	{
		m_PUT[i].dbOldCurrentTime = Li2Double(m_pSPPI[i].KernelTime) + Li2Double(m_pSPPI[i].UserTime) + 
										Li2Double(m_pSPPI[i].DpcTime) + Li2Double(m_pSPPI[i].InterruptTime);
 
		m_PUT[i].dbOldIdleTime = Li2Double(m_pSPPI[i].IdleTime);
	}

	dTotal = m_dbIdleTime;
	return TRUE;
}
