#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

#include <pthread.h>

#include "udpserver.h"

#define portnum 6221


void *udpserver_Task(void* pArg)
{
	int sockfd;
	int num;
	char buf[256];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int length=sizeof(struct sockaddr);

	//�����׽���
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf(" create failed\n");
		return; 
	}

	//��ʼ��IP��Ϣ
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family =AF_INET;
	server_addr.sin_port = htons(portnum);//����˿ں�
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//����Ϊ�κ�IP������ͨ��
	//�󶨵�ַ
	bind(sockfd,(struct sockaddr*)&server_addr,length);


	while(1)
	{

		bzero(buf,sizeof(buf));//���buf
		num=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&length);
		if (num <= 0)
		{
			if (errno == EINTR)
				continue;
			
			printf("recvfrom error");
		}
		else if(num > 0)
		{
			printf("receive data: %d:%s\n",num,buf);
			sendto(sockfd, buf, num, 0,
				   (struct sockaddr *)&client_addr, length);
			printf("reply data: %d:%s\n",num, buf);
		}

	}

	//��������	
	close(sockfd);

	return;
}



void udpserver_Init(void)
{
	pthread_t udpTask;

	pthread_create(&udpTask,0,udpserver_Task,NULL);

	pthread_join(udpTask,NULL);
}


