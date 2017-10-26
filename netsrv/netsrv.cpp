#include<winsock2.h>
#include<stdio.h>

#include<windows.h>
using namespace std;


int main()
{
	//�����׽��ֿ�
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

	//�����׽���,��Ϊ�ǻ���UDP�ģ�������SOCK_DGRAM.
	SOCKET sockSrv=socket(AF_INET,SOCK_DGRAM,0);

	//���ڷ������ˣ�����Ӧ�ý��а�
	SOCKADDR_IN addrSrv;//����һ����ַ�ṹ��ı���
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;//��ַ��
	addrSrv.sin_port=htons(6000);//�˿ں�

	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	char recvBuf[100];//�ַ����飬����������Ϣ
	char sendBuf[100];//����������Ϣ
	char tempBuf[200];//��������м�����

	SOCKADDR_IN addrClient;//����һ����ַ�ṹ��ı���,��ͨѶ��ʱ��������Ҫ��ȡ������ͨѶ����һ���ĵ�ַ��Ϣ,��һ��ȡ������ͨ������recvfrom����õģ�����������Ҫ�ṩһ����ַ�ṹ��ı���
	int len =sizeof(SOCKADDR);

	//whileѭ������֤ͨѶ�����ܹ����ϵ�ѭ����ȥ
	while(1)
	{
		//��������
		recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);
		//�ж��Ƿ�����Ի���q��ʾ����
		if('q'==recvBuf[0])
		{
			sendto(sockSrv,"q",strlen("q")+1,0,(SOCKADDR*)&addrClient,len);
			printf("Char end!\n");
			break;
		}
		//�����ݸ�ʽ����tempBuf��
		sprintf(tempBuf,"%s say:%s",inet_ntoa(addrClient.sin_addr),recvBuf);//addrClient.sin_addr��ʾ�Է���IP��ַ��inet_ntoa��IPת��Ϊ���ʮ���Ʊ�ʾ����ʽ����172.0.0.1
		//�������Ϣ���ó���ɫ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		printf("%s\n",tempBuf);//�����ݴ�ӡ���
		//��������ɫѡ��Ĭ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("Please input data:\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		gets(sendBuf);//�Ӽ�����������
		sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrClient,len);//��������

	}
	//��ѭ��������ʱ��,�ر��׽���
	closesocket(sockSrv);
	//��ֹ���׽��ֵ�ʹ��
	WSACleanup();

    return 0;
}
