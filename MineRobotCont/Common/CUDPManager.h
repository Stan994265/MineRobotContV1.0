//
// CUDPManager.h
// 

#ifndef __H_CUDPManager__
#define __H_CUDPManager__
#include<winsock2.h>

class CUDPManager
{
private:
	SOCKET		mSckReceiver;
	SOCKET		mSckSender;
	DWORD		mTargetIP;      // Host byte order
	WORD		mTargetPort;
	WORD		mLocalPort;     // Serverl port

	BOOL		mIsReceiving;
	HANDLE		mRcvThread;

public:
	CUDPManager();
	~CUDPManager();

	void SetTargetIP(DWORD inIP);    // For client
	DWORD GetTargetIP(void);
	void SetTargetIP(const char * inIP);
	void GetTargetIP(char * outIP);

	void SetTargetPort(WORD inPort); // For client
	WORD GetTargetPort(void);

	void SetLocalPort(WORD inPort);  // For server
	WORD GetLocalPort(void);

	BOOL CreateSender(void);
	void DeleteSender(void);
	BOOL CreateReceiver(void);
	void DeleteReceiver(void);

	// Sending and Receiving...
	BOOL Send(const char * inBuffer, long inLength);
	BOOL SendTo(const char * inBuffer, long inLength, 
		DWORD inIP, WORD inPort);

	BOOL StartReceiving(void);
	void StopReceiving(void);

private:
	void ReceivingLoop(void);
	static DWORD WINAPI ReceivingThrd(void * pParam);
};

#endif // __H_CUDPManager__