//
// CUDPManager.cpp
// 

/*-----------------------------------------------------*\
			HQ Tech, Make Technology Easy!       
 More information, please go to http://hqtech.nease.net.
/*-----------------------------------------------------*/

#include "stdafx.h"
#include "CUDPManager.h"
#include "UNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CUDPManager::CUDPManager()
{
	mSckReceiver = INVALID_SOCKET;
	mSckSender   = INVALID_SOCKET;
	mTargetIP    = 0x7f000001; // 127.0.0.1
	mTargetPort  = 10025;
	mLocalPort   = 10025;

	mIsReceiving = FALSE;
	mRcvThread   = NULL;

	WSADATA wsaData;
	WORD version=MAKEWORD(2,2);
	int ret=WSAStartup(version,&wsaData);
	if(ret!=0)
	{
		TRACE("Initilize Error!\n");
		//m_bInit=FALSE;
		::MessageBox(NULL,"socket 初始化错误","提示信息",NULL);
	}

}

CUDPManager::~CUDPManager()
{
	
	DeleteSender();
	DeleteReceiver();
	StopReceiving();
	WSACleanup();
}

void CUDPManager::SetTargetIP(DWORD inIP)
{
	mTargetIP = inIP;
}

DWORD CUDPManager::GetTargetIP(void)
{
	return mTargetIP;
}

void CUDPManager::SetTargetIP(const char * inIP)
{
	mTargetIP = ntohl(inet_addr(inIP));
}

void CUDPManager::GetTargetIP(char * outIP)
{
	if (outIP)
	{
		struct in_addr   in;
		in.S_un.S_addr = htonl(mTargetIP);
		char * pStr = inet_ntoa(in);
		strcpy(outIP, pStr);
	}
}

void CUDPManager::SetTargetPort(WORD inPort)
{
	mTargetPort = inPort;
}

WORD CUDPManager::GetTargetPort(void)
{
	return mTargetPort;
}

void CUDPManager::SetLocalPort(WORD inPort)
{
	mLocalPort = inPort;
}

WORD CUDPManager::GetLocalPort(void)
{
	return mLocalPort;
}

BOOL CUDPManager::CreateSender(void)
{
	DeleteSender();

	mSckSender = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSckSender != INVALID_SOCKET)
	{
		return TRUE;
	}
	return FALSE;
}

void CUDPManager::DeleteSender(void)
{
	if (mSckSender != INVALID_SOCKET)
	{
		closesocket(mSckSender);
		mSckSender = INVALID_SOCKET;
	}
}

BOOL CUDPManager::CreateReceiver(void)
{
	DeleteReceiver();

	mSckReceiver = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSckReceiver != INVALID_SOCKET)
	{
		BOOL flag = TRUE;
		int ret = setsockopt(mSckReceiver, SOL_SOCKET, SO_REUSEADDR, 
			(char *) &flag, sizeof(flag));
		if (ret == SOCKET_ERROR) 
		{
			DeleteReceiver();
			return FALSE;
		}

		SOCKADDR_IN  addr;
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port        = htons(mLocalPort);
		ret = bind(mSckReceiver, (struct sockaddr*) &addr, sizeof(addr));
		if (ret == SOCKET_ERROR) 
		{
			DeleteReceiver();
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

void CUDPManager::DeleteReceiver(void)
{
	if (mSckReceiver != INVALID_SOCKET)
	{
		closesocket(mSckReceiver);
		mSckReceiver = INVALID_SOCKET;
	}
}

// Sending methods
BOOL CUDPManager::Send(const char * inBuffer, long inLength)
{
	SOCKADDR_IN  addr;
	memset((char *) &addr, 0, sizeof(addr));  
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(mTargetIP);
//	addr.sin_port        = htons(mTargetPort);
	addr.sin_port        = htons(10025);

	int val = sendto(mSckSender, inBuffer, inLength, 0, 
		(sockaddr *) &addr, sizeof(addr));
	return (val != SOCKET_ERROR);
}

BOOL CUDPManager::SendTo(const char * inBuffer, long inLength, 
						 DWORD inIP, WORD inPort)
{
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock != INVALID_SOCKET)
	{
		SOCKADDR_IN  addr;
		memset((char *) &addr, 0, sizeof(addr));  
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = htonl(inIP);
		addr.sin_port        = htons(inPort);

		int val = sendto(sock, inBuffer, inLength, 0, 
			(sockaddr *) &addr, sizeof(addr));
		if (val == SOCKET_ERROR)
		{
#ifdef _DEBUG
			UNetwork::DumpSocketError();
#endif
		}
		closesocket(sock);
		return (val != SOCKET_ERROR);
	}
	return FALSE;
}

// Receiving methods
BOOL CUDPManager::StartReceiving(void)
{
	// Create socket if necessary
	if (mSckReceiver == INVALID_SOCKET)
	{
		CreateReceiver();
	}

	if (mSckReceiver != INVALID_SOCKET)
	{
		if (mIsReceiving)
		{
			return TRUE;
		}
		
		DWORD threadID = 0;
		mRcvThread = CreateThread(NULL, 0, ReceivingThrd, 
			this, 0, &threadID);
		return (mRcvThread != NULL);
	}
	return FALSE;
}

void CUDPManager::StopReceiving(void)
{
	if (mIsReceiving)
	{
		DeleteReceiver();
		// Make sure the receiving thread has been terminated 
		if (mRcvThread != NULL) 
		{
			WaitForSingleObject(mRcvThread, INFINITE);
			mRcvThread = NULL;
		}
	}
}

DWORD WINAPI CUDPManager::ReceivingThrd(void * pParam)
{
	ASSERT(pParam);
	CUDPManager * pController = (CUDPManager*) pParam;
	pController->ReceivingLoop();
	return 0;
}

void CUDPManager::ReceivingLoop(void)
{
	// Just for demo!
	struct sockaddr_in  addr_cli;
	int  addr_cli_len = sizeof(addr_cli);
	char buffer[1024];
	long bytes = 0;

	mIsReceiving = TRUE;
	while (mIsReceiving)
	{				
		int addr_cli_len = sizeof(addr_cli);
		bytes = recvfrom(mSckReceiver, (char *)buffer, 1024, 0,
			(LPSOCKADDR) &addr_cli, (int *) &addr_cli_len);
		if (bytes == SOCKET_ERROR ||
			bytes == 0)
		{
			mIsReceiving = FALSE;
		}
		else
		{
			buffer[bytes] = '\0';
			char *  pStr  = inet_ntoa(addr_cli.sin_addr);
			
			// Check whether to reply to the sender
			if (buffer[0] == '1')
			{
				CString str = "0Received OK.";  
				SendTo(str, str.GetLength() + 1, ntohl(inet_addr(pStr)), 10026);
			}

			// Pop up a message box
			CString msg;
			msg.Format(_T("Receive from %s \nContent:%s"), pStr, buffer+1);
			AfxMessageBox(msg);
		}
	}
}
