/*
 * auser.h
 *
 *  Created on: Sep 3, 2020
 *      Author: dccan
 */
#ifndef AUSER_H_
#define AUSER_H_

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>

#define auser_Timeout 5/* time out 50s */
typedef struct auser_d_user {
	char *name;
	char *id;
	uint32_t ip;
	uint16_t port;
	int64_t live;
	struct auser_d_user *next;
} auser_d_user;
#ifdef __cplusplus
extern "C" {
#endif

void auser_m_addUser(auser_d_user **point, struct sockaddr_in *info, char *id,
		char *name); // ok

void auser_m_freeUser(auser_d_user *point); // ok

void auser_m_removeTimeout(auser_d_user **point); // ok

auser_d_user** auser_m_listUser(auser_d_user *point, char *name, int *size); // ok

auser_d_user* auser_m_getUser(auser_d_user *point, char *id); //ok
#ifdef __cplusplus
}
#endif
#endif /* AUSER_H_ */
