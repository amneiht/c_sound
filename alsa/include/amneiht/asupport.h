/*
 * asupport.h
 *	test ok
 *  Created on: Sep 3, 2020
 *      Author: dccan
 */

#ifndef ASUPPORT_H_
#define ASUPPORT_H_

#ifdef __cplusplus
extern "C" {
#endif
char* asup_m_trim(char *str);
int asup_m_find(char *str, char s);
char* asup_m_convertPointer(char *name);
void** asup_m_initMarix(int row, int col, int size);
void asup_m_freeMatrix(void **in, int row) ;
#ifdef __cplusplus
}
#endif

#endif /* ASUPPORT_H_ */
