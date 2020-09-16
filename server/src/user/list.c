/*
 * list.c
 *
 *  Created on: Sep 3, 2020
 *      Author: dccan
 */

#include <bits/stdint-intn.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../support/asupport.h"
#include "auser.h"

int64_t live() {
	struct timeval time;
	gettimeofday(&time, NULL);
	int64_t s1 = (int64_t) (time.tv_sec);
	return s1;
}
void auser_m_removeTimeout(auser_d_user **point) {
	long time = live();
	auser_d_user *p, *pn;
	p = *point;
	pn = NULL; // gan gia tri
	while (p != NULL) {
		if ((time - p->live) > auser_Timeout) {
			if (pn != NULL) {
				pn->next = p->next; // noi con tro
				free(p);
				p = pn->next;
			} else {
				pn = p;
				p = p->next;
				free(pn);
				pn = NULL;
				*point = p;
			}
		} else {
			pn = p;
			p = p->next;
		}
	}
}
void auser_m_addUser(auser_d_user **point, struct sockaddr_in *info, char *id,
		char *name) {
	if (*point == NULL) {
		*point = (auser_d_user*) calloc(1, sizeof(auser_d_user));
		auser_d_user *pt = *point;
		pt->id = asup_m_convertPointer(id);
		pt->name = asup_m_convertPointer(name);
		pt->next = NULL;
		pt->ip = info->sin_addr.s_addr;
		pt->port = info->sin_port;
		pt->next = NULL;
		pt->live = live(); // chua ro kieu du lieu
	} else {
		auser_d_user *pt = *point;
		while (pt->next != NULL) {
			pt = pt->next;
		}
		pt->next = (auser_d_user*) calloc(1, sizeof(auser_d_user));
		pt = pt->next;
		pt->id = asup_m_convertPointer(id);
		pt->name = asup_m_convertPointer(name);
		pt->next = NULL;
		pt->ip = info->sin_addr.s_addr;
		pt->port = info->sin_port;
		pt->next = NULL;
		pt->live = live();
	}
}
void auser_m_freeUser(auser_d_user *pt) {
	auser_d_user*ps;
	while (pt != NULL) {
		free(pt->id);
		free(pt->name);
		ps = pt->next;
		free(pt);
		pt = ps;
	}

}
auser_d_user* auser_m_getUser(auser_d_user *pt, char *id) {

	while (pt != NULL) {
		if (strcmp(pt->id, id) == 0) {
			return pt;
		}
		pt = pt->next;
	}
	return NULL;
}
auser_d_user** auser_m_listUser(auser_d_user *point, char *name, int *size) {
	auser_d_user **res;
	auser_d_user *pt = point;
	int dem = 0;
	while (pt != NULL) {
		if (strcmp(pt->name, name) == 0) {
			dem++;
		}
		pt = pt->next;
	}
	if (dem > 0) {
//		auser_d_user* res[dem];
		pt = point;
		res = (auser_d_user**) malloc(dem * sizeof(auser_d_user*));
		int i = 0;
		while (pt != NULL) {
			if (strcmp(pt->name, name) == 0) {
				res[i] = pt;
			}
			pt = pt->next;
		}
		*size = dem;
		return res;
	} else {
		*size = dem;
		return NULL;
	}
}
