/*
 * TestPluse.c
 *
 *  Created on: Sep 13, 2020
 *      Author: dccan
 */

#include <bits/stdint-intn.h>
#include <pulse/error.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int mainPulse(int argc, char **argv) {
	pa_simple *s = NULL, *cap = NULL;
	pa_sample_spec ss;
	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 2;
	ss.rate = 44100;
	int error;
	int test = 0;

	if (!(s = pa_simple_new(NULL, "amneiht", PA_STREAM_PLAYBACK, NULL, "test",
			&ss, NULL, NULL, &error))) {
		fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n",
				pa_strerror(error));
		test = 1;
	}
	if (!(cap = pa_simple_new(NULL, "amneiht", PA_STREAM_RECORD, NULL, "record",
			&ss, NULL, NULL, &error))) {
		fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n",
				pa_strerror(error));
		test = 1;
	}
	if (!test) {
		int BUFSIZE = 10;
		int16_t buf[BUFSIZE];
		int16_t *b2 ;
		b2 = calloc(BUFSIZE,sizeof(int16_t));
		size_t r;
		printf("size of buff:%ld \n",sizeof(b2));
		for (int i = 0; i < 1000; i++) {
			if ((r = pa_simple_read(cap, buf, BUFSIZE, &error)) < 0) {
				fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n",
						pa_strerror(error));
				goto finish;
			}
			printf("size of %ld \n",r);
			if (pa_simple_write(s, buf,  BUFSIZE, &error) < 0) {
				fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n",
						pa_strerror(error));
				goto finish;
			}
		}
		pa_simple_drain(cap,NULL);
		if ((r = pa_simple_read(cap, b2, BUFSIZE, &error)) < 0) {
			fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n",
					pa_strerror(error));
			goto finish;
		}
		for(int i=0;i<BUFSIZE;i++)
			printf("%d ",b2[i]);
		free(b2);
	}

	finish: if (s)
		pa_simple_free(s);
	if (cap)
		pa_simple_free(cap);

	return 0;
}
