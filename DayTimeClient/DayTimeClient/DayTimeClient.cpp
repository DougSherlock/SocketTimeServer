// DayTimeClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
//#include <winsock.h> //"unp.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

//int main()
//{
//    std::cout << "Hello World!\n";
//}

#define MAXLINE 1000
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
	cout << "TimeClient - initializing Windows sockets" << endl;
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	const char* url = "127.0.0.1"; //[google] // "206.168.112.96";

	//if (argc != 2)
	//	err_quit("usage: a.out <IPaddress>");

	struct addrinfo* address = NULL;
	struct sockaddr_in serv_addr; // Server address data structure.
	struct hostent* server;      // Server data structure.
	//int nRes= getaddrinfo("time.google.com", NULL, NULL, &address); // Convert URL to IP.
	//freeaddrinfo(address);

	cout << "TimeClient - creating socket" << endl;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);	/* daytime server */
	cout << "TimeClient - initializing socket" << endl;
	if (inet_pton(AF_INET, url, &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error");

	cout << "TimeClient - connecting to server" << endl;
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	cout << "TimeClient - receiving from server" << endl;
	while ((n = /*read*/recv(sockfd, recvline, MAXLINE, 0)) > 0) {
		recvline[n] = 0;	/* null terminate */
		cout << "TimeClient - received data from server" << endl;
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	//--- CLEAN UP SOCKETS -----------------------------
	cout << "TimeClient - cleaning up Windows sockets" << endl;
	WSACleanup();

	exit(0);
}