//
// UNetwork.cpp
//

/*-----------------------------------------------------*\
			HQ Tech, Make Technology Easy!       
 More information, please go to http://hqtech.nease.net.
/*-----------------------------------------------------*/

#include "stdafx.h"
#include "UNetwork.h"
#include <Winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
BOOL UNetwork::GetHostInfo(char * outIP, char * outName)
{
	char   name[300];
	if (gethostname(name, 300) == 0)
	{
		if (outName)
		{
			strcpy(outName, name);
		}

		PHOSTENT  hostinfo;
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			LPCSTR pIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			strcpy(outIP, pIP);
			return TRUE;
		}
	}
	return FALSE;
}

void UNetwork::DumpSocketError(void)
{
	switch (WSAGetLastError())
	{
	case WSANOTINITIALISED:
		TRACE("A successful WSAStartup call must occur before using this function. ");
		break;
	case WSAENETDOWN:
		TRACE("The network subsystem has failed. ");
		break;
	case WSAEACCES:
		TRACE("The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address. ");
		break;
	case WSAEINVAL:
		TRACE("An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled. ");
		break;
	case WSAEINTR:
		TRACE("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall. ");
		break;
	case WSAEINPROGRESS:
		TRACE("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. ");
		break;
	case WSAEFAULT:
		TRACE("The buf or to parameters are not part of the user address space, or the tolen parameter is too small. ");
		break;
	case WSAENETRESET:
		TRACE("The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. ");
		break;
	case WSAENOBUFS:
		TRACE("No buffer space is available. ");
		break;
	case WSAENOTCONN:
		TRACE("The socket is not connected (connection-oriented sockets only). ");
		break;
	case WSAENOTSOCK:
		TRACE("The descriptor is not a socket. ");
		break;
	case WSAEOPNOTSUPP:
		TRACE("MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations. ");
		break;
	case WSAESHUTDOWN:
		TRACE("The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH. ");
		break;
	case WSAEWOULDBLOCK:
		TRACE("The socket is marked as nonblocking and the requested operation would block. ");
		break;
	case WSAEMSGSIZE:
		TRACE("The socket is message oriented, and the message is larger than the maximum supported by the underlying transport. ");
		break;
	case WSAEHOSTUNREACH:
		TRACE("The remote host cannot be reached from this host at this time. ");
		break;
	case WSAECONNABORTED:
		TRACE("The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable. ");
		break;
	case WSAECONNRESET:
		TRACE("The virtual circuit was reset by the remote side executing a hard or abortive close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet. The application should close the socket as it is no longer usable. ");
		break;
	case WSAEADDRNOTAVAIL:
		TRACE("The remote address is not a valid address, for example, ADDR_ANY. ");
		break;
	case WSAEAFNOSUPPORT:
		TRACE("Addresses in the specified family cannot be used with this socket. ");
		break;
	case WSAEDESTADDRREQ:
		TRACE("A destination address is required. ");
		break;
	case WSAENETUNREACH:
		TRACE("The network cannot be reached from this host at this time. ");
		break;
	case WSAETIMEDOUT:
		TRACE("The connection has been dropped, because of a network failure or because the system on the other end went down without notice. ");
		break;
	default:
		TRACE("Unknown socket error. ");
		break;
	}
}
