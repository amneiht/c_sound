/*
 * server.c
 *
 *  Created on: Sep 4, 2020
 *      Author: dccan
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../asip/asip.h"
#include "../user/auser.h"

#define true 1
#define false 0
#define boolean int
#define MAXSIZE  1000

boolean run = true;
auser_d_user *user = null;

void gen_random(char *s, const int len);
void responeSip(char *mess, struct sockaddr_in *client, int sockfd);
void intToChar(int i, char *ms) {
	sprintf(ms, "%d", i);
}
void handlesip(asip_d_mess *point, struct sockaddr_in *client, int sockfd);

void server_run();
int main(int argc, char **argv) {
	server_run();
	return 1;
}

void server_run() {
	fprintf(stderr, "[log] server run \n");
	int sockfd;
	int port = 9981;
	char *buff = (char*) malloc(MAXSIZE * sizeof(char));
	struct sockaddr_in servaddr, cliaddr;
	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	if (bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr))
			< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	listen(sockfd, 5);
	int len, n;
	size_t max = MAXSIZE;
	len = sizeof(cliaddr);  //len is value/resuslt
	while (run) {
		memset(&cliaddr, 0, sizeof(cliaddr));
		n = recvfrom(sockfd, buff, max,
		MSG_WAITALL, (struct sockaddr*) &cliaddr, (socklen_t*) &len);
		fprintf(stderr, "\n[log] request comming \n");
		if (buff[n - 1] != '0') {
			buff[n] = '\0';
		}
		asip_d_mess *point = null;
		asip_m_parseMess(&point, buff);
		handlesip(point, &cliaddr, sockfd);
		asip_m_freeMess(point);
	}
	free(buff);
	auser_m_freeUser(user);
	close(sockfd);
	fprintf(stderr, "[log] server close");
}
void handlesip(asip_d_mess *point, struct sockaddr_in *client, int sockfd) {

	char *type = asip_m_findHeaderValue(point, asip_d_Header_Type);
	fprintf(stderr, "\n[log] request type : %s \n", type);
	if (strcasecmp(type, asip_d_Type_Regiter) == 0) {
		char *name = asip_m_findHeaderValue(point, asip_d_Header_To);
		char id[10];
		gen_random(id, 8);

		// tao ban tin phan hoi
		asip_d_mess *res = null;
		asip_m_addHeader(&res, asip_d_Header_Type, "ok");
		asip_m_addHeader(&res, "id", id);
		fprintf(stderr, "[log] id:%s \n", id);
		// them nguoi dung
		auser_m_addUser(&user, client, id, name);

		//phan hoi
		char ng[1000];
		asip_m_toStringMess(res, 1000, ng);
		responeSip(ng, client, sockfd);

		// giai phong bo nho
		asip_m_freeMess(res);

	} else if (strcmp(type, asip_d_Type_Invite) == 0) {
		int size = -1;
		char *to = asip_m_findHeaderValue(point, asip_d_Header_To); // lay nguoi nhan
		auser_d_user **us1 = auser_m_listUser(user, to, &size); // danh sach nguoi nhan
		fprintf(stderr, "\n[log] number of callee : %d \n", size);
		if (size == 0) {
			//TODO
			fprintf(stderr, "[log] no avaible user to call\n");
			asip_d_mess *asip = NULL;
			asip_m_addHeader(&asip, asip_d_Header_Type, "cancel");
			asip_m_addHeader(&asip, asip_d_Header_From, "server");
			char ng[1000];
			asip_m_toStringMess(asip, 1000, ng);
			responeSip(ng, client, sockfd);
			asip_m_freeMess(asip);
			return;
		}
		// lay tham so cong ,ip nguoi gui
		char *ip = inet_ntoa(client->sin_addr);
		int port = ntohs(client->sin_port);
		char pt[10];
		intToChar(port, pt);
		// them tieu de vao ban tin
		asip_m_addHeader(&point, "host", ip);
		asip_m_addHeader(&point, "port", pt);

		//phan hoi
		char ng[1000];
		asip_m_toStringMess(point, 1000, ng);
		// gui phan  hoi
		struct sockaddr_in res;
		res.sin_family = AF_INET;
		res.sin_port = (us1[0])->port;
		res.sin_addr.s_addr = (us1[0])->ip;
		responeSip(ng, &res, sockfd);

		// giai phong bo nho
		free(us1);
	} else if ((strcmp(type, "ok") == 0) || (strcmp(type, "cancel") == 0)) {
		char *id = asip_m_findHeaderValue(point, "id");
		if (id == null) {
			fprintf(stderr, "[error]null id\n");
			return;
		}
		auser_d_user *ua = auser_m_getUser(user, id);
		if (ua == null) {
			fprintf(stderr, "[error]null user\n");
			return;
		}
		if (strcmp(type, "ok") == 0) {
			char *ip = inet_ntoa(client->sin_addr);
			int port = ntohs(client->sin_port);
			char pt[10];
			intToChar(port, pt);
			// them tieu de vao ban tin
			asip_m_addHeader(&point, "host", ip);
			asip_m_addHeader(&point, "port", pt);
		}
		char *pt = asip_m_findHeaderValue(point, "port2");
		if (pt == null) {
			fprintf(stderr, "[error]null port\n");
			return;
		}
		char ng[1000];
		asip_m_toStringMess(point, 1000, ng);
		struct sockaddr_in res;
		res.sin_family = AF_INET;
		res.sin_port = htons(atoi(pt));
		res.sin_addr.s_addr = ua->ip;
		responeSip(ng, &res, sockfd);

	}

}
void responeSip(char *mess, struct sockaddr_in *client, int sockfd) {
	int n = strlen(mess);
	fprintf(stderr, "[log] mess is : %d byte \n", n);
	int leng = sizeof(*client);
	n = sendto(sockfd, mess, n, MSG_CONFIRM, (struct sockaddr*) client, leng);
	if (n < 0) {
		fprintf(stderr, "[error]sendto false \n");
	}
}
void gen_random(char *s, const int len) {
	static const char alphanum[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	static int alength = sizeof(alphanum);
	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (alength - 1)];
	}
	s[len] = 0;
}
