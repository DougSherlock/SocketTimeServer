// DayTimeClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
//#include <winsock.h> //"unp.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include	<time.h>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;
//int main()
//{
//    std::cout << "Hello World!\n";
//}

#define MAXLINE 1000
#define SA struct sockaddr

void err_quit(const char* s) { std::cout << s << std::endl; exit(0); }
void err_sys(const char* s) { std::cout << s << std::endl; }
void bzero(void* p, int size) { ZeroMemory(p, size); }

int
main(int argc, char** argv)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	//--- INITIALIZE SOCKETS -----------------------------
	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);
	cout << "TimeServer - initializing Windows sockets" << endl;
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	const char* url = "127.0.0.1"; //[google] // "206.168.112.96";
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	cout << "TimeServer - creating socket" << endl;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);	/* daytime server */

	cout << "TimeServer - binding socket" << endl;
	bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

	cout << "TimeServer - listening on socket" << endl;
	listen(listenfd, SOMAXCONN);

	for (; ; ) {
		cout << "TimeServer - listening for connection from client" << endl;
		connfd = accept(listenfd, (SA*)NULL, NULL);
		ticks = time(NULL);
		char timeBuff[100];
		ctime_s(timeBuff, sizeof(timeBuff), &ticks);
		//snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		cout << "TimeServer - accepted connection from client" << endl;
		snprintf(buff, sizeof(buff), "%.24s\r\n", timeBuff);
		cout << "TimeServer - sending time " << buff;
		send(connfd, buff, strlen(buff), 0);
		cout << "TimeServer - closing client socket" << endl;
		closesocket(connfd);
	}


	//--- CLEAN UP SOCKETS -----------------------------
	cout << "TimeServer - cleaning up Windows sockets" << endl;
	WSACleanup();

	exit(0);
}


