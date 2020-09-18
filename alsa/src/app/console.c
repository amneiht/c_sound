/*
 * console.c
 *
 *  Created on: Sep 8, 2020
 *      Author: dccan
 */

#include <amneiht/am_sock.h>
#include <amneiht/asip.h>
#include <amneiht/asupport.h>
#include <amneiht/g711.h>
#include <amneiht/rtp.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <bits/stdint-intn.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <pthread.h>
#include <pulse/error.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void sip_login(int *sock, char **buff, int leng, struct sockaddr_in *ser,
		char *id);
void sip_call(struct sockaddr_in *ser, char **buff, int d);
void accept_call(struct sockaddr_in *ser);
void cancel_invite(struct sockaddr_in *ser);
void* AudioStream(void *x);
void* AudioGet(void *x);
void* handle_sip(void *x);
int praseParameters(char *str, char **mg);
int console_app();
void initFormat();

pa_sample_spec format;
asip_d_mess *mess = null; // xin loi thay uy truoc
int is_call = 0;
pthread_t a_call;
pthread_t a_send;
pthread_t _sip = 0;
static int run = 1;
int sip_ser = 1;
char user[30];
char userid[30];
void testStream() {
	initFormat();
	struct sockaddr_in ser;
	ser.sin_addr.s_addr = inet_addr("192.168.2.103");
	ser.sin_port = htons(10000);
	ser.sin_family = AF_INET;
	int sockfd = asock_m_UdpServerSocket(10000);
	if (sockfd < 0) {
		printf("false");
	}
	int err = pthread_create(&a_call, null, AudioGet, &sockfd); // tao luong nhan
	if (err < 0)
		return;
	err = pthread_create(&a_send, null, AudioStream, &ser); // tao luong gui
	if (err < 0)
		return;

	sleep(20);
	run = 0;
	pthread_join(a_call, NULL); // doi huy luong
	pthread_join(a_send, NULL); // doi huy luong
}
int main(int argc, char **argv) {

//	testStream();
	return console_app();
}
void initFormat() {
	format.channels = 2;
	format.format = PA_SAMPLE_S16LE;
	format.rate = 8000;
}
int console_app() {
	initFormat();
	char **buff = (char**) asup_m_initMarix(20, 50, sizeof(char));
	char input[200];
	char info[] =
			"this is test program , create by amneiht a.k.a dcc\ntype help for more info\n";
	char help[] =
			"\nlogin to server :login <user> <ip> <port> \nmake a call: call <name>\naccept call:a\ncancel call:c\nexit :q\n";
	struct sockaddr_in ser;
	printf("%s", info);
	int sock = -1;
	while (1) {

		fflush(stdin); // lam sach luong
		fgets(input, 200, stdin);
		int d = praseParameters(input, buff);
		if (d > 0) {
			if (strcmp(buff[0], "login") == 0) {
				sip_login(&sock, buff, d, &ser, userid);
			} else if (strcmp(buff[0], "call") == 0) {
				sip_call(&ser, buff, d);
			} else if (strcmp(buff[0], "a") == 0) {
				accept_call(&ser);
			} else if (strcmp(buff[0], "q") == 0) {
				break;
			} else if (strcmp(buff[0], "c") == 0) {
				cancel_invite(&ser);
			} else if (strcmp(buff[0], "help") == 0) {
				printf("%s", help);
			}
		}

	}
	sip_ser = 0;
	if (mess != null) {
		asip_m_freeMess(mess);
		mess = null;
	}
	sip_ser = 0;
	run = 0;
	pthread_join(a_call, NULL); // doi huy luong
	pthread_join(a_send, NULL); // doi huy luong
	pthread_join(_sip, NULL);
	asup_m_freeMatrix((void**) buff, 20);
	return 0;
}
/**
 * call other people
 * call <name>
 */
void sip_call(struct sockaddr_in *ser, char **buff, int d) {

	int sockfd = asock_m_UdpSocket();
	struct sockaddr_in *cli = null;
	if (d < 2)
		return;
	int err = asock_m_Invite(buff[1], user, userid, &cli, ser, sockfd);
	if (err < 0)
		return;
	int *x = malloc(sizeof(int));
	*x = sockfd;
	err = pthread_create(&a_call, null, AudioGet, x); // tao luong nhan
	if (err < 0) {
		free(x);
		return;
	}

	err = pthread_create(&a_send, null, AudioStream, cli); // tao luong gui
	if (err < 0)
		return;
	is_call = 1;
}
/*
 * login command
 * login <user> <ip> <port> <pass:chua ho tro>
 */
void sip_login(int *sock, char **buff, int leng, struct sockaddr_in *ser,
		char *id) {
//	if (leng < 4) {
//		printf("you should use command login <user> <ip> <port>");
//		return;
//	}
	int stl = strlen(buff[1]);
	for (int i = 0; i <= stl; i++) {
		user[i] = buff[1][i];
	}
	close(*sock);
	char *ip, *port;
	if (leng == 4) {
		ip = buff[2];
		port = buff[3];
	} else {
		ip = "192.168.2.103";
		port = "9981";
	}
	int sp = asock_m_UdpSocket();
	// thieu dat time out
	if (sp < 0)
		return;
	memset(ser, 0, sizeof(struct sockaddr_in));
	ser->sin_family = AF_INET;
	ser->sin_addr.s_addr = asock_m_hostToIp(ip);
	int d = atoi(port);
	if (d == 0)
		return;
	ser->sin_port = ntohs(d);
	*sock = sp;
	asock_m_register(user, id, ser, sp);
	printf("your id is %s\n", id);
	sip_ser = 0;
	pthread_join(_sip, NULL);
	sip_ser = 1;
	pthread_create(&_sip, NULL, handle_sip, sock);
}
void* handle_sip(void *x) {
	printf("handle_sip\n");
	int sock = *((int*) x);
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("Error");
	}
	char buff[1000];
	int n;
	while (sip_ser) {
		n = recv(sock, buff, 1000, MSG_WAITALL);
		if (n > 0) {
			if (buff[n - 1] != '\0')
				buff[n] = '\0';
			if (mess == NULL) {
				asip_m_parseMess(&mess, buff);
				char *type = asip_m_findHeaderValue(mess, "type");
				if (strcmp(type, "invite") == 0) {
					char *from = asip_m_findHeaderValue(mess, "from");
					printf("you has a call from : %s\n", from);
				}
			}
		}
	}
	if (mess != null) {
		asip_m_freeMess(mess);
		mess = null;
	}
	close(sock);
	return null;
}
void accept_call(struct sockaddr_in *ser) {
	if (mess != null) {
		int sockfd = asock_m_UdpSocket();
		struct sockaddr_in *cli = null;
		int err = asock_m_acceptInvite(mess, user, &cli, ser, sockfd);
		if (err > 0) {
			int *x = (int*) malloc(sizeof(int));
			*x = sockfd;
			err = pthread_create(&a_call, null, AudioGet, x); // tao luong nhan
			if (err < 0) {
				free(x);
				return;
			}
			err = pthread_create(&a_send, null, AudioStream, cli); // tao luong gui
			if (err < 0)
				return;
			is_call = 1;

		}
		asip_m_freeMess(mess);
		mess = null;
	}
}
/**
 * huy cuoc goi
 */
void cancel_invite(struct sockaddr_in *ser) {
	if (mess != null) {
		int sockfd = asock_m_UdpSocket();
		asock_m_cancleInvite(mess, user, ser, sockfd);
		close(sockfd);
		asip_m_freeMess(mess);
		mess = null;
	}
}
void* AudioStream(void *x) {
	struct sockaddr_in *ser = (struct sockaddr_in*) x;
	pa_simple *send = NULL;
	int sock = asock_m_UdpSocket();
	if (sock < 0)
		return NULL;
	int error;
	if (!(send = pa_simple_new(NULL, "amneiht", PA_STREAM_RECORD, NULL, "test",
			&format, NULL, NULL, &error))) {
		fprintf(stderr, __FILE__":send: pa_simple_new() failed: %s\n",
				pa_strerror(error));
		close(sock);
		return NULL;
	}
// CREATE RTP HEADER
	artp_header header;
	memset(&header, 0, sizeof(header));
	header.sep = 12;
	header.group = 1;
	header.id = rand() % 1000;
//	init buffer
	int BUFSIZE = 160;
	int16_t buf[BUFSIZE];
	int8_t out[BUFSIZE];
	int size = BUFSIZE * sizeof(int16_t);
	long sizeout = BUFSIZE * sizeof(int8_t);
//	char test[] = "tststs";
	while (run) {
		if (pa_simple_read(send, buf, size, &error) < 0) {
			fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n",
					pa_strerror(error));
			goto finish;
		}
		UlawArrayEncode(buf, BUFSIZE, out);
		artp_sendto(&header, out, sizeout, ser, sock);
//		artp_sendto(&header, userid, strlen(userid), ser, sock);
//		sleep(1);
	}

	finish: pa_simple_free(send);
	close(sock);
	free(ser);
	return NULL;

}
void* AudioGet(void *x) {
	int sockfd = *((int*) x);
	int size = 1000;
	int8_t buff[size];
	int16_t play[size];
	pa_simple *s;
	int error;
	if (!(s = pa_simple_new(NULL, "amneiht", PA_STREAM_PLAYBACK, NULL, "test",
			&format, NULL, NULL, &error))) {
		fprintf(stderr, __FILE__":get pa_simple_new() failed: %s\n",
				pa_strerror(error));
		goto finish;
	}
	artp_header header;
	int i = 0;
	int n;
	while (run) {
		n = artp_recvfrom(&header, buff, sockfd);
		if (n > 0) {
			UlawArrayDecode(buff, n, play);
			if (pa_simple_write(s, play, n * sizeof(int16_t), &error) < 0) {
				fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n",
						pa_strerror(error));
				goto finish;
			}
//		i++;
//		printf("%s nhan data tu :%s lan thu %d \n", user, buff, i);
		}
	}
	finish: if (s != NULL)
		pa_simple_free(s);
	close(sockfd);
	free(x);
	return NULL;
}

int praseParameters(char *str, char **mg) {

	int dem = strlen(str);
	int point = 0;
	int lamp = 0;
	int vt = 0;
	for (int i = 0; i < dem; i++) {
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			if (lamp) {
				mg[vt][point] = '\0';
				vt++;
				point = 0;
				lamp = 0;
			}
		} else {
			lamp = 1;
			mg[vt][point] = str[i];
			point++;
		}
	}
	if (str[dem - 2] != ' ') {
		mg[vt][point] = '\0';
		vt++;
	}
	if (point > 0 && vt == 0)
		vt++;
	return vt;
}
