#include "Client.h"



Client::Client(void){

}

Client::~Client(void)
{
}


SOCKET Client::runrun(void)
{
	int rc = 0;
	WSAData wsaData;
	SOCKET sockfd = INVALID_SOCKET;
	DWORD second;
	char* ipaddress;
	char* port;
	char buffer[256];
	string parastring[6];

	fstream out;
	int i = 0;
	out.open("Text.txt", ios::in);
	while (!out.eof()){
		out.getline(buffer, 256, '\n');
		if (buffer[0] != ':'){
			parastring[i] = buffer;
			i++;
		}

	}
	out.close();
	ipaddress = const_cast<char*>(parastring[0].c_str());
	port = const_cast<char*>(parastring[1].c_str());
	second = (DWORD)const_cast<char*>(parastring[2].c_str());
	rc = initCommunication(ipaddress, port, &sockfd, &wsaData);
	
	
	if (rc != 0) {
		cout << "Could not establish connectrion to the server" << endl;
		cout << "Check the server, ip address and port number and try again" << endl;
		goto err;
	}


err:

	// Shut Winsock back down and take off.
	if (sockfd != INVALID_SOCKET) {
		closesocket(sockfd);
	}

	WSACleanup();

	return sockfd;
}


int Client::initCommunication(char *serverIPAdd, char *port, SOCKET *sockfd, WSAData *wsaData)
{
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




int Client::clientAction(char *message, SOCKET sockfd)

{

	int len;
	int rc;
	int i = 0, sLength;

	//char *s2 = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29";
	//char *s3 = "This is a long message.  The purpose of this long message is to  demonstrate that a message can be broken in to many small pieces by the TCP protocol, at the sender end, and then assembled on the receiver end.  This demonstrates that packets are sent in orer and then reassebled unti all the message was correctly received.  This demonstrates that order in which the packets are sent is preserved and the reassembling of the message is done correctly.";
	//char *s = "3 100 100           ."; //"This is a short message string."; 
	//char *s1 = "the sign.";



	char buf[MSG_LENGTH];
	int continue2Recv = 1;

	sLength = strlen(message);
	while (i <= sLength) {
		// send the string until it has completed
		// note that the message size is artificially limited in length 
		len = min(MSG_LENGTH - 1, sLength - i);	// send four bytes at a time

		// in this case the message is sent only once and the client asks for the connectin to be termineated.  
		// this does not have to be the case. The connection can be maintained if needed. 
		if (len == 0) {
			//end of string
			//shutdown(sockfd,1);
			break;
		}

		// copy part of the message to be sent
		memset(buf, 0, MSG_LENGTH);			// clear the memory
		strcpy(buf, "[CLIENT 1] ");

		//memcpy(&buf[strlen(buf)], &sMy[i],len-strlen(buf));	// copy part of the message
		int currentPosition;
		if (len + strlen(buf) > MSG_LENGTH - 1) currentPosition = len - strlen(buf);
		else currentPosition = len;
		memcpy(&buf[strlen(buf)], &message[i], currentPosition);	// copy part of the message
cout << "CLIENT 1 sending -> " << buf << endl;


		// send the message
		rc = strlen(buf);
		rc = send(sockfd, buf, strlen(buf), 0);   // note the number of bytes that were sent can be smaller then 
		if (rc == SOCKET_ERROR) {
			// error
			rc = -1;
			break;
		}

		i += len;
		if (rc != len) {
			// not all bytes were sent 
			// need loop
			// ignore for now
			rc = 5;
		}

		if (rc == 0) {
			//end of string
			//shutdown(sockfd,1);
			break;
		}
	}

	// we know that the server echos the data back
	while (continue2Recv) {
		// wait for the server to send the messages back.  In this case it is synchronous.
		// the next message will not be sent until the original message was "echoed" back
		memset(buf, 0, MSG_LENGTH);
		rc = recv(sockfd, buf, MSG_LENGTH - 1, 0);    // wait until something was sent

		if (rc == 0) {
			// connecion is closed 
			rc = CONNECTION_CLOSED;
			break;
		}
		else if (rc == SOCKET_ERROR) {
			// error
			// could be that nothing is sent back. In this case you may want to leave the while loop.
			rc = WSAGetLastError();
			switch (rc) {
			case WSAEWOULDBLOCK:
				// connection was not established yet.  
				// do not call connect again on the same socket.

				continue2Recv = 0;
				rc = 0;
				break;
			case WSAECONNABORTED:
				rc = CONNECTION_CLOSED;
				goto err;
				break;

			default:
				rc = -1;
				goto err;
				break;
			}
		}
		else 	{
			cout << "client receeived -->" << buf << endl;
		}
	}

err:
	return(rc);
}




char *Client::PackData(Vector3f position, Vector3f lookAtVector, double sendTime, float speed){
	
	char *datapack;
	string str1, str2, str3,str4;
	str1 = vectortostring(position);
	str2 = vectortostring(lookAtVector);
	str3 = stringify(sendTime);
	str4 = stringify(speed);
	string final = str1+' '+ str2+' '+str3+' '+str4;
	datapack = new char[final.length() + 1];
	strcpy(datapack, final.c_str());
	
	return datapack;
}


temp Client::UnpackData(char str[]){
	temp tp;
	double	time;
	float speed;
	Vector3f position;
	Vector3f look;
	string strin = str;
	//remove head;
	string newstr=strin.substr(9);
	string buf;
	stringstream ss(newstr);  
	vector<string> arr;
	while (ss >> buf)
		arr.push_back(buf);

	position.x=atof(arr[0].c_str());
	position.y = atof(arr[1].c_str());
	position.z = atof(arr[2].c_str());
	look.x = atof(arr[3].c_str());
	look.y = atof(arr[4].c_str());
	look.z = atof(arr[5].c_str());
	time = atof(arr[6].c_str());
	speed = atof(arr[7].c_str());
	tp.position = position;
	tp.lookAtVector = look;
	tp.sendTime = time;
	tp.speed = speed;
	return tp;
}


string Client::vectortostring(Vector3f position){
	string str1 = stringify(position.x);
	string str2 = stringify(position.y);
	string str3 = stringify(position.z);
	string str=str1+' '+str2+' '+str3;
	return str;
}





































//char* client::format1(char* name, Vector3f position){
//	
//	string str;
//	sprintf_s(buf1, "%.8f", position.x);
//	sprintf_s(buf2, "%.8f", position.y);
//	sprintf_s(buf3, "%.8f", position.z);
//	str = name+':'+(string)buf1 + ' ' + (string)buf2 + ' ' + (string)buf3;
//	return const_cast<char*>(str.c_str());
//	
//
//}
//char* client::format2(string name,Vector4f position){
//
//
//	string str;
//	sprintf_s(buf1, "%.8f", position.x);
//	sprintf_s(buf2, "%.8f", position.y);
//	sprintf_s(buf3, "%.8f", position.z);
//	sprintf_s(buf4, "%.8f", position.w);
//	str = name + ':' + (string)buf1 + ' ' + (string)buf2 + ' ' + (string)buf3+' '+(string)buf4;
//	return const_cast<char*>(str.c_str());
//}