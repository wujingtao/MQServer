#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9999
#define IP "192.168.1.56"
int main(int argc, char* argv[])
{
	//1.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		return -1;
	}

	//2.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		return -1;
	}

	//3.
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP);

	int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (ret==SOCKET_ERROR)
	{
		cout << "connect error" << endl;
		return -1;
	}

	//4.


	int index = 0;

	int recvCount = 0;
	while (true)
	{
		recvCount = 0;

		//接收大小
		char *szBuffLen = new char[4];
		ret = recv(sock, szBuffLen, 4, 0);
		if (ret < 4)
		{
			//TODO:
			cout << "lengthBuff error" << endl;
			continue;
		}

		int buffSize = *(int*)szBuffLen;
		char *szBuff = new char[buffSize];
		cout << "buffSize:" << buffSize << endl;

		do 
		{
			ret = recv(sock, szBuff+recvCount, buffSize-recvCount, 0);
			if (ret>0)
			{
				recvCount += ret;
				cout << "recv count:" << recvCount << endl;
			}
			else if (ret==SOCKET_ERROR)
			{
				cout << "recvCount error" << endl;
			}
		} while (recvCount<buffSize);


		++index;

		stringstream ss;
		ss << index << ".jpg";
		ofstream ofs(ss.str(), ofstream::binary);
		if (ofs.is_open())
		{
			ofs.write(szBuff, recvCount);
		}
		ofs.close();
	}

	closesocket(sock);
	WSACleanup();

	return 0;
}