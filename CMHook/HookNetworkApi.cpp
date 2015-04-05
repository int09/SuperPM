#include "StdAfx.h"
#include "HookNetworkApi.h"

extern CCMHooker CMHooker;
extern CDataManager DataMgr;



int WSAAPI send_Detours(SOCKET s,const char FAR * buf,int len,int flags)
{

	//�Ƚ����ݷ��ͳ�ȥ��
	typedef int (WSAAPI *sendT)(SOCKET,const char FAR *,int,int);
	sendT lpsend;
	lpsend = (sendT)CMHooker.GetApiSourceAddress("send");
	if(lpsend == NULL) return SOCKET_ERROR;

	int nResualt = lpsend(s,buf,len,flags);

	SEND_API_DATA sendData;
	ZeroMemory(&sendData,sizeof(SEND_API_DATA));

	//���������Ϣ�����淵��ֵ
	sendData.nLastError=WSAGetLastError();
	sendData.nResult = nResualt;
	sendData.Socket = s;
	sendData.nProtocol = NETWORK_PROTOCOL_TCP;
	sendData.dwAPIFlags = HOOK_SEND_API;


	//��ȡIP�Ͷ˿�
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(s,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	StringCbCopyA(sendData.szIP,sizeof(sendData.szIP),inet_ntoa(sockAddr.sin_addr));
	sendData.dwPort = ntohs(sockAddr.sin_port);
	
	//�������ݳ���
	sendData.dwDataSize = len;

	//��ʼ��ͷ��
	StringCbCopyA(sendData.HeadData.szAPIName,sizeof(sendData.HeadData.szAPIName),"send");
	DataMgr.InitHeadData(&sendData.HeadData);

	//��������
	DataMgr.SendData(FORMAT_NETWORKIO_DATA,&sendData,sizeof(SEND_API_DATA),
		(LPVOID)buf,len,NETWORK_NORMAL_DATA,NULL);

	//���ô�����Ϣ
	WSASetLastError(sendData.nLastError);


	return nResualt;
}


int WSAAPI recv_Detours(SOCKET s, char FAR * buf,int len,int flags)
{
	
	
	typedef int (WSAAPI *recvT)(SOCKET,char FAR *,int,int);
	recvT lprecv;
	lprecv = (recvT)CMHooker.GetApiSourceAddress("recv");
	if(lprecv == NULL) return SOCKET_ERROR;

	//�Ƚ������ݳ�ȥ��
	int nResualt = lprecv(s,buf,len,flags);
	
	RECV_API_DATA recvData;
	ZeroMemory(&recvData,sizeof(RECV_API_DATA));

	//���������Ϣ
	recvData.nLastError = WSAGetLastError();
	recvData.nResult = nResualt;
	recvData.Socket = s;
	recvData.nProtocol = NETWORK_PROTOCOL_TCP;
	recvData.dwAPIFlags = HOOK_RECV_API;

	//��ȡIP�Ͷ˿�
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(s,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	StringCbCopyA(recvData.szIP,sizeof(recvData.szIP),inet_ntoa(sockAddr.sin_addr));
	recvData.dwPort = ntohs(sockAddr.sin_port);

	//�������ݳ���
	recvData.dwDataSize = nResualt;

	//��ʼ��ͷ��
	StringCbCopyA(recvData.HeadData.szAPIName,sizeof(recvData.HeadData.szAPIName),"recv");
	DataMgr.InitHeadData(&recvData.HeadData);

	//��������
	DataMgr.SendData(FORMAT_NETWORKIO_DATA,&recvData,sizeof(RECV_API_DATA),
		(LPVOID)buf,len,NETWORK_NORMAL_DATA,NULL);


	//���ô�����Ϣ
	WSASetLastError(recvData.nLastError);
	return nResualt;
}


int WSAAPI sendto_Detours(SOCKET s,const char* buf,int len,int flags,const sockaddr* to,int tolen)
{

	typedef int (WSAAPI *sendtoT)(SOCKET,const char FAR *,int,int,const struct sockaddr FAR *,int);
	sendtoT lpsendto;
	lpsendto = (sendtoT)CMHooker.GetApiSourceAddress("sendto");
	
	if(lpsendto == NULL) return SOCKET_ERROR;

	//�Ƚ����ݷ��ͳ�ȥ��
	int nResualt = lpsendto(s,buf,len,flags,to,tolen);

	SENDTO_API_DATA sendtoData;
	ZeroMemory(&sendtoData,sizeof(SENDTO_API_DATA));

	//���������Ϣ
	sendtoData.nLastError = WSAGetLastError();
	sendtoData.nResult = nResualt;
	sendtoData.Socket = s;
	sendtoData.nProtocol = NETWORK_PROTOCOL_UDP;
	sendtoData.dwAPIFlags = HOOK_SENDTO_API;

	//��ȡIP�Ͷ˿�
	SOCKADDR_IN *lpAddr;
	lpAddr = (SOCKADDR_IN *)to;
	StringCbCopyA(sendtoData.szIP,sizeof(sendtoData.szIP),inet_ntoa(lpAddr->sin_addr));
	sendtoData.dwPort = ntohs(lpAddr->sin_port);
	
	//�������ݳ���
	sendtoData.dwDataSize = len;

	//��ʼ��ͷ��
	StringCbCopyA(sendtoData.HeadData.szAPIName,sizeof(sendtoData.HeadData.szAPIName),"sendto");
	DataMgr.InitHeadData(&sendtoData.HeadData);

	//��������
	DataMgr.SendData(FORMAT_NETWORKIO_DATA,&sendtoData,sizeof(SENDTO_API_DATA),
		(LPVOID)buf,len,NETWORK_NORMAL_DATA,NULL);
	
	WSASetLastError(sendtoData.nLastError);
	return nResualt;
}



int WSAAPI recvfrom_Detours(SOCKET s,const char* buf,int len,int flags,const sockaddr* to,int tolen)
{

	typedef int (WSAAPI *recvfromT)(SOCKET,const char FAR *,int,int,const struct sockaddr FAR *,int);
	recvfromT lprecvfrom;
	lprecvfrom = (recvfromT)CMHooker.GetApiSourceAddress("recvfrom");
	
	if(lprecvfrom == NULL) return SOCKET_ERROR;

	//�Ƚ�������
	int nResualt = lprecvfrom(s,buf,len,flags,to,tolen);


	RECVFROM_API_DATA recvfromData;
	ZeroMemory(&recvfromData,sizeof(RECVFROM_API_DATA));

	//���淵��ֵ���ʹ�����롣
	recvfromData.nLastError = WSAGetLastError();
	recvfromData.nResult = nResualt;
	recvfromData.Socket = s;
	recvfromData.nProtocol = NETWORK_PROTOCOL_UDP;
	recvfromData.dwAPIFlags = HOOK_RECVFROM_API;

	//��ȡIP�Ͷ˿�
	SOCKADDR_IN *lpAddr;
	lpAddr = (SOCKADDR_IN *)to;
	StringCbCopyA(recvfromData.szIP,sizeof(recvfromData.szIP),inet_ntoa(lpAddr->sin_addr));
	recvfromData.dwPort = ntohs(lpAddr->sin_port);

	//�������ݳ���
	recvfromData.dwDataSize = len;

	//��ʼ��ͷ��
	StringCbCopyA(recvfromData.HeadData.szAPIName,sizeof(recvfromData.HeadData.szAPIName),"recvfrom");
	DataMgr.InitHeadData(&recvfromData.HeadData);

	//��������
	DataMgr.SendData(FORMAT_NETWORKIO_DATA,&recvfromData,sizeof(RECVFROM_API_DATA),
		(LPVOID)buf,len,NETWORK_NORMAL_DATA,NULL);

	WSASetLastError(recvfromData.nLastError);
	return nResualt;
}


int WSAAPI WSASend_Detours(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesSent,
							 DWORD dwFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
							 )
{
	typedef int (WSAAPI *WSASendT)(SOCKET,LPWSABUF,DWORD,LPDWORD,DWORD,LPWSAOVERLAPPED,LPWSAOVERLAPPED_COMPLETION_ROUTINE);
	WSASendT lpWSASend;
	lpWSASend = (WSASendT)CMHooker.GetApiSourceAddress("WSASend");

	if(lpWSASend == NULL) return SOCKET_ERROR;

	//�Ƚ����ݷ��ͳ�ȥ��
	int nResualt = lpWSASend(s,lpBuffers,dwBufferCount,lpNumberOfBytesSent,dwFlags,lpOverlapped,lpCompletionRoutine);
	
	WSASEND_API_DATA WSASendData;
	ZeroMemory(&WSASendData,sizeof(WSASEND_API_DATA));

	//���淵��ֵ���ʹ�����Ϣ
	WSASendData.nLastError = WSAGetLastError();
	WSASendData.nResult = nResualt;
	WSASendData.Socket = s;
	WSASendData.nProtocol = NETWORK_PROTOCOL_TCP;
	WSASendData.dwAPIFlags = HOOK_WSASEND_API;

	//��ȡĿ��IP�Ͷ˿�
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(s,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	
	StringCbCopyA(WSASendData.szIP,sizeof(WSASendData.szIP),inet_ntoa(sockAddr.sin_addr));
	WSASendData.dwPort = ntohs(sockAddr.sin_port);
	
	//��ʼ��ͷ��
	StringCbCopyA(WSASendData.HeadData.szAPIName,sizeof(WSASendData.HeadData.szAPIName),"WSASend");
	DataMgr.InitHeadData(&WSASendData.HeadData);


	//������Ҫ�����⴦��
	LPBYTE lpBuffer = DataMgr.GetBuffer(sizeof(WSASEND_API_DATA));
	for (DWORD i=0;i<dwBufferCount;i++)
	{
		CopyMemory(&lpBuffer[WSASendData.dwDataSize],lpBuffers[i].buf,lpBuffers[i].len);
		WSASendData.dwDataSize+=lpBuffers[i].len;

		if(i + 1 !=dwBufferCount)
		{
			CopyMemory(&lpBuffer[WSASendData.dwDataSize],PARTITION_SYMBOL,strlen(PARTITION_SYMBOL));
			WSASendData.dwDataSize+=strlen(PARTITION_SYMBOL);
		}
	}

	//��������
	DataMgr.ReleaseBufferAndSend(FORMAT_NETWORKIO_DATA,
		&WSASendData,sizeof(WSASEND_API_DATA),NETWORK_NORMAL_DATA,NULL);

	WSASetLastError(WSASendData.nLastError);
	return nResualt;
}


int WSAAPI WSARecv_Detours(IN SOCKET s,
							 IN OUT LPWSABUF lpBuffers,
							 IN DWORD dwBufferCount,
							 OUT LPDWORD lpNumberOfBytesRecvd,
							 IN OUT LPDWORD lpFlags,
							 IN LPWSAOVERLAPPED lpOverlapped,
							 IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{

	typedef int (WSAAPI *WSARecvT)(SOCKET,LPWSABUF,DWORD,LPDWORD,LPDWORD,
		LPWSAOVERLAPPED,LPWSAOVERLAPPED_COMPLETION_ROUTINE);
	
	//�Ƚ�������
	WSARecvT lpWSARecv;
	lpWSARecv = (WSARecvT)CMHooker.GetApiSourceAddress("WSARecv");
	if(lpWSARecv == NULL) return SOCKET_ERROR;

	int nResualt = lpWSARecv(s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpOverlapped,lpCompletionRoutine);
	
	WSARECV_API_DATA WSARecvData;
	ZeroMemory(&WSARecvData,sizeof(WSARECV_API_DATA));

	//���淵��ֵ���ʹ�����Ϣ
	WSARecvData.nLastError = WSAGetLastError();
	WSARecvData.nResult = nResualt;
	WSARecvData.Socket = s;
	WSARecvData.nProtocol = NETWORK_PROTOCOL_TCP;
	WSARecvData.dwAPIFlags = HOOK_WSARECV_API;

	//��ʼ��ͷ��
	StringCbCopyA(WSARecvData.HeadData.szAPIName,sizeof(WSARecvData.HeadData.szAPIName),"WSARecv");
	DataMgr.InitHeadData(&WSARecvData.HeadData);
	
	//��ȡĿ��IP�Ͷ˿�
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(s,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	StringCbCopyA(WSARecvData.szIP,sizeof(WSARecvData.szIP),inet_ntoa(sockAddr.sin_addr));
	WSARecvData.dwPort = ntohs(sockAddr.sin_port);

	
	//������Ҫ�����⴦��
	DWORD dwSize = *lpNumberOfBytesRecvd;
	LPBYTE lpMemory = DataMgr.GetBuffer(sizeof(WSARECV_API_DATA));
	

	for (DWORD i=0;i<dwBufferCount;i++)
	{
		if(dwSize >= lpBuffers[i].len )
		{
			CopyMemory(&lpMemory[WSARecvData.dwDataSize],lpBuffers[i].buf,lpBuffers[i].len);
			WSARecvData.dwDataSize+=lpBuffers[i].len;
			dwSize-=lpBuffers[i].len;
		}else
		{
			CopyMemory(&lpMemory[WSARecvData.dwDataSize],lpBuffers[i].buf,dwSize);
			WSARecvData.dwDataSize+=dwSize;
			dwSize-=lpBuffers[i].len;
		}
		if(i + 1 !=dwBufferCount)
		{
			CopyMemory(&lpMemory[WSARecvData.dwDataSize],PARTITION_SYMBOL,strlen(PARTITION_SYMBOL));
			WSARecvData.dwDataSize+=strlen(PARTITION_SYMBOL);
		}
	}


	//��������
	DataMgr.ReleaseBufferAndSend(FORMAT_NETWORKIO_DATA,
		&WSARecvData,sizeof(WSARECV_API_DATA),NETWORK_NORMAL_DATA,NULL);

	WSASetLastError(WSARecvData.nLastError);
	return nResualt;
}


int WSAAPI WSASendTo_Detours(IN SOCKET s,
							   IN LPWSABUF lpBuffers,
							   IN DWORD dwBufferCount,
							   OUT LPDWORD lpNumberOfBytesSent,
							   IN DWORD dwFlags,
							   IN const struct sockaddr FAR * lpTo,
							   IN int iTolen,
							   IN LPWSAOVERLAPPED lpOverlapped,
							   IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
							   )
{
	
	

	//�Ƚ����ݷ��ͳ�ȥ��
	typedef int (WSAAPI *WSASendToT)(SOCKET,LPWSABUF,DWORD,LPDWORD,DWORD,const struct sockaddr FAR * lpTo,
		int,LPWSAOVERLAPPED,LPWSAOVERLAPPED_COMPLETION_ROUTINE);
	WSASendToT lpWSASendTo;
	lpWSASendTo = (WSASendToT)CMHooker.GetApiSourceAddress("WSASendTo");

	if(lpWSASendTo ==  NULL) return SOCKET_ERROR;

	int nResualt = lpWSASendTo(s,lpBuffers,dwBufferCount,lpNumberOfBytesSent,dwFlags,lpTo,iTolen,lpOverlapped,lpCompletionRoutine);
	
	WSASENDTO_API_DATA WSASendToData;
	ZeroMemory(&WSASendToData,sizeof(WSASENDTO_API_DATA));

	//���淵��ֵ���ʹ�����Ϣ
	WSASendToData.nLastError = WSAGetLastError();
	WSASendToData.nResult = nResualt;
	WSASendToData.Socket = s;
	WSASendToData.nProtocol = NETWORK_PROTOCOL_UDP;
	WSASendToData.dwAPIFlags = HOOK_WSASENDTO_API;


	//��ȡĿ��IP�Ͷ˿�
	SOCKADDR_IN *lpAddr;
	lpAddr = (SOCKADDR_IN *)lpTo;
	StringCbCopyA(WSASendToData.szIP,sizeof(WSASendToData.szIP),inet_ntoa(lpAddr->sin_addr));
	WSASendToData.dwPort = ntohs(lpAddr->sin_port);


	//��ʼ��ͷ��
	StringCbCopyA(WSASendToData.HeadData.szAPIName,sizeof(WSASendToData.HeadData.szAPIName),"WSASendTo");
	DataMgr.InitHeadData(&WSASendToData.HeadData);
	
	
	//������Ҫ�����⴦��
	LPBYTE lpBuffer = DataMgr.GetBuffer(sizeof(WSASENDTO_API_DATA));
	for (DWORD i=0;i<dwBufferCount;i++)
	{
		CopyMemory(&lpBuffer[WSASendToData.dwDataSize],lpBuffers[i].buf,lpBuffers[i].len);
		WSASendToData.dwDataSize+=lpBuffers[i].len;

		if(i + 1 !=dwBufferCount)
		{
			CopyMemory(&lpBuffer[WSASendToData.dwDataSize],PARTITION_SYMBOL,strlen(PARTITION_SYMBOL));
			WSASendToData.dwDataSize+=strlen(PARTITION_SYMBOL);
		}
	}

	//��������
	DataMgr.ReleaseBufferAndSend(FORMAT_NETWORKIO_DATA,
		&WSASendToData,sizeof(WSASENDTO_API_DATA),NETWORK_NORMAL_DATA,NULL);

	WSASetLastError(WSASendToData.nLastError);
	return nResualt;
}


int WSAAPI WSARecvFrom_Detours(IN SOCKET s,
								 IN LPWSABUF lpBuffers,
								 IN DWORD dwBufferCount,
								 OUT LPDWORD lpNumberOfBytesSent,
								 IN LPDWORD dwFlags,
								 IN const struct sockaddr FAR * lpFrom,
								 IN LPINT lpFromlen,
								 IN LPWSAOVERLAPPED lpOverlapped,
								 IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
								 )
{
	
	

	//�Ƚ����ݽ���
	typedef int (WSAAPI *WSARecvFromT)(SOCKET,LPWSABUF,DWORD,LPDWORD,LPDWORD,const struct sockaddr FAR * ,
		LPINT,LPWSAOVERLAPPED,LPWSAOVERLAPPED_COMPLETION_ROUTINE);
	WSARecvFromT lpWSARecvFrom;
	lpWSARecvFrom = (WSARecvFromT)CMHooker.GetApiSourceAddress("WSARecvFrom");

	if(lpWSARecvFrom == NULL) return SOCKET_ERROR;

	int nResualt = lpWSARecvFrom(s,lpBuffers,dwBufferCount,lpNumberOfBytesSent,dwFlags,lpFrom,lpFromlen,
		lpOverlapped,lpCompletionRoutine);
	
	WSARECVFROM_API_DATA WSARecvFromData;
	ZeroMemory(&WSARecvFromData,sizeof(WSARECVFROM_API_DATA));

	//���淵��ֵ���ʹ�����Ϣ
	WSARecvFromData.nLastError = WSAGetLastError();
	WSARecvFromData.nResult = nResualt;
	WSARecvFromData.Socket = s;
	WSARecvFromData.nProtocol = NETWORK_PROTOCOL_UDP;
	WSARecvFromData.dwAPIFlags = HOOK_WSARECVFROM_API;
	
	
	//��ȡĿ��IP�Ͷ˿�
	SOCKADDR_IN *lpAddr;
	lpAddr = (SOCKADDR_IN *)lpFrom;
	StringCbCopyA(WSARecvFromData.szIP,sizeof(WSARecvFromData.szIP),inet_ntoa(lpAddr->sin_addr));
	WSARecvFromData.dwPort = ntohs(lpAddr->sin_port);

	//��ʼ��ͷ��
	StringCbCopyA(WSARecvFromData.HeadData.szAPIName,sizeof(WSARecvFromData.HeadData.szAPIName),"WSARecvFrom");
	DataMgr.InitHeadData(&WSARecvFromData.HeadData);
	
	
	//������Ҫ�����⴦��
	LPBYTE lpMemory = DataMgr.GetBuffer(sizeof(WSARECV_API_DATA));
	DWORD dwSize = *lpNumberOfBytesSent;

	for (DWORD i=0;i<dwBufferCount;i++)
	{
		if(dwSize >= lpBuffers[i].len )
		{
			CopyMemory(&lpMemory[WSARecvFromData.dwDataSize],lpBuffers[i].buf,lpBuffers[i].len);
			WSARecvFromData.dwDataSize+=lpBuffers[i].len;
			dwSize-=lpBuffers[i].len;
		}else
		{
			CopyMemory(&lpMemory[WSARecvFromData.dwDataSize],lpBuffers[i].buf,dwSize);
			WSARecvFromData.dwDataSize+=dwSize;
			dwSize-=lpBuffers[i].len;
		}
		if(i + 1 !=dwBufferCount)
		{
			CopyMemory(&lpMemory[WSARecvFromData.dwDataSize],PARTITION_SYMBOL,strlen(PARTITION_SYMBOL));
			WSARecvFromData.dwDataSize+=strlen(PARTITION_SYMBOL);
		}
	}
	

	//��������
	DataMgr.ReleaseBufferAndSend(FORMAT_NETWORKIO_DATA,
		&WSARecvFromData,sizeof(WSARECVFROM_API_DATA),NETWORK_NORMAL_DATA,NULL);

	WSASetLastError(WSARecvFromData.nLastError);
	return nResualt;
}


int WSAAPI  getaddrinfo_Detours(
								IN const char FAR * nodename,
								IN const char FAR * servname,
								IN const struct addrinfo FAR * hints,
								OUT struct addrinfo FAR * FAR * res
								)
{
	typedef int (WSAAPI * getaddrinfoT)(
		IN const char FAR * nodename,
		IN const char FAR * servname,
		IN const struct addrinfo FAR * hints,
		OUT struct addrinfo FAR * FAR * res);
	
	getaddrinfoT lpgetaddrinfo =(getaddrinfoT)CMHooker.GetApiSourceAddress("getaddrinfo");
	INT nResult = lpgetaddrinfo(nodename,servname,hints,res);
	DWORD dwError = WSAGetLastError();
	
	//�ռ���Ϣ
	if(nResult == 0)
	{
		//��ʱֻ֧��IPV4
		NETWORK_DNSDATA Dns;
		struct addrinfo *curr;
		struct sockaddr_in  *sockaddr_ipv4;
		
		for (curr = *res; curr != NULL; curr = curr->ai_next) 
		{
			if(curr->ai_family == AF_INET)
			{
				StringCbCopyA(Dns.szDNS,sizeof(Dns.szDNS),nodename);
				
				sockaddr_ipv4 = (struct sockaddr_in *)curr->ai_addr;
				
				StringCbCopyA(Dns.szIP,sizeof(Dns.szIP),inet_ntoa(sockaddr_ipv4->sin_addr));
				
				DataMgr.SendData(FORMAT_NETWORKIO_DATA,
					&Dns,
					sizeof(NETWORK_DNSDATA),
					NULL,
					NULL,
					NETWORK_DNS_DATA,
					NULL);
			}
		}
	}
	
	WSASetLastError(dwError);
	return nResult;
}	


struct hostent FAR * WSAAPI gethostbyname_Detours(const char FAR * name)
{
	typedef struct hostent FAR *( WSAAPI * gethostbynameT)(const char FAR * name);
	gethostbynameT lpgethostbyname;
	lpgethostbyname = (gethostbynameT)CMHooker.GetApiSourceAddress("gethostbyname");
	
	//�Ƚ���
	hostent FAR * hResult = lpgethostbyname(name);
	
	DWORD dwError = WSAGetLastError();
	
	if(hResult)
	{
		NETWORK_DNSDATA Dns;
		StringCbCopyA(Dns.szDNS,sizeof(Dns.szDNS),name);
		in_addr in;
 		CopyMemory(&in.S_un.S_addr,hResult->h_addr,sizeof(u_long));
		StringCbCopyA(Dns.szIP,sizeof(Dns.szIP),inet_ntoa(in));

		DataMgr.SendData(FORMAT_NETWORKIO_DATA,
			&Dns,
			sizeof(NETWORK_DNSDATA),
			NULL,
			NULL,
			NETWORK_DNS_DATA,
			NULL);
	}

	WSASetLastError(dwError);
	return hResult;
}

