/*
 * support.c
 *
 *  Created on: Sep 3, 2020
 *      Author: dccan
 */

#include <stdlib.h>
#include <string.h>
#include <amneiht/asupport.h>
void** asup_m_initMarix(int row, int col, int size) {
	void **res;
	res = (void**) malloc(row * sizeof(void*));
	for (int i = 0; i < row; i++) {
		res[i] = (void*) malloc(col * size);
	}
	return res;
}
void asup_m_freeMatrix(void **in, int row) {
	for (int i = 0; i < row; i++) {
		free(in[i]);
	}
	free(in);
}
char* asup_m_convertPointer(char *name) {
	int i, lengname = strlen(name);
	char *header;
	header = (char*) malloc(sizeof(char) * (lengname + 1));
	for (i = 0; i < lengname; i++) {
		header[i] = name[i];
	}
	header[lengname] = '\0';
	return header;
}
char* asup_m_trim(char *str) {
	int lg = strlen(str);
	int i, j;
	for (i = 0; i < lg; i++) {
		if (str[i] != ' ')
			break;
	}
	for (j = lg - 1; j > -1; j--) {
		if (str[j] != ' ')
			break;
	}
	char *res;
	if (i > j) {
		res = (char*) malloc(1 * sizeof(char));
		res[0] = '\0';
		return res;
	}
	int leng = j - i + 2;
	int h = i;
	res = (char*) malloc(leng * sizeof(char));
	for (; i <= j; i++) {
		res[i - h] = str[i];
	}
	res[leng - 1] = (char) '\0';
	return res;
}
int asup_m_find(char *str, char s) {
	int i = 0;
	int lg = strlen(str);
	for (; i < lg; i++) {
		if (str[i] == s) {
			return i;
		}
	}
	return -1;
}

