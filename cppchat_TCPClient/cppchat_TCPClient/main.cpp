#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

//客户端主要步骤
//1.创建socket套接字
//2.连接服务器
//3.开始通讯
//4.关闭连接

int main()
{
	//windows上使用网络功能需要开启网络权限

	WSAData wsaData;
	//创建结构体变量，用来接收初始化信息

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//请求使用Winsock2.2，传入wsaData地址让函数能把信息写入变量

	//-----------------1.创建socket套接字-------------------

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		std::cout << "fail to init socket" << std::endl;
		return -1;
	}

	//-------------------2.连接服务器---------------------
	struct sockaddr_in target;
	target.sin_family = AF_INET;
	target.sin_port = htons(8080);
	target.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	int ret = connect(client_socket, (struct sockaddr*)&target, sizeof(target));
	if (ret == -1)
	{
		std::cout << "fail to connect server" << std::endl;
		closesocket(client_socket);
		return -1;
	}

	//---------------------3.开始通讯----------------------

	while (1)
	{
		std::string sbuffer;
		std::getline(std::cin, sbuffer);

		send(client_socket, sbuffer.c_str(), static_cast<int>(sbuffer.size()), 0);

		char rbuffer[1024];
		recv(client_socket, rbuffer, sizeof(rbuffer), 0);

		if (ret <= 0)
		{
			break;
		}
		std::cout << rbuffer;
	}

	//---------------------4.关闭连接----------------------
	closesocket(client_socket);
	

	return 0;
}