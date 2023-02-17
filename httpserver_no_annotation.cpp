#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<windows.h>
#include<string.h>
#pragma comment(lib,"ws2_32")
int main() {
	WSADATA wsaDt;
	SOCKET s_sock, c_sock;
	struct sockaddr_in s_addrSock, c_addrSock;
	BOOL valid = 1;
	WSAStartup(MAKEWORD(2, 2), &wsaDt);
	
	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	s_addrSock.sin_family = AF_INET;
	s_addrSock.sin_port = htons(333); 
	s_addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	int clen; 
	char sr[1024];
	char sg[1024];
	setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&valid, sizeof(valid));
	
	if (bind(s_sock, (struct sockaddr*)&s_addrSock, sizeof(s_addrSock)) != 0) {
		printf("bind err No:%d", WSAGetLastError());
		return 0;
	}
	
	if (listen(s_sock, 3) != 0) {
		printf("listen err No: %d", WSAGetLastError());
		return 0;
	}

	memset(sr, 0, sizeof(sr)); 
	while (1) {
		clen = sizeof(c_addrSock);
		c_sock = accept(s_sock, (struct sockaddr*)&c_addrSock, &clen);
		if (c_sock == -1) {
			printf("accept err No:%d", WSAGetLastError);
			return 0;
		}
		memset(sg, 0, sizeof(sg)); 
		_snprintf(sr, sizeof(sr),"HTTP/2 200 OK\r\nContent-Type: text/html\r\n\r\nºý°øÆÌ 6±â À±ÅÂ¿µ");
		if (recv(c_sock, sg, sizeof(sg), 0) == -1) {
			printf("recv err No:%d", WSAGetLastError);
			return 0;
		}
		send(c_sock, sr, (int)strlen(sr), 0);
		closesocket(c_sock); 
	}
	closesocket(s_sock);
	WSACleanup();
}
