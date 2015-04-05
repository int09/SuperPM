#ifndef HookNetworkApi_h__
#define HookNetworkApi_h__


int WSAAPI send_Detours(SOCKET s,
						const char FAR * buf,
						int len,
						int flags);

int WSAAPI recv_Detours(SOCKET s, 
						char FAR * buf,
						int len,
						int flags);

int WSAAPI recvfrom_Detours(SOCKET s,
							const char* buf,
							int len,
							int flags,
							const sockaddr* to,
							int tolen);

int WSAAPI sendto_Detours(SOCKET s,
						  const char* buf,
						  int len,
						  int flags,
						  const sockaddr* to,
						  int tolen);

int WSAAPI WSASend_Detours(SOCKET s,
						   LPWSABUF lpBuffers,
						   DWORD dwBufferCount,
						   LPDWORD lpNumberOfBytesSent,
						   DWORD dwFlags,
						   LPWSAOVERLAPPED lpOverlapped,
						   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);


int WSAAPI WSARecv_Detours(IN SOCKET s,
						   IN OUT LPWSABUF lpBuffers,
						   IN DWORD dwBufferCount,
						   OUT LPDWORD lpNumberOfBytesRecvd,
						   IN OUT LPDWORD lpFlags,
						   IN LPWSAOVERLAPPED lpOverlapped,
						   IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

int WSAAPI WSASendTo_Detours(IN SOCKET s,
							 IN LPWSABUF lpBuffers,
							 IN DWORD dwBufferCount,
							 OUT LPDWORD lpNumberOfBytesSent,
							 IN DWORD dwFlags,
							 IN const struct sockaddr FAR * lpTo,
							 IN int iTolen,
							 IN LPWSAOVERLAPPED lpOverlapped,
							 IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
							  );

int WSAAPI WSARecvFrom_Detours(IN SOCKET s,
							   IN LPWSABUF lpBuffers,
							   IN DWORD dwBufferCount,
							   OUT LPDWORD lpNumberOfBytesSent,
							   IN LPDWORD dwFlags,
							   IN const struct sockaddr FAR * lpFrom,
							   IN LPINT lpFromlen,
							   IN LPWSAOVERLAPPED lpOverlapped,
							   IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
								 );

int WSAAPI  getaddrinfo_Detours(
								IN const char FAR * nodename,
								IN const char FAR * servname,
								IN const struct addrinfo FAR * hints,
								OUT struct addrinfo FAR * FAR * res
								);

struct hostent FAR * WSAAPI gethostbyname_Detours(const char FAR * name);

#endif // HookNetworkApi_h__