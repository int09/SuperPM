#ifndef Public_h__
#define Public_h__
#include <WINSOCK2.H>
#pragma comment(lib,"WS2_32.LIB")


#define PARTITION_SYMBOL "\r\n----------------��һ�����ݰ�------------------\r\n"


typedef struct _HEAD_DATA
{
	CHAR	szAPIName[100];			//API NAME
	DWORD	dwPID;					//����ID
	DWORD	dwTID;					//�߳�ID
	TCHAR	szModule[MAX_PATH];		//����·��
}HEAD_DATA,*LPHEAD_DATA;

typedef struct _NETWORK_API_DATA
{
	HEAD_DATA HeadData;
	DWORD dwAPIFlags;
	DWORD dwCaller;
	INT  nProtocol;
	SOCKET Socket;					//Socket
	CHAR szIP[30];					//IP��ַ
	DWORD dwPort;					//�˿�
	INT nResult;					//����ֵ
	INT nLastError;					//GetLastError�ķ���ֵ
	INT dwDataSize;					//���ݴ�С
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
	DWORD dwAPIFlags;				//API��־
	TCHAR szParam[2048];			//����
	INT nResult;					//����ֵ
	INT nLastError;					//GetLastError�ķ���ֵ
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


#define MAP_NAME				TEXT("CM_HOOK_DATA_FILEMAPPING")	//�ڴ��ļ�������
#define SHARE_MEMEORY_SIZE		(10 * 1024 *1024)							//�����ڴ��С
#define RESERVED_MEMORY			(2 * 1024 * 1024)							//�����ڴ��Сһ����10M ǰ2M�ڴ汣����


//1M�����ڴ��������Ϣ
typedef struct _RESERVE_INFO		
{
	HWND hNetworkIoDlg;				//����֪ͨ�����д������ɵĴ��ھ��
	UINT uNetworkIoDlgMsg;			//�Զ�����ϢID

	HWND hFileIoDlg;				//����֪ͨ�ļ���д������ɵĴ��ھ��
	UINT uFileIoDlgMsg;				//�Զ�����ϢID

	HWND hRegistryIoDlg;			//����֪ͨע����д������ɵĴ��ھ��
	UINT uRegistryIoDlgMsg;

	HWND hRunLogIoDlg;				//����������־֪ͨ�Ĵ��ھ��
	HWND uRunLogIoDlgMsg;			//�Զ�����ϢID

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


//���ݸ�ʽ
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

