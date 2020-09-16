/*
 * Testalsa.c
 *
 *  Created on: Sep 13, 2020
 *      Author: dccan
 */

#include <alsa/asoundlib.h>
#include <stdio.h>

#define MAX_THREADS 10
static const char *devname = "hw:0,0";

static int periodsize = 16 * 1024;
static int bufsize = 16 * 1024 * 4;
static int channels = 2;
static int rate = 48000;
static snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
int setup_params(snd_pcm_t *pcm);
int mainTestALsa(int argc, char **argv) {
	snd_pcm_t *cap, *play;
	int err;

	err = snd_pcm_open(&cap, devname, SND_PCM_STREAM_CAPTURE, 0);
	printf("pcm icap %d \n", err);
	if (err < 0) {
		return -1;
	}

	err = snd_pcm_open(&play, devname, SND_PCM_STREAM_PLAYBACK, 0);
	if (err < 0) {
		return -1;
	}
	printf("pcm play %d \n", err);
	setup_params(cap);
	setup_params(play);
	int size = 1000;
	char buf[size];
	for (int i = 0; i < 100; i++) {
		err = snd_pcm_readi(cap, buf, size);
		printf("ssss %d \n", err);
		if (err > 0) {
			snd_pcm_writei(play, buf, err);
		} else {
			printf("ssss \n");
		}

	}
	snd_pcm_close(play);
	snd_pcm_close(cap);
	return 0;
}

int setup_params(snd_pcm_t *pcm) {
	snd_pcm_hw_params_t *hw;

	/* FIXME: more finer error checks */
	snd_pcm_hw_params_alloca(&hw);
	snd_pcm_hw_params_any(pcm, hw);
	snd_pcm_hw_params_set_access(pcm, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(pcm, hw, format);
	snd_pcm_hw_params_set_channels(pcm, hw, channels);
	snd_pcm_hw_params_set_rate(pcm, hw, rate, 0);
	snd_pcm_hw_params_set_period_size(pcm, hw, periodsize, 0);
	snd_pcm_hw_params_set_buffer_size(pcm, hw, bufsize);

	if (snd_pcm_hw_params(pcm, hw) < 0) {
		fprintf(stderr, "snd_pcm_hw_params error\n");
		return -1;
	}
//	snd_pcm_hw_params_free(hw);
	return 0;
}
