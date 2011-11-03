// I do not need more than 2 sockets open
#define FD_SETSIZE 2 
#include <winsock.h>
#include "netgame.h"

static const netinit NetInit;

netinit::netinit()
	{
#if defined(_WINDOWS)
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );
	WSAStartup( wVersionRequested, &wsaData );
#endif
	}


netinit::~netinit()
	{
#if defined (_WINDOWS)
	WSACleanup();
#endif
	}

netgameclient::netgameclient(int port,char *hostname)
	{
	iPort=port;
	hostent* hostInfo=gethostbyname(hostname); 
	if (hostInfo)
		iAddr=*(long*)hostInfo->h_addr_list[0];
	else
		iAddr=htonl(INADDR_NONE);
	}


bool netgameclient::init()
	{
	iSock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
//	unsigned long nonblocking=1;
	unsigned long nonblocking=0;
#ifdef _WINDOWS
	ioctlsocket(iSock,FIONBIO,&nonblocking);
#else
	ioctl(iSock,FIONBIO,&nonblocking);	
#endif
	return true;
	}
	
netgameclient::~netgameclient()
	{
	closesocket(iSock);
	}

bool netgameclient::send(const struct command *cmd)
	{
	struct sockaddr_in saddr;
	saddr.sin_addr.S_un.S_addr=iAddr;
	saddr.sin_port=htons(iPort);
	saddr.sin_family=AF_INET;
	return (::sendto(iSock,(const char *)cmd,sizeof(struct command),0,(const struct sockaddr*)&saddr,sizeof(saddr)) == sizeof(struct command));
	}

netgameserver::netgameserver(int port)
	{
	iBufRead=0;
	iPort=port;
//	FD_ZERO(&iReadSet);
	}

bool netgameserver::init()
	{
	struct sockaddr_in saddr;
	saddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(iPort);
	saddr.sin_family=AF_INET;
	iSock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	bind(iSock,(const sockaddr*) &saddr,sizeof(saddr));
	unsigned long nonblocking=1;
#ifdef _WINDOWS
	ioctlsocket(iSock,FIONBIO,&nonblocking);
#else
	ioctl(iSock,FIONBIO,&nonblocking);	
#endif
//	FD_SET(iSock,&iReadSet);
	return true;
	}

netgameserver::~netgameserver()
	{
	closesocket(iSock);
	}

bool netgameserver::recv(struct command &cmd)
	{
	struct timeval tval={0,0};
//	int selret=::select((int)iSock,&iReadSet,NULL,NULL,&tval);
//	if (selret>0)
		{
		char *bufaddr=(char*)(&iCmd)+iBufRead;
		int retval=::recv(iSock,bufaddr,sizeof(iCmd)-iBufRead,0);
		if (retval == SOCKET_ERROR)
			{
#ifdef _WINDOWS
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return false;
#else
			return false;
#endif	
			}
		else
			{
			iBufRead += retval;
			if (iBufRead == sizeof(iCmd))
				{
				iBufRead=0;
				cmd=iCmd;
				return true;
				}
			else
				return false;
			}
		}
	return false;
	}

