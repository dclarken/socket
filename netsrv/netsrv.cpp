#include<winsock2.h>
#include<stdio.h>

#include<windows.h>
using namespace std;


int main()
{
	//加载套接字库
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

	//创建套接字,因为是基于UDP的，所以用SOCK_DGRAM.
	SOCKET sockSrv=socket(AF_INET,SOCK_DGRAM,0);

	//对于服务器端，接着应该进行绑定
	SOCKADDR_IN addrSrv;//定义一个地址结构体的变量
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;//地址族
	addrSrv.sin_port=htons(6000);//端口号

	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	char recvBuf[100];//字符数组，用来接收信息
	char sendBuf[100];//用来发送信息
	char tempBuf[200];//用来存放中间数据

	SOCKADDR_IN addrClient;//定义一个地址结构体的变量,在通讯的时候，我们需要获取和我们通讯的这一方的地址信息,这一获取是我们通过调用recvfrom来获得的，但是我们需要提供一个地址结构体的变量
	int len =sizeof(SOCKADDR);

	//while循环，保证通讯过程能够不断的循环下去
	while(1)
	{
		//接收数据
		recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);
		//判断是否结束对话，q表示结束
		if('q'==recvBuf[0])
		{
			sendto(sockSrv,"q",strlen("q")+1,0,(SOCKADDR*)&addrClient,len);
			printf("Char end!\n");
			break;
		}
		//将数据格式化到tempBuf中
		sprintf(tempBuf,"%s say:%s",inet_ntoa(addrClient.sin_addr),recvBuf);//addrClient.sin_addr表示对方的IP地址，inet_ntoa将IP转换为点分十进制表示的形式，如172.0.0.1
		//将输出信息设置成绿色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		printf("%s\n",tempBuf);//将数据打印输出
		//将字体颜色选回默认
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("Please input data:\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		gets(sendBuf);//从键盘输入数据
		sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrClient,len);//发送数据

	}
	//当循环结束的时候,关闭套接字
	closesocket(sockSrv);
	//终止对套接字的使用
	WSACleanup();

    return 0;
}
