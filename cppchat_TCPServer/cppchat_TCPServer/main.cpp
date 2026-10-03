#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

//服务端主要步骤
//1.创建socket套接字
//2.给这个socket绑定一个端口号
//3.给这个socket开启监听属性
//4.等待客户端连接
//5.开启通讯
//6.关闭通讯

int main()
{
	//windows上使用网络功能需要开启网络权限

	WSAData wsaData;	
	//创建结构体变量，用来接收初始化信息

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//请求使用Winsock2.2，传入wsaData地址让函数能把信息写入变量

	//--------------1.创建socket套接字---------------
	
	//SOCKET socket(
	//int af,		//协议地址簇 IPV4/IPV6 AF_INET/AF_INET6
	//int type,		//类型 流式协议/帧式协议 SOCK_STREAM/SOCK_DGRAM
	//int protocol	//保护协议 0
	//);

	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == INVALID_SOCKET)
	{
		std::cout << "failed to init socket" << std::endl;
		std::cout << "error code:"<< GetLastError()<< std::endl;
		return -1;
	}
	
	//--------------2.给socket绑定一个端口---------------
	 
	//struct sockaddr_in {
	//	ADDRESS_FAMILY sin_family;	//协议地址簇
	//	USHORT sin_port;			//端口号
	//	IN_ADDR sin_addr;			//IP地址
	//	CHAR sin_zero[8];			//保留字节
	//}
	
	struct sockaddr_in local = { 0 };
	local.sin_family = AF_INET;					//使用ipv4协议
	local.sin_port = htons(8080);				//大小端问题，要加hton关键字，s代表short
	local.sin_addr.s_addr = htonl(INADDR_ANY);	//只接受哪个网卡的数据
	//local.sin_addr.s_addr = inet_addr("0.0.0.0");		//将字符串IP转化为整数IP
			
	//int bind(
	//	SOCKET s,
	//	const struct sockaddr FAR * name,	
	//	int namelen
	//);

	int ret1 = bind(listen_socket, (struct sockaddr*)&local, sizeof(local));
	if (ret1 == -1)
	{
		std::cout << "failed to bind socket" << std::endl;
		std::cout << "error code:" << GetLastError() << std::endl;
		return -1;
	}

	//---------------3.给这个socket开启监听属性---------------

	//listen(
	//	SOCKET s,
	//	int backlog
	//);

	int ret2 = listen(listen_socket, 10);
	if (ret2 == -1)
	{
		std::cout << "failed to listen socket" << std::endl;
		std::cout << "error code:" << GetLastError() << std::endl;
		return -1;
	}

	//-------------------4.等待客户端连接-----------------------
	
	//accept(
	//	SOCKET s,						//监听socket
	//	struct sockaddr* addr,		//客户端的IP地址和端口号
	//	int FAR * addrlen				//结构的大小
	//);

	while (1)
	{
		SOCKET client_socket = accept(listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
		{
			continue;
		}

	//---------------------5.开始通讯---------------------------
		while (1)
		{
			char buffer[1024] = { 0 };

			//recv(
			//	SOCKET s,	//客户端socket
			//	char * buf,	//接受的数据存到哪里
			//	int len,	//接收的长度
			//	int flags	//0
			//);
			recv(client_socket, buffer, 1024, 0);

			std::cout << buffer << std::endl;

			send(client_socket, buffer, strlen(buffer), 0);
		}
		
	//---------------------6.关闭通讯---------------------------

		closesocket(client_socket);
	}
	
	return 0;
}