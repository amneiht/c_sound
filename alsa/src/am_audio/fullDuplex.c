/**
 * Jan Newmarch
 */
#define PERIOD_SIZE 1024
#define BUF_SIZE (PERIOD_SIZE * 2)
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <amneiht/am_audio.h>
int amaudio_setparams(snd_pcm_t *handle) {
	snd_pcm_hw_params_t *hw_params;
	int err;
	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
				snd_strerror(err));
		exit(1);
	}
	if ((err = snd_pcm_hw_params_any(handle, hw_params)) < 0) {
		fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
				snd_strerror(err));
		exit(1);
	}
	if ((err = snd_pcm_hw_params_set_access(handle, hw_params,
			SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
		exit(1);
	}
	if ((err = snd_pcm_hw_params_set_format(handle, hw_params,
			SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
		exit(1);
	}
	unsigned int rate = 8000;
	if ((err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &rate, 0))
			< 0) {
		fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
		exit(1);
	}
	printf("Rate is %d\n", rate);
	if ((err = snd_pcm_hw_params_set_channels(handle, hw_params, 2)) < 0) {
		fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
		exit(1);
	}
	snd_pcm_uframes_t buffersize = BUF_SIZE;
	if ((err = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params,
			&buffersize)) < 0) {
		printf("Unable to set buffer size %i: %s\n", BUF_SIZE,
				snd_strerror(err));
		exit(1);
		;
	}
	snd_pcm_uframes_t periodsize = PERIOD_SIZE;
	fprintf(stderr, "period size now %ld \n", periodsize);
	if ((err = snd_pcm_hw_params_set_period_size_near(handle, hw_params,
			&periodsize, 0)) < 0) {
		printf("Unable to set period size %li: %s\n", periodsize,
				snd_strerror(err));
		exit(1);
	}
	if ((err = snd_pcm_hw_params(handle, hw_params)) < 0) {
		fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
		exit(1);
	}
	snd_pcm_uframes_t p_psize;
	snd_pcm_hw_params_get_period_size(hw_params, &p_psize, NULL);
	fprintf(stderr, "period size %lu\n", p_psize);
	snd_pcm_hw_params_get_buffer_size(hw_params, &p_psize);
	fprintf(stderr, "buffer size %lu\n", p_psize);
	snd_pcm_hw_params_free(hw_params);
	if ((err = snd_pcm_prepare(handle)) < 0) {
		fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
				snd_strerror(err));
		exit(1);
	}
	return 0;
}
int amaudio_set_sw_params(snd_pcm_t *handle) {
	snd_pcm_sw_params_t *swparams;
	int err;
	snd_pcm_sw_params_alloca(&swparams);
	err = snd_pcm_sw_params_current(handle, swparams);
	if (err < 0) {
		fprintf(stderr,
				"Broken configuration for this PCM: no configurations available\n");
		exit(1);
	}
	err = snd_pcm_sw_params_set_start_threshold(handle, swparams, PERIOD_SIZE);
	if (err < 0) {
		printf("Unable to set start threshold: %s\n", snd_strerror(err));
		return err;
	}
	err = snd_pcm_sw_params_set_avail_min(handle, swparams, PERIOD_SIZE);
	if (err < 0) {
		printf("Unable to set avail min: %s\n", snd_strerror(err));
		return err;
	}
	if (snd_pcm_sw_params(handle, swparams) < 0) {
		fprintf(stderr, "unable to install sw params:\n");
		exit(1);
	}
	return 0;
}
int amaudio_initPcmSteam(snd_pcm_t **pb, snd_pcm_t **ct) {
	int err;
	char device[] = "hw:0";
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
	int buf[BUF_SIZE];
	/**** Out card *******/
	if ((err = snd_pcm_open(pb, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf(stderr, "cannot open audio device %s (%s)\n", device,
				snd_strerror(err));
		exit(1);
	}
	/*********** In card **********/
	if ((err = snd_pcm_open(ct, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf(stderr, "cannot open audio device %s (%s)\n", device,
				snd_strerror(err));
		exit(1);
	}
	amaudio_setparams(*pb);
	amaudio_set_sw_params(*pb);

	amaudio_setparams(*ct);
	amaudio_set_sw_params(*ct);
	if ((err = snd_pcm_link(*ct, *pb)) < 0) {
		printf("Streams link error: %s\n", snd_strerror(err));
		exit(0);
	}

	if ((err = snd_pcm_prepare(*pb)) < 0) {
		fprintf(stderr,
				"cannot prepare playback audio interface for use (%s)\n",
				snd_strerror(err));
		exit(1);
	}
	/**************** stuff something into the playback buffer ****************/
	if (snd_pcm_format_set_silence(format, buf, 2 * BUF_SIZE) < 0) {
		fprintf(stderr, "silence error\n");
		exit(1);
	}
	int n = 0;
	while (n++ < 2) {
		if (snd_pcm_writei(*pb, buf, BUF_SIZE) < 0) {
			fprintf(stderr, "write error\n");
			exit(1);
		}
	}

	return 1;
}
