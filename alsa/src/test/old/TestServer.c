/*
 * TestServer.c
 *
 *  Created on: Sep 14, 2020
 *      Author: dccan
 */

#include <amneiht/am_sock.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int mainSeRvEr(int argc, char **argv) {
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(9981);
	ser.sin_addr.s_addr = inet_addr("192.168.2.100");
	//client 1
	int sock = asock_m_UdpSocket();
	char id[20];
	asock_m_register("can", id, &ser, sock);
	//   client 2
	char id2[20];
	int sock2 = asock_m_UdpSocket();
	asock_m_register("pika", id2, &ser, sock2);

//	asock_m_Invite("pika", "can", id, &ser, &cli, sock);

	asip_d_mess *mess = null;
	asip_m_addHeader(&mess, asip_d_Header_Type, asip_d_Type_Invite);
	asip_m_addHeader(&mess, asip_d_Header_From, "can");
	asip_m_addHeader(&mess, asip_d_Header_To, "pika");
	asip_m_addHeader(&mess, "id", id);
	char buff[1000];
	int lg = asip_m_toStringMess(mess, 1000, buff);
	int err = sendto(sock, buff, lg, 0, (struct sockaddr*)& ser,
			sizeof(struct sockaddr));
	if (err < 1) {
		printf("loi nhan data invite");
		return -1;
	}
	char buf[200];
	recv(sock2, buf, 200, MSG_WAITALL);
	printf("%s", buf);
	close(sock);
	close(sock);
	close(sock2);
	return  0;
}
