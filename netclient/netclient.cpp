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

	//�����׽���
	SOCKET sockClient=socket(AF_INET,SOCK_DGRAM,0);

	//����UDP�Ŀͻ�����˵��������Ҫȥ��,����Ҫ������Ϣ��Ҫ���͵��Է������ĵ�ַ��Ϣ,Ҳ���Ƿ������˵ĵ�ַ��Ϣ
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	addrSrv.sin_family=AF_INET;//��ַ��
	addrSrv.sin_port=htons(6000);//���ն˿ں�


	char recvBuf[100];//��������
	char sendBuf[100];//��������
	char tempBuf[200];//��ʱ���ݴ洢

	int len=sizeof(SOCKADDR);//���ڷ��ؽ������ݵĵ�ַ�ṹ�ĳ���,�����Ⱦ�����ʼ��

	while(1)
	{
		printf("please input data:\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
		gets(sendBuf);
		sendto(sockClient,sendBuf,strlen(sendBuf)+1,0,
			(SOCKADDR*)&addrSrv,len);//��������
        recvfrom(sockClient,recvBuf,100,0,
			(SOCKADDR*)&addrSrv,&len);//��������
		//�ж��Ƿ�����Ի�
		if('q'==recvBuf[0])
		{
			sendto(sockClient,"q",strlen("q")+1,0,
				(SOCKADDR*)&addrSrv,len);
			printf("Chat end!\n");
			break;
		}
		//�����յ������ݸ�ʽ����tempBuf��
		sprintf(tempBuf,"%s say:%s",inet_ntoa(addrSrv.sin_addr),recvBuf);
		//����������Ϊ��ɫ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
		printf("%s\n",tempBuf);
		//������ѡ��Ĭ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}
	//�ر��׽���
	closesocket(sockClient);
	//��ֹ���׽��ֿ��ʹ��
	WSACleanup();
    return 0;
}
