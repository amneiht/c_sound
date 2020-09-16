/*
 * TestThread.c
 *
 *  Created on: Sep 14, 2020
 *      Author: dccan
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* testTt(void *x) {
	int i=5 ;
	while (i) {
		i-- ;
		sleep(1);
		printf("rrr \n");
	}
	return NULL;
}
int mainThreadTest(int argc, char **argv) {
	pthread_t pt;
	pthread_create(&pt, NULL, testTt, NULL);
	pthread_join(pt, NULL);
	pthread_join(pt, NULL);
	return 12;
}

