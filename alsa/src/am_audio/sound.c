/*
 * sound.c
 *
 *  Created on: Sep 3, 2020
 *      Author: dccan
 */

#include <amneiht/am_audio.h>
#include <bits/stdint-intn.h>
#include <stdio.h>

void am_audio_m_close(DataLine *p) {
	snd_pcm_close(p);
}
int am_audio_m_write(DataLine *p, int16_t *buff, int length) {

	return snd_pcm_writei(p, buff, length);
}
int am_audio_m_read(DataLine *p, int16_t *buff, int length) {
	return snd_pcm_readi(p, buff, length);
}
void am_audio_m_g729(int Type, AudioFormat *af) {
	af->handle = Type;
	af->sampleRate = 8000;
	af->access = SND_PCM_ACCESS_RW_INTERLEAVED;
	af->format = SND_PCM_FORMAT_S16_LE;
	af->chanel = 1;
	af->bits = 16;
}
int am_audio_m_openAudio(AudioFormat *af, snd_pcm_t **handle) {
	//TODO
	snd_pcm_uframes_t frames;
	int err;
	unsigned int fragments = 2;
	int buffer_size = 4800;
	int frame_size = af->chanel * (af->bits / 8);
	frames = buffer_size / frame_size * fragments;
	if (buffer_size != frames * frame_size / fragments) {
		buffer_size = frames * frame_size / fragments;
	}

	snd_pcm_hw_params_t *hw_params;
	char  device[] = "plughw:0,0" ;
//	char device[] = "hw:0,0";
	if ((err = snd_pcm_open(handle, device, af->handle, 0)) < 0) {
		fprintf(stderr, "cannot open audio device %s (%s)\n", device,
				snd_strerror(err));
		return -1;
	}
	snd_pcm_t *playback_handle = *handle;
	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
				snd_strerror(err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
		fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
				snd_strerror(err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params,
			af->access)) < 0) {
		fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params,
			af->format)) < 0) {
		fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
		return -1;
	}
	unsigned int rate = af->sampleRate;
	if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params,
			&rate, 0)) < 0) {
		fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params,
			af->chanel)) < 0) {
		fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
		return -1;
	}
	if ((err = snd_pcm_hw_params_set_periods_near(playback_handle, hw_params,
			&fragments, 0)) < 0) {
		fprintf(stderr, "Error setting # fragments to %d: %s\n", fragments,
				snd_strerror(err));
		return -1;
	}
//	if ((err = snd_pcm_hw_params_set_buffer_size_near(playback_handle,
//			hw_params, &frames)) < 0) {
//		fprintf(stderr, "Error setting buffer_size %ld frames: %s\n", frames,
//				snd_strerror(err));
//		return -1;
//	}
	if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
		fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
		return -1;
	}

	snd_pcm_hw_params_free(hw_params);

	if ((err = snd_pcm_prepare(playback_handle)) < 0) {
		fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
				snd_strerror(err));
		return -1;
	}

	return 0;
}

