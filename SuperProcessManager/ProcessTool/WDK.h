#ifndef WDK_h__
#define WDK_h__



#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004)
typedef LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

typedef LONG KPRIORITY;

typedef enum _SYSTEM_INFORMATION_CLASS 
{
		SystemBasicInformation,              // 0        Y        N
		SystemProcessorInformation,          // 1        Y        N
		SystemPerformanceInformation,        // 2        Y        N
		SystemTimeOfDayInformation,          // 3        Y        N
		SystemNotImplemented1,               // 4        Y        N
		SystemProcessesAndThreadsInformation, // 5       Y        N
		SystemCallCounts,                    // 6        Y        N
		SystemConfigurationInformation,      // 7        Y        N
		SystemProcessorTimes,                // 8        Y        N
		SystemGlobalFlag,                    // 9        Y        Y
		SystemNotImplemented2,               // 10       Y        N
		SystemModuleInformation,             // 11       Y        N
		SystemLockInformation,               // 12       Y        N
		SystemNotImplemented3,               // 13       Y        N
		SystemNotImplemented4,               // 14       Y        N
		SystemNotImplemented5,               // 15       Y        N
		SystemHandleInformation,             // 16       Y        N
		SystemObjectInformation,             // 17       Y        N
		SystemPagefileInformation,           // 18       Y        N
		SystemInstructionEmulationCounts,    // 19       Y        N
		SystemInvalidInfoClass1,             // 20
		SystemCacheInformation,              // 21       Y        Y
		SystemPoolTagInformation,            // 22       Y        N
		SystemProcessorStatistics,           // 23       Y        N
		SystemDpcInformation,                // 24       Y        Y
		SystemNotImplemented6,               // 25       Y        N
		SystemLoadImage,                     // 26       N        Y
		SystemUnloadImage,                   // 27       N        Y
		SystemTimeAdjustment,                // 28       Y        Y
		SystemNotImplemented7,               // 29       Y        N
		SystemNotImplemented8,               // 30       Y        N
		SystemNotImplemented9,               // 31       Y        N
		SystemCrashDumpInformation,          // 32       Y        N
		SystemExceptionInformation,          // 33       Y        N
		SystemCrashDumpStateInformation,     // 34       Y        Y/N
		SystemKernelDebuggerInformation,     // 35       Y        N
		SystemContextSwitchInformation,      // 36       Y        N
		SystemRegistryQuotaInformation,      // 37       Y        Y
		SystemLoadAndCallImage,              // 38       N        Y
		SystemPrioritySeparation,            // 39       N        Y
		SystemNotImplemented10,              // 40       Y        N
		SystemNotImplemented11,              // 41       Y        N
		SystemInvalidInfoClass2,             // 42
		SystemInvalidInfoClass3,             // 43
		SystemTimeZoneInformation,           // 44       Y        N
		SystemLookasideInformation,          // 45       Y        N
		SystemSetTimeSlipEvent,              // 46       N        Y
		SystemCreateSession,                 // 47       N        Y
		SystemDeleteSession,                 // 48       N        Y
		SystemInvalidInfoClass4,             // 49
		SystemRangeStartInformation,         // 50       Y        N
		SystemVerifierInformation,           // 51       Y        Y
		SystemAddVerifier,                   // 52       N        Y
		SystemSessionProcessesInformation    // 53       Y        N
} SYSTEM_INFORMATION_CLASS;


typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaxLength;
	PWSTR Buffer;
} UNICODE_STRING;


typedef struct _CLIENT_ID   
{   
	LONG   UniqueProcess;   
	LONG   UniqueThread;   
} CLIENT_ID,*LPCLIENT_ID;



typedef struct _THREAD_INFO
{
	LARGE_INTEGER   KernelTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   CreateTime;
    ULONG            WaitTime;
    PVOID            StartAddress;
    CLIENT_ID        ClientId;
    KPRIORITY        Priority;
    KPRIORITY        BasePriority;
    ULONG            ContextSwitchCount;
    LONG            State;
    LONG            WaitReason;
}THREADINFO, *LPTHREADINFO;


typedef   enum   _THREADINFOCLASS   {   
	ThreadBasicInformation,   
		ThreadTimes,   
		ThreadPriority,   
		ThreadBasePriority,   
		ThreadAffinityMask,   
		ThreadImpersonationToken,   
		ThreadDescriptorTableEntry,   
		ThreadEnableAlignmentFaultFixup,   
		ThreadEventPair_Reusable,   
		ThreadQuerySetWin32StartAddress,   
		ThreadZeroTlsCell,   
		ThreadPerformanceCount,   
		ThreadAmILastThread,   
		ThreadIdealProcessor,   
		ThreadPriorityBoost,   
		ThreadSetTlsArrayAddress,   
		ThreadIsIoPending,   
		ThreadHideFromDebugger,   
		ThreadBreakOnTermination,   
		MaxThreadInfoClass   
}THREADINFOCLASS; 

typedef enum _THREAD_STATE
{
		StateInitialized,
		StateReady,
		StateRunning,
		StateStandby,
		StateTerminated,
		StateWait,
		StateTransition,
		StateUnknown
}THREAD_STATE;

typedef enum _KWAIT_REASON
{
		Executive,
		FreePage,
		PageIn,
		PoolAllocation,
		DelayExecution,
		Suspended,
		UserRequest,
		WrExecutive,
		WrFreePage,
		WrPageIn,
		WrPoolAllocation,
		WrDelayExecution,
		WrSuspended,
		WrUserRequest,
		WrEventPair,
		WrQueue,
		WrLpcReceive,
		WrLpcReply,
		WrVirtualMemory,
		WrPageOut,
		WrRendezvous,
		Spare2,
		Spare3,
		Spare4,
		Spare5,
		Spare6,
		WrKernel,
		MaximumWaitReason
}KWAIT_REASON;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;		//线程数量
    LARGE_INTEGER SpareLi1;
    LARGE_INTEGER SpareLi2;
    LARGE_INTEGER SpareLi3;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    DWORD BasePriority;
    DWORD UniqueProcessId;
    DWORD InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG PageDirectoryFrame;
	
    ULONG PeakVirtualSize;
    ULONG VirtualSize;
    ULONG PageFaultCount;
    ULONG PeakWorkingSetSize;
    ULONG WorkingSetSize;
    ULONG QuotaPeakPagedPoolUsage;
    ULONG QuotaPagedPoolUsage;
    ULONG QuotaPeakNonPagedPoolUsage;
    ULONG QuotaNonPagedPoolUsage;
    ULONG PagefileUsage;
    ULONG PeakPagefileUsage;
    ULONG PrivateUsage;
	
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    THREADINFO TH[1];	//线程链表
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;


typedef struct _SYSTEM_BASIC_INFORMATION
{ 
	DWORD dwUnknown1; 
	ULONG uKeMaximumIncrement; 
	ULONG uPageSize; 
	ULONG uMmNumberOfPhysicalPages; 
	ULONG uMmLowestPhysicalPage; 
	ULONG uMmHighestPhysicalPage; 
	ULONG uAllocationGranularity; 
	PVOID pLowestUserAddress; 
	PVOID pMmHighestUserAddress; 
	ULONG uKeActiveProcessors; 
	BYTE bKeNumberProcessors; 
	BYTE bUnknown2; 
	WORD wUnknown3; 
} SYSTEM_BASIC_INFORMATION,*LPSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION
{ 
	LARGE_INTEGER liIdleTime; 
	DWORD dwSpare[76]; 
} SYSTEM_PERFORMANCE_INFORMATION,*LPSYSTEM_PERFORMANCE_INFORMATION;
 
typedef struct _SYSTEM_TIME_INFORMATION
{ 
	LARGE_INTEGER liKeBootTime; 
	LARGE_INTEGER liKeSystemTime; 
	LARGE_INTEGER liExpTimeZoneBias; 
	ULONG uCurrentTimeZoneId; 
	DWORD dwReserved; 
} SYSTEM_TIME_INFORMATION,*LPSYSTEM_TIME_INFORMATION;
 


typedef enum _OBJECT_INFORMATION_CLASS {
		ObjectBasicInformation,
		ObjectNameInformation,
		ObjectTypeInformation,
		ObjectAllInformation,
		ObjectDataInformation
 }OBJECT_INFORMATION_CLASS;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG            ProcessId;
	UCHAR            ObjectTypeNumber;
	UCHAR            Flags;
	USHORT            Handle;
	PVOID            Object;
	ACCESS_MASK        GrantedAccess;
 } SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

typedef struct _PUBLIC_OBJECT_BASIC_INFORMATION {
    ULONG Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG HandleCount;
    ULONG PointerCount;
    ULONG Reserved[10];    // reserved for internal use
} PUBLIC_OBJECT_BASIC_INFORMATION, *PPUBLIC_OBJECT_BASIC_INFORMATION;

#endif // WDK_h__