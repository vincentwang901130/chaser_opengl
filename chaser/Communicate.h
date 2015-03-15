#include <stdio.h>
#include<windows.h>
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include "nuss_vector.h"
using namespace std;


// DEFINE  
#define MSG_LENGTH 256
#define NUM_CONNECTION_ATTEMPTS 10
#define CONNECTION_CLOSED -10



class Communicate
{
public:
	
	int		initCommunication(char *serverIPAdd, char *port, SOCKET *sockfd, WSAData *wsaData);
	int 	clientAction(SOCKET sockfd);

};
