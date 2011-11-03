#ifndef __NETGAME_H__
#define __NETGAME_H__
#include "command.h"

#if defined (_TYPEDEFSOCKET_)
typedef unsigned int           SOCKET;
typedef struct { int count; SOCKET array[FD_SETSIZE]; } fd_set;
#endif

class netinit
{
public:
	netinit();
	~netinit();
};

class netgameclient
{
public:
	netgameclient(int port, char* srvname);
	bool init();
	~netgameclient();
	bool send(const struct command *cmd);
private:
	SOCKET iSock;
	long iAddr;
	int iPort;
};

class netgameserver
{
public:
	netgameserver(int port);
	~netgameserver();
	bool init();
	bool recv(struct command &cmd);
private:
	int iPort;
	SOCKET iSock;
	//fd_set iReadSet;
	struct command iCmd;
	unsigned int iBufRead;
};

#endif