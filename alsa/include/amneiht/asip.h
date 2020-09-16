/*
 * asip.h
 * test ok
 *  Created on: Sep 1, 2020
 *      Author: dccan
 */

#ifndef ASIP_H_
#define ASIP_H_

typedef struct asip_d_mess {
	char *name;
	char *value;
	struct asip_d_mess *next;
} asip_d_mess;

#ifdef __cplusplus
extern "C" {
#endif

#define asip_eos '\0'  // end of string
#define null NULL
#define asip_size 1000;
// cac tieu de mac dinh
extern char *asip_d_Header_Type;
extern char *asip_d_Header_To;
extern char *asip_d_Header_Addr;
extern char *asip_d_Header_From;
extern char *asip_d_Header_Port;
// cac loai ban tin
extern char *asip_d_Type_Regiter;
extern char *asip_d_Type_Invite;
extern char *asip_d_Type_Ack;
extern char *asip_d_Type_Bye;
extern char *asip_d_Type_Cancel;

/**
 *  chuyen doi tin nhan sang cau truc sip
 */
int asip_m_parseMess(asip_d_mess **point, char *mess);
/**
 * giai phong bo nho
 */
void asip_m_freeMess(asip_d_mess *point);
/**
 * tim 1 gia tri header , khong phan biet chu hoa hay chu thuong
 */
char* asip_m_findHeaderValue(asip_d_mess *point, char *test);
/**
 * them header vao sip
 */
void asip_m_addHeader(asip_d_mess **point, char *name, char *value);
/**
 * lay ban tin sip
 */
int asip_m_toStringMess(asip_d_mess *point, int leng, char *mess);

#ifdef __cplusplus
}
#endif

#endif /* ASIP_H_ */
