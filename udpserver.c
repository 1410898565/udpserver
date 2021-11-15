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

	//创建套接字
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf(" create failed\n");
		return; 
	}

	//初始化IP信息
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family =AF_INET;
	server_addr.sin_port = htons(portnum);//网络端口号
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//设置为任何IP都可以通信
	//绑定地址
	bind(sockfd,(struct sockaddr*)&server_addr,length);


	while(1)
	{

		bzero(buf,sizeof(buf));//清除buf
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

	//结束连接	
	close(sockfd);

	return;
}



void udpserver_Init(void)
{
	pthread_t udpTask;

	pthread_create(&udpTask,0,udpserver_Task,NULL);

	pthread_join(udpTask,NULL);
}


