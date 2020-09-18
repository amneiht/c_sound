/*
 * sipclient.c
 *
 *  Created on: Sep 7, 2020
 *      Author: dccan
 */

#include <amneiht/am_sock.h>
#include <amneiht/asip.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int mainSipC(int argc, char **argv) {
	int sockfd = asock_m_UdpSocket();
	if (sockfd < 0)
		return -1;
	char buff[1000];
	struct sockaddr_in ser;
	ser.sin_addr.s_addr = inet_addr("192.168.1.100");
	ser.sin_family = AF_INET;
	ser.sin_port = htons(9981);
	asock_m_register("can", buff, &ser, sockfd);
	printf("d : %s", buff);
	close(sockfd);
	return 0;
}
int asock_m_acceptInvite(asip_d_mess *invite, char *name,
		struct sockaddr_in **cli, struct sockaddr_in *ser, int sockfd) {

	char *host = asip_m_findHeaderValue(invite, "host");
	if (host == null) {
		return -1;
	}
	char *port = asip_m_findHeaderValue(invite, "port");
	if (port == null) {
		return -1;
	}
	*cli = (struct sockaddr_in*) calloc(1, sizeof(struct sockaddr_in));
	struct sockaddr_in *cliaddr = *cli;
	cliaddr->sin_port = htons(atoi(port));
	cliaddr->sin_addr.s_addr = inet_addr(host);
	cliaddr->sin_family = AF_INET;

	asip_d_mess *mess = null;
	asip_m_addHeader(&mess, asip_d_Header_Type, "ok");
	asip_m_addHeader(&mess, asip_d_Header_From, name);
	char *id = asip_m_findHeaderValue(invite, "id");
	char *to = asip_m_findHeaderValue(invite, "from");

	asip_m_addHeader(&mess, asip_d_Header_To, to);
	asip_m_addHeader(&mess, "id", id);
	asip_m_addHeader(&mess, "port2", port);
	char buf[1000];
	int n = asip_m_toStringMess(mess, 1000, buf);

	n = sendto(sockfd, buf, n, 0, (struct sockaddr*) ser,
			sizeof(struct sockaddr));
//	printf("mess :\n%s \n", buf);
	asip_m_freeMess(mess);
	return 1;
	// loi thong tin thi gui thong tin huy

}
int asock_m_cancleInvite(asip_d_mess *invite, char *name,
		struct sockaddr_in *ser, int sockfd) {
	asip_d_mess *mess = null;
	char *id = asip_m_findHeaderValue(invite, "id");
	char *to = asip_m_findHeaderValue(invite, "from");
	char *port = asip_m_findHeaderValue(invite, "port");
	if (id == NULL || to == NULL || port == NULL) {
		printf(__FILE__"mess null parameter \n");
		return -1;
	}
	asip_m_addHeader(&mess, asip_d_Header_Type, asip_d_Type_Cancel);
	asip_m_addHeader(&mess, asip_d_Header_From, name);

	asip_m_addHeader(&mess, asip_d_Header_To, to);
	asip_m_addHeader(&mess, "id", id);
	asip_m_addHeader(&mess, "port2", port);
	char buf[1000];
	int n = asip_m_toStringMess(mess, 1000, buf);
	n = sendto(sockfd, buf, n, 0, (struct sockaddr*) ser,
			sizeof(struct sockaddr));
	asip_m_freeMess(mess);
	if (n < 0)
		return -1;
	return 0;

}
int asock_m_Invite(char *callee, char *caller, char *id,
		struct sockaddr_in **cli, struct sockaddr_in *ser, int sockfd) {

	asip_d_mess *mess = null, *respone = null;
	asip_m_addHeader(&mess, asip_d_Header_Type, asip_d_Type_Invite);
	asip_m_addHeader(&mess, asip_d_Header_From, caller);
	asip_m_addHeader(&mess, asip_d_Header_To, callee);
	asip_m_addHeader(&mess, "id", id);
	char buff[1000];
	int lg = asip_m_toStringMess(mess, 1000, buff);
	int err = sendto(sockfd, buff, lg, 0, (struct sockaddr*) ser,
			sizeof(struct sockaddr));
	if (err < 1) {
		printf("loi nhan data invite");
		return -1;
	}
	err = recv(sockfd, buff, 1000, MSG_WAITALL);
	if (err < 1) {
		printf("loi nhan data invite");
		return -1;
	}
	if (buff[err - 1] != '\0') {
		buff[err] = '\0';
	}
	int res = -1;
//	printf("header la : %s \n", buff);
	asip_m_parseMess(&respone, buff);
	char *type = asip_m_findHeaderValue(respone, asip_d_Header_Type);
	if (type == null) {
		printf("no type header \n");
		res = -3;
	} else if (strcmp(type, "cancel") == 0) {
		printf("cancel request \n");
		res = -2;
	} else if (strcmp(type, "ok") == 0) {
		res = -1;
		char *host = asip_m_findHeaderValue(respone, "host");
		if (host == null)
			goto END;
		char *port = asip_m_findHeaderValue(respone, "port");
		if (port == null)
			goto END;
		*cli = (struct sockaddr_in*) malloc(1 * sizeof(struct sockaddr_in));
		struct sockaddr_in *cliaddr = *cli;
		cliaddr->sin_port = htons(atoi(port));
		cliaddr->sin_addr.s_addr = inet_addr(host);
		cliaddr->sin_family = AF_INET;
		res = 1;
	}
	END: asip_m_freeMess(respone);
	asip_m_freeMess(mess);
	return res;
}
int asock_m_register(char *name, char *out, struct sockaddr_in *ser, int sockfd) {
	asip_d_mess *mess = null, *respone = null;
	asip_m_addHeader(&mess, asip_d_Header_Type, asip_d_Type_Regiter);
	asip_m_addHeader(&mess, asip_d_Header_To, name);
	char buff[1000];
	int leng = asip_m_toStringMess(mess, 1000, buff);
	int err = sendto(sockfd, buff, leng, 0, (struct sockaddr*) ser,
			sizeof(struct sockaddr));
	if (err < 0) {
		printf("loi gui data register");
		return -1;
	}
	err = recv(sockfd, buff, 1000, MSG_WAITALL);
	if (err < 1) {
		printf("loi nhan data register");
		return -1;
	}
	if (buff[err - 1] != '\0') {
		buff[err] = '\0';
	}
	asip_m_parseMess(&respone, buff);
	char *id = asip_m_findHeaderValue(respone, "id");
	int lg = strlen(id);
	for (int i = 0; i < lg; i++) {
		out[i] = id[i];
	}
	out[lg] = '\0';
	asip_m_freeMess(respone);
	asip_m_freeMess(mess);
	return 1;
}

