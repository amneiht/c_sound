/*
 * sock.c
 *
 *  Created on: Sep 7, 2020
 *      Author: dccan
 */

#include <asm-generic/socket.h>
#include <bits/stdint-uintn.h>
#include <netdb.h>	//hostent
#include <netinet/in.h>
#include <stdio.h> //printf
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

uint32_t asock_m_hostToIp(char *hostname) {
	struct hostent *he;
	struct in_addr **addr_list;
	if ((he = gethostbyname(hostname)) == NULL) {
		// get the host info
		herror("gethostbyname");
		return 0;
	}
	addr_list = (struct in_addr**) he->h_addr_list;
	uint32_t res = (*addr_list[0]).s_addr;
	return res;
}
int asock_m_UdpServerSocket(int port) {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("loi con nect udp");
		return -1;
	}

	int on = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on,
			sizeof(on)) < 0) {
		close(sockfd);
		return -1;
	}
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr))
			< 0) {
		perror("bind failed");
		return -1;
	}
	int a = 65535;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &a, sizeof(int)) == -1) {
		fprintf(stderr, "Error setting socket opts:SO_RCVBUF \n");
	}
	listen(sockfd, 5);
	return sockfd;

}
int asock_m_UdpSocket() {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("loi con nect udp");
		return -1;
	}
//	struct timeval tv;
//	tv.tv_sec = 20;
//	tv.tv_usec = 0;
//	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
//		perror("Error");
//	}
	int on = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on,
			sizeof(on)) < 0) {
		close(sockfd);
		return -1;
	}
	return sockfd;

}
