/*
 * rtp.h
 *
 *  Created on: Sep 9, 2020
 *      Author: dccan
 */

#ifndef ASOCKET_RTP_RTP_H_
#define ASOCKET_RTP_RTP_H_

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <netinet/in.h>

typedef struct artp_header {
	uint8_t info;
	uint8_t playload;
	uint16_t sep;
	uint32_t timestamp;
	int32_t id;  // csrc
	int32_t group; //ssrc
} artp_header;
//void setVersion(int x, artp_header *d);

int artp_createPacket(void *input, void *out, long size, artp_header *header);
int artp_extractPacket(void *input, void *output, long size,
		artp_header *header);
int artp_sendto(artp_header *header, void *input, long size,
		struct sockaddr_in *ser, int sockfd);
int artp_recvfrom(artp_header *header, void *out, int sockfd);
#endif /* ASOCKET_RTP_RTP_H_ */
