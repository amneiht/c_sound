/*
 * send.c
 *
 *  Created on: Sep 9, 2020
 *      Author: dccan
 */

#include "amneiht/rtp.h"
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

int64_t live() {
	struct timeval time;
	gettimeofday(&time, NULL);
	int64_t s1 = (int64_t) (time.tv_sec);
	return s1;
}

int artp_createPacket(void *data, void *out, long size, artp_header *header) {
	static const int length = sizeof(artp_header);
	header->sep++; // tang moi lan gui
	header->timestamp = (uint32_t) live();
	uint8_t *get = (uint8_t*) header;
	uint8_t *res = (uint8_t*) out;
	uint8_t *in = (uint8_t*) data;
	for (int i = 0; i < length; i++) {
		res[i] = get[i];
	}
	for (int i = 0; i < size; i++) {
		res[i + length] = in[i];
	}
	return size + length;
}
int artp_sendto(artp_header *header, void *input, long size,
		struct sockaddr_in *ser, int sockfd) {
	static char buff[1000];
	static const socklen_t len = sizeof(struct sockaddr_in);
	int n = artp_createPacket(input, buff, size, header);
	int err = sendto(sockfd, buff, n, MSG_CONFIRM, (struct sockaddr*) ser, len);
	if (err < 0) {
		printf("loi gui data\n");
	}
	return err;
}
int artp_recvfrom(artp_header *header, void *out, int sockfd) {
	static char buff[500];
	static const int lgs = sizeof(artp_header);
	int n = recv(sockfd, buff, 500, MSG_WAITALL);
	if (n < lgs)
		return -1;
	n = artp_extractPacket(buff, out, n, header);
	char *ls = (char*) out;
	if (ls[n - 1] != '\0')
		ls[n] = '\0';
	return n;
}
int artp_extractPacket(void *input, void *output, long size,
		artp_header *header) {
	static const int length = sizeof(artp_header);
	uint8_t *get = (uint8_t*) header;
	uint8_t *out = (uint8_t*) output;
	uint8_t *in = (uint8_t*) input;
	if (header != NULL)
		for (int i = 0; i < length; i++) {
			get[i] = out[i];
		}
	for (int i = 0; i < size; i++) {
		out[i] = in[i + length];
	}
	return size - length;
}
