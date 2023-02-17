#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<windows.h>
#include<string.h> //memset
#pragma comment(lib,"ws2_32")
int main() {
	WSADATA wsaDt;
	SOCKET s_sock, c_sock;
	struct sockaddr_in s_addrSock, c_addrSock;
	BOOL valid = 1; //소켓 옵션 설정에 사용할 변수
	WSAStartup(MAKEWORD(2, 2), &wsaDt); //윈도우소켓 초기화
	
	s_sock = socket(AF_INET, SOCK_STREAM, 0); //소켓 생성
	s_addrSock.sin_family = AF_INET; // IPv4 주소체계 사용
	s_addrSock.sin_port = htons(333); // 포트 번호 
	s_addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //주소 자동할당
	int clen; //클라이언트의 주소 길이를 저장할 변수
	char sr[1024]; // server respone, 서버가 응답할때 사용할 문자열
	char sg[1024]; // server get 클라이언트가 서버에게 보내는 값을 입력받을 변수
	setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&valid, sizeof(valid)); //소켓 옵션 설정
	
	if (bind(s_sock, (struct sockaddr*)&s_addrSock, sizeof(s_addrSock)) != 0) {
		printf("bind err No:%d", WSAGetLastError());
		return 0;
	}//소켓과 서버정보 바인드
	
	if (listen(s_sock, 3) != 0) {
		printf("listen err No: %d", WSAGetLastError());
		return 0;
	}//수락받을 수 있게 대기상태 시작

	memset(sr, 0, sizeof(sr)); //sr변수 0으로 초기화
	while (1) {
		clen = sizeof(c_addrSock);
		c_sock = accept(s_sock, (struct sockaddr*)&c_addrSock, &clen);
		if (c_sock == -1) {
			printf("accept err No:%d", WSAGetLastError);
			return 0;
		}
		memset(sg, 0, sizeof(sg)); //sg변수 즉 클라이언트로부터 수신받을 값을 저장하는 변수를 0으로 초기화
		_snprintf(sr, sizeof(sr),"HTTP/2 200 OK\r\nContent-Type: text/html\r\n\r\n빡공팟 6기 윤태영");
		//문자열을 1바이트로 처리 및 변수에 저장	
		//HTTP/2 200 OK에서 /2는 버전, 200 OK는 요청이 성공했다는 뜻을 알리는 성공 응답 상태 코드이다.
		//Content-Type: text/html의 뜻은 문자열을 뜻하며 뒤의 빡공팟 6기 윤태영을 출력한다.
		if (recv(c_sock, sg, sizeof(sg), 0) == -1) {// 소켓으로부터 즉 클라이언트 소켓으로부터 데이터를 수신받는다.
			printf("recv err No:%d", WSAGetLastError);
			return 0;
		}
		send(c_sock, sr, (int)strlen(sr), 0);//c_sock 즉 클라이언트 소켓에게 sr변수를 전송,클라이언트에게 응답 전송
		closesocket(c_sock); //소켓 해제
	}
	closesocket(s_sock); //소켓 해제
	WSACleanup(); //윈도우소켓 해제
}
