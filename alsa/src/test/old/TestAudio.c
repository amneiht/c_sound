/*
 * TestAudio.c
 *
 *  Created on: Sep 11, 2020
 *      Author: dccan
 */

#include <amneiht/am_audio.h>
#include <amneiht/g711.h>
#include <bits/stdint-intn.h>

int mainTAudio(int argc, char **argv) {
	int16_t leng = 8000;
	int16_t buff[leng];
	int16_t de[leng];
	int8_t en[leng];
	AudioFormat af;
	am_audio_m_g729(am_audio_capture, &af);
	DataLine *in;

	am_audio_m_g729(am_audio_playback, &af);
	DataLine *out;
	for (int i = 0; i < 10; i++) {
		am_audio_m_openAudio(&af, &in);
		am_audio_m_read(in, buff, leng);
		UlawArrayEncode(buff, leng, en);
		am_audio_m_openAudio(&af, &out);
		UlawArrayDecode(en, leng, de);

		am_audio_m_write(out, de, leng);
	}
	am_audio_m_close(in);
	am_audio_m_close(out);
	return 0;
}

