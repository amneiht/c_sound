/*
 * am_audio.h
 *
 *  Created on: Sep 4, 2020
 *      Author: dccan
 */

#ifndef AM_AUDIO_H_
#define AM_AUDIO_H_

#include <alsa/asoundlib.h>
#include <bits/stdint-intn.h>

#define g729Encoder bcg729EncoderChannelContextStruct
#define g729Decoder bcg729DecoderChannelContextStruct

#define am_audio_playback SND_PCM_STREAM_PLAYBACK
#define am_audio_capture SND_PCM_STREAM_CAPTURE
#define DataLine snd_pcm_t
typedef struct AudioFormat {
	unsigned int sampleRate;
	int access;
	int format;
	int handle;
	int chanel;
	int bits ;
} AudioFormat;
/*
 *   0 dung, khac sai
 */
#ifdef __cplusplus
extern "C" {
#endif
int am_audio_m_openAudio(AudioFormat *af, snd_pcm_t **handle);

void am_audio_m_g729(int Type , AudioFormat * f);

int am_audio_m_read(DataLine *p, int16_t *buff, int length);

int am_audio_m_write(DataLine *p, int16_t *buff, int length);

void am_audio_m_close(DataLine *p);
#ifdef __cplusplus
}
#endif
#endif /* AM_AUDIO_H_ */
