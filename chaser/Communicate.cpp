#include "Communicate.h"




int Communicate::initCommunication(char *serverIPAdd, char *port, SOCKET *sockfd, WSAData *wsaData){
	int rc = 0;
	unsigned short serverPort = 0;
	//SOCKET clientSock = INVALID_SOCKET;
	struct sockaddr_in serverAdd;
	int clientAddLen = 0;
	//fd_set write_fds;
	timeval timeout;		// used to determine a wait time for the completion of the select action
	int count = 0;			// counter for the number of attmepts to connected to the server
	int connected = 0;		// a flag to determine whether the connectivity was successful

	// init the time out
	*sockfd = INVALID_SOCKET;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;	// 0.5 seconds

	// Start Winsock up
	if ((rc = WSAStartup(MAKEWORD(2, 2), wsaData)) != 0) {
		cout << "WSAStartup error.  rc = " << rc << "." << endl;
		cout.flush();
		return(1);
	}

	cout << " max number of sockets = " << wsaData->iMaxSockets << " fd_setsocket = " << FD_SETSIZE << endl;
	// open the socket 
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);	// service request will determine the protocol could be IPPRORTO_TCP
	if (*sockfd == INVALID_SOCKET) {
		// error
		rc = -1;
		goto err;
	}

	sscanf(port, "%hu", &serverPort);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.S_un.S_addr = inet_addr(serverIPAdd);
	serverAdd.sin_port = htons(serverPort);

	while (!connected && count < NUM_CONNECTION_ATTEMPTS) {
		count++;
		rc = connect(*sockfd, (struct sockaddr *) &serverAdd, sizeof(serverAdd));
		if (rc == SOCKET_ERROR) {
			// error
			rc = WSAGetLastError();
			switch (rc) {
			case WSAECONNREFUSED:
				// server may not be up yet
				Sleep(1000);
				break;
			case WSAEWOULDBLOCK:
				// connection was not established yet.  
				// do not call connect again on the same socket.

				Sleep(1000);
				break;
			default:
				rc = -1;
				goto err;
				break;
			}
		}
		else {
			connected = 1;
			// If iMode!=0, non-blocking mode is enabled.
			u_long iMode = 1;
			ioctlsocket(*sockfd, FIONBIO, &iMode);
		}
	}

	if (!connected) rc = -1;
err:
	return(rc);

}

/********************************************************************************/

/// reads data from the sockets and send it back

int serverAction(fd_set *master_fds)

{

	int len;
	int rc = 0;
	int i, j;
	timeval timeout;
	SOCKET readSocket, writeSocket;
	fd_set read_fds, write_fds;
	char buf[MSG_LENGTH];
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;  // wait for 0.5 seconds
	memset(buf, 0, MSG_LENGTH);
	// copy the master list
	read_fds = *master_fds;
	if (read_fds.fd_count > 0) {
		rc = select(0, &read_fds, NULL, NULL, &timeout);		// check the status of the sockets descriptors that the server is listening to

		switch (rc) {
		case 0:
			// nothing ready for readv return
			goto err;
			break;
		case SOCKET_ERROR:
			// check what is the error
			rc = WSAGetLastError();
			goto err;
			break;
		default:
			// some sockects are ready for read
			break;
		}
	}

	for (i = 0; i < master_fds->fd_count; i++) {
		readSocket = master_fds->fd_array[i];
		rc = recv(readSocket, buf, MSG_LENGTH - 1, 0);    // wait until something was sent
		switch (rc) {
		case 0:
			// connecion is closed
			// remove from the list
			FD_CLR(readSocket, master_fds);
			break;
		case SOCKET_ERROR:
			// error
			rc = WSAGetLastError();
			switch (rc) {
			case WSAECONNREFUSED:
				// server may not be up yet
				Sleep(1000);
				break;
			case WSAEWOULDBLOCK:
				// connection was not established yet.  
				// do not call connect again on the same socket.
				break;
			case WSAECONNRESET:
				FD_CLR(readSocket, master_fds);
				break;
			default:
				rc = -1;
				goto err;
				break;
			}
			break;
		default:
			break;
		}


		cout << "server received -> " << buf << endl;
		// something was read from the connectcion
		// send it back
		len = rc;
		write_fds = *master_fds;

		// can add some code to remove the sending socket !!!
		for (j = 0; j < write_fds.fd_count; j++) {
			// send to all the clients
			writeSocket = write_fds.fd_array[j];
			rc = send(writeSocket, buf, len, 0);   // note the number of bytes that were sent can be smaller then 
			if (rc == SOCKET_ERROR) {
				rc = WSAGetLastError();
				switch (rc) {
				case 0:
					// connecion is closed
					// remove from the list
					FD_CLR(writeSocket, master_fds);
					break;
				case WSAECONNRESET:
					// error
					FD_CLR(writeSocket, master_fds);
					rc = -1;
					break;
				default:
					closesocket(writeSocket);
					FD_CLR(writeSocket, master_fds);
					rc = -1;
					break;
				}
			}
			else {
				if (rc != len) {
					// not all bytes were sent 
					// need loop
					rc = 5;
				}
				cout << "server sent: " << buf << endl;
			}
		}
	}
	rc = 0;

err:
	return(rc);
}




char* messageformat(Vector3f p){
	char* buf1;
	char* buf2;
	char* buf3;
	gcvt(p.x, 3, buf1);
	gcvt(p.y, 3, buf2);
	gcvt(p.z, 3, buf3);
	string str = (string)buf1 + ' ' + (string)buf2 + ' ' + buf3;
	char* message = const_cast<char*>(str.c_str());
return message;
}


char* messageformat(Vector4f p){
	char* buf1;
	char* buf2;
	char* buf3;
	char* buf4;
	gcvt(p.x, 3, buf1);
	gcvt(p.y, 3, buf2);
	gcvt(p.z, 3, buf3);
	gcvt(p.w, 3, buf4);
	string str = (string)buf1 + ' ' + (string)buf2 + ' ' + buf3 + ' ' + buf4;
	char* message = const_cast<char*>(str.c_str());
	return message;
}























//#include <WinSock2.h>
//#include <fstream>
//#include <string>
//#include <stdlib.h>
//#include <iostream>
//#include "nuss_vector.h"
//SOCKADDR_IN Srvaddr;
//SOCKET Srvsock;
//SOCKADDR_IN Clientsock;
//using namespace std;
//
//
////initclientsocket, bind ip and port
//int InitCliSocket(char *ipAddress, int port){
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int Resn = 0;
//	int ErrCoden = 0;
//	int bExit = 0;
//
//	wVersionRequested = MAKEWORD(1, 1);
//
//	Resn = WSAStartup(wVersionRequested, &wsaData);
//
//	if (0 != Resn)
//	{
//		printf("err code:%d", WSAGetLastError());
//		return 0;
//	}
//	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
//	{
//		printf("Version Different");
//		WSACleanup();
//		return 0;
//	}
//
//	Srvsock = socket(AF_INET, SOCK_DGRAM, 0);
//
//	if (INVALID_SOCKET == Srvsock)
//	{
//		printf("err code:%d", WSAGetLastError());
//		return 0;
//	}
//
//	return 1;
//}
//
//	//message send
//	int sendmsg(char *buf)
//	{
//		int nLen = sizeof(SOCKADDR_IN);
//		sendto(Srvsock, buf, strlen(buf), 0, (SOCKADDR*)&Srvaddr, sizeof(SOCKADDR));
//		memset(buf, 0, strlen(buf));
//		recvfrom(Srvsock, buf, 1024, 0, (SOCKADDR*)&Clientsock, &nLen);
//		return 0;
//	}
//
	
//		
//	}
//
//
