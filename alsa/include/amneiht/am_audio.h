/*
 * am_audio.h
 *
 *  Created on: Sep 4, 2020
 *      Author: dccan
 */

#ifndef AM_AUDIO_H_
#define AM_AUDIO_H_

#include <alsa/asoundlib.h>

/*
 *   0 dung, khac sai
 */
#ifdef __cplusplus
extern "C" {
#endif
int amaudio_setparams(snd_pcm_t *handle);
int amaudio_set_sw_params(snd_pcm_t *handle);
int amaudio_initPcmSteam(snd_pcm_t **pb, snd_pcm_t **ct);
#ifdef __cplusplus
}
#endif
#endif /* AM_AUDIO_H_ */
