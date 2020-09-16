/*
 * am_sock.h
 *
 *  Created on: Sep 7, 2020
 *      Author: dccan
 */

#ifndef ASOCKET_AM_SOCK_H_
#define ASOCKET_AM_SOCK_H_

#include <bits/stdint-uintn.h>
#include <netinet/in.h>
#include "asip.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * chap nhan yeu cau goi
 * name : ten nguoi dung
 * invite : ban tin moi
 * ser : thong tin ser
 * cliaddr : noi chua thong tin nguoi goi
 */
int asock_m_acceptInvite(asip_d_mess *invite, char *name,
		struct sockaddr_in **cliaddr, struct sockaddr_in *ser, int sockfd);

int asock_m_cancleInvite(asip_d_mess *invite, char *name,
		struct sockaddr_in *ser, int sockfd);

int asock_m_Invite(char *callee, char *caller, char *id,
		 struct sockaddr_in **cli,struct sockaddr_in *ser, int sockfd) ;

int asock_m_register(char *name, char *out, struct sockaddr_in *ser, int sockfd);

uint32_t asock_m_hostToIp(char *hostname);
int asock_m_UdpServerSocket(int port);
int asock_m_UdpSocket();
#ifdef __cplusplus
}
#endif
#endif /* ASOCKET_AM_SOCK_H_ */
