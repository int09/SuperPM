#ifndef Public_h__
#define Public_h__
#include <WINSOCK2.H>
#pragma comment(lib,"WS2_32.LIB")


#define PARTITION_SYMBOL "\r\n----------------下一个数据包------------------\r\n"


typedef struct _HEAD_DATA
{
	CHAR	szAPIName[100];			//API NAME
	DWORD	dwPID;					//进程ID
	DWORD	dwTID;					//线程ID
	TCHAR	szModule[MAX_PATH];		//进程路径
}HEAD_DATA,*LPHEAD_DATA;

typedef struct _NETWORK_API_DATA
{
	HEAD_DATA HeadData;
	DWORD dwAPIFlags;
	DWORD dwCaller;
	INT  nProtocol;
	SOCKET Socket;					//Socket
	CHAR szIP[30];					//IP地址
	DWORD dwPort;					//端口
	INT nResult;					//返回值
	INT nLastError;					//GetLastError的返回值
	INT dwDataSize;					//数据大小
}NETWORK_API_DATA,*LPNETWORK_API_DATA;

typedef NETWORK_API_DATA SEND_API_DATA;
typedef NETWORK_API_DATA RECV_API_DATA;
typedef NETWORK_API_DATA SENDTO_API_DATA;
typedef NETWORK_API_DATA RECVFROM_API_DATA;
typedef NETWORK_API_DATA WSASEND_API_DATA;
typedef NETWORK_API_DATA WSARECV_API_DATA;
typedef NETWORK_API_DATA WSASENDTO_API_DATA;
typedef NETWORK_API_DATA WSARECVFROM_API_DATA;

enum {NETWORK_NORMAL_DATA,NETWORK_DNS_DATA};
#define HOOK_SEND_API			0x2
#define HOOK_RECV_API			0x4
#define HOOK_SENDTO_API			0x8
#define HOOK_RECVFROM_API		0x10
#define HOOK_WSASEND_API		0x20
#define HOOK_WSARECV_API		0x40
#define HOOK_WSASENDTO_API		0x80
#define HOOK_WSARECVFROM_API	0x100


typedef struct _FILEIO_API_DATA
{
	HEAD_DATA HeadData;
	DWORD dwAPIFlags;				//API标志
	TCHAR szParam[2048];			//参数
	INT nResult;					//返回值
	INT nLastError;					//GetLastError的返回值
}FILEIO_API_DATA,*LPFILEIO_API_DATA;

#define HOOK_CREATEFILEW_API			0x2




typedef struct _NETWORK_DNSDATA
{
	CHAR  szDNS[1000];
	CHAR  szIP[100];
}NETWORK_DNSDATA,*LPNETWORK_DNSDATA;


enum{NETWORK_PROTOCOL_TCP,NETWORK_PROTOCOL_UDP};
#define  NETWORK_PROTOCOL_TCP_TEXT		TEXT("TCP")
#define  NETWORK_PROTOCOL_UDP_TEXT		TEXT("UDP")


#define MAP_NAME				TEXT("CM_HOOK_DATA_FILEMAPPING")	//内存文件共享名
#define SHARE_MEMEORY_SIZE		(10 * 1024 *1024)							//共享内存大小
#define RESERVED_MEMORY			(2 * 1024 * 1024)							//共享内存大小一共是10M 前2M内存保留。


//1M保留内存的数据信息
typedef struct _RESERVE_INFO		
{
	HWND hNetworkIoDlg;				//接收通知网络读写数据完成的窗口句柄
	UINT uNetworkIoDlgMsg;			//自定义消息ID

	HWND hFileIoDlg;				//接收通知文件读写数据完成的窗口句柄
	UINT uFileIoDlgMsg;				//自定义消息ID

	HWND hRegistryIoDlg;			//接收通知注册表读写数据完成的窗口句柄
	UINT uRegistryIoDlgMsg;

	HWND hRunLogIoDlg;				//接收运行日志通知的窗口句柄
	HWND uRunLogIoDlgMsg;			//自定义消息ID

	DWORD bReserve[100];
}RESERVE_INFO,*LPRESERVE_INFO;


typedef struct _TARGET_PROCESS_HOOK_INFO
{
	DWORD dwPID;
	bool bHookNetworkIO;
	bool bHookRegIo;
	bool bHookFileIo;
	bool bReserve[100];
}TARGET_PROCESS_HOOK_INFO,*LPTARGET_PROCESS_HOOK_INFO;


//数据格式
#define  FORMAT_NETWORKIO_DATA			0x2
#define  FORMAT_FILEIO_DATA				0x4
#define  FORMAT_REGISTRYIO_DATA			0x8
#define  FORMAT_RUNLOG_DATA				0x10


typedef struct _HOOK_RECORD
{
	DWORD dwPID;
	BOOL bNetworkIoApiHook;
	BOOL bFileIoApiHook;
	BOOL bRegIoApiHook;
}HOOK_RECORD,*LPHOOK_RECORD;

typedef LPHOOK_RECORD(WINAPI *SelectHookRecordT)(DWORD dwPID);
typedef BOOL(WINAPI *InsertHookRecordT)(LPHOOK_RECORD lpRecord);
typedef BOOL(WINAPI *UpdateHookRecordT)(LPHOOK_RECORD lpRecord);
typedef BOOL(WINAPI *DeleteHookRecordT)(LPHOOK_RECORD lpRecord);
typedef DWORD(WINAPI *UnHookNetworkIoT)(LPVOID lpParameter);
typedef DWORD(WINAPI *HookNetworkIoT)(LPVOID lpParameter);

#endif // Public_h__

