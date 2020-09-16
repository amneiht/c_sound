/*
 * testSocketClose.c
 *
 *  Created on: Sep 14, 2020
 *      Author: dccan
 */

#include <amneiht/am_sock.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

void* testSock1(void *x) {
	int sock = *((int*) x) ;
	char buff[100];
	recv(sock, buff, 100, MSG_WAITALL);
	return NULL ;
}
int maintestSock1(int argc, char **argv) {
	int sock = asock_m_UdpSocket();
	pthread_t s1;
	pthread_create(&s1, NULL, testSock1, &sock);
	sleep(3);
	close(sock);
//	pthread_join(s1,NULL);
	return 1;
}

