/*
 * TestRtp.c
 *
 *  Created on: Sep 11, 2020
 *      Author: dccan
 */

#include <amneiht/rtp.h>
#include <stdio.h>
#include <string.h>

int mainRtp(int argc, char **argv) {
	artp_header h;
	char buff[] = "teststst";
	char de[100];
	char en[100];
	int n = artp_createPacket(buff, en, strlen(buff) + 1, &h);
	artp_extractPacket(en, de, n, NULL);
	printf("%s", de);
	return 0;
}

