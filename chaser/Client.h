#include "nuss_vector.h"
#include <winsock.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Convert.h"
#include <vector>
#include <sstream>
// DEFINE  
#define MSG_LENGTH 256
#define NUM_CONNECTION_ATTEMPTS 10
#define CONNECTION_CLOSED -10

using namespace std;
struct temp
{
	Vector3f position;
	Vector3f lookAtVector;
	double sendTime;
	float speed;
};

class Client{
public:
	Client(void);
	~Client(void);
	SOCKET runrun(void);
	//char* format1(char* name,Vector3f position);
	//char* format2(char* name,Vector4f position);
	void ReadPara(char* filename);
	int initCommunication(char *serverIPAdd, char *port, SOCKET *sockfd, WSAData *wsaData);
	int clientAction(char* message, SOCKET sockfd);
	DWORD getsleeptime(void);
	char *PackData(Vector3f position, Vector3f lookAtVector, double sendTime, float speed);
	temp UnpackData(char str[]);
	string vectortostring(Vector3f position);
	

};