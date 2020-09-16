/*
 * testSound.c
 *
 *  Created on: Sep 12, 2020
 *      Author: dccan
 */

#include <amneiht/am_audio.h>
#include <amneiht/g711.h>
#include <bits/stdint-intn.h>

int mainSoubs(int argc, char **argv) {
	int16_t leng = 160;
	int16_t buff[leng];
	AudioFormat af;
	am_audio_m_g729(am_audio_capture, &af);
	DataLine *in;
	am_audio_m_openAudio(&af, &in);
	am_audio_m_g729(am_audio_playback, &af);
	DataLine *out;
	am_audio_m_openAudio(&af, &out);

	for (int i = 0; i < 1000; i++) {
		am_audio_m_read(in, buff, leng);
		am_audio_m_write(out, buff, leng);
	}

	am_audio_m_close(in);
	am_audio_m_close(out);
	return 0;
}
