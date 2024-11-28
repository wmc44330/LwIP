/*
 * @Author: MC Wang @ GDUT
 * @Date: 2023-12-24 21:49:56
 * @LastEditTime: 2024-11-25 18:49:08
 * @Description:
 * @FilePath: \lwIP\User\Apps\AppTaskTcpServer.c
 *
 */
#include "AppTaskTcpServer.h"

// TCP服务器任务控制块
OS_TCB AppTaskTcpServerTCB;
// TCP服务器任务堆栈
CPU_STK AppTaskTcpServerStk[APP_CFG_TASK_TCP_SERVER_STK_SIZE];

#define TCPSERVER_DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__)

uint8_t TcpServerBuffer[TCP_SERVER_RX_BUFSIZE] __attribute__((section(".exsram.TcpServerBuffer")));

int sockfd, connfd;

void AppTaskTcpServer(void *p_arg)
{
	OS_ERR err = OS_ERR_NONE;

	struct sockaddr_in tcp_s_sock, tcp_c_sock = {0};
	uint8_t ip_str[20] = {0};
	char addrlen = sizeof(tcp_c_sock);

	memset(TcpServerBuffer, 0, TCP_SERVER_RX_BUFSIZE);

	TCPSERVER_DEBUG("TCP Server App Start...\r\n");
	// 创建TCP服务器套接字
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	tcp_s_sock.sin_family = AF_INET;
	tcp_s_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	tcp_s_sock.sin_port = htons(SERVER_PORT);

	int ret = bind(sockfd, (struct sockaddr *)&tcp_s_sock, sizeof(tcp_s_sock));
	if (ret < 0)
	{
		TCPSERVER_DEBUG("bind failed\r\n");
		close(sockfd);
	}

	/* 使服务器进入监听状态 */
	ret = listen(sockfd, 50);
	if (ret < 0)
	{
		TCPSERVER_DEBUG("listen error\r\n");
		close(sockfd);
	}

	/* 接收客户端发送过来的数据 */
	while (DEF_TRUE)
	{
		connfd = accept(sockfd, (struct sockaddr *)&tcp_c_sock, (socklen_t *)&addrlen);
		if (connfd < 0)
		{
			TCPSERVER_DEBUG("accept error\r\n");
			break;
		}
		else
		{
			TCPSERVER_DEBUG("\nClient is connectting...\n");
			memcpy(ip_str, inet_ntoa(tcp_c_sock.sin_addr.s_addr), sizeof(ip_str));
			TCPSERVER_DEBUG("Client IP : %s\n", ip_str);
			TCPSERVER_DEBUG("Client Port: %d\n", tcp_c_sock.sin_port);
		}

		while (DEF_TRUE)
		{
			// 接收缓冲区清零
			memset(TcpServerBuffer, 0x0, sizeof(TcpServerBuffer));
			// 读数据，堵塞进程
			ret = recv(connfd, TcpServerBuffer, sizeof(TcpServerBuffer), 0);
			if (ret <= 0)
			{
				TCPSERVER_DEBUG("recv error: %d\n", ret);
				close(connfd);
				break;
			}
			TCPSERVER_DEBUG("recv success %d : \n%s\n", ret, TcpServerBuffer);
		}
	}
	close(sockfd);
	OSTaskDel(NULL, &err);
}
