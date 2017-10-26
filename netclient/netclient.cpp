#include<Winsock2.h>
#include<stdio.h>
using namespace std;
int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return 0;
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return 0;
	}

	//创建套接字
	SOCKET sockClient=socket(AF_INET,SOCK_DGRAM,0);

	//基于UDP的客户端来说，它不需要去绑定,但是要设置信息将要发送到对方机器的地址信息,也就是服务器端的地址信息
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	addrSrv.sin_family=AF_INET;//地址族
	addrSrv.sin_port=htons(6000);//接收端口号


	char recvBuf[100];//接收数据
	char sendBuf[100];//发送数据
	char tempBuf[200];//临时数据存储

	int len=sizeof(SOCKADDR);//用于返回接收数据的地址结构的长度,必须先经过初始化

	while(1)
	{
		printf("please input data:\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
		gets(sendBuf);
		sendto(sockClient,sendBuf,strlen(sendBuf)+1,0,
			(SOCKADDR*)&addrSrv,len);//发送数据
        recvfrom(sockClient,recvBuf,100,0,
			(SOCKADDR*)&addrSrv,&len);//接收数据
		//判断是否结束对话
		if('q'==recvBuf[0])
		{
			sendto(sockClient,"q",strlen("q")+1,0,
				(SOCKADDR*)&addrSrv,len);
			printf("Chat end!\n");
			break;
		}
		//将接收到的数据格式化到tempBuf中
		sprintf(tempBuf,"%s say:%s",inet_ntoa(addrSrv.sin_addr),recvBuf);
		//将字体设置为黄色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
		printf("%s\n",tempBuf);
		//将字体选回默认
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}
	//关闭套接字
	closesocket(sockClient);
	//终止对套接字库的使用
	WSACleanup();
    return 0;
}
