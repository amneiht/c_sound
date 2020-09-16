//#include <stdio.h>
//#include <stdlib.h>
//
//#include "am_audio/am_audio.h"
//int encodeData();
//void openB729();
//int testWrite();
//int main1(int argc, char *argv[]) {
//	testWrite() ;
//	return 10;
//}
//void openB729() {
//	FILE *fp = fopen("/home/dccan/Music/g729.am", "rb");
//	AudioFormat af;
//	am_audio_m_g729(am_audio_playback,&af);
//	DataLine *cap;
//	am_audio_m_openAudio(&af, &cap);
//	g729Decoder *de = initBcg729DecoderChannel();
//	int16_t out[80];
//	uint8_t data[20];
//	int size = sizeof(uint8_t);
//	int read;
//	while ((read = fread(data, size, 10, fp)) > 0) {
//		bcg729Decoder(de, data, 10, 0, 0, 0, out);
//		am_audio_m_write(cap, out, 80);
//	}
//
//	closeBcg729DecoderChannel(de);
//	am_audio_m_close(cap);
//	fclose(fp);
//}
//int encodeData() {
//	FILE *fp = fopen("/home/dccan/Music/audio.am", "rb");
//	FILE *fl = fopen("/home/dccan/Music/g729.am", "wb");
//
//	g729Encoder *en = initBcg729EncoderChannel(2);
//	int read;
//	int16_t buff[8000];
//	uint8_t out[100];
//
//	int size16 = sizeof(int16_t);
//	int size8 = sizeof(uint8_t);
//	uint8_t data;
//	read = fread(buff, size16, 80, fp);
//	while (read > 0) {
//		bcg729Encoder(en, buff, out, &data);
//		fwrite(out, size8, data, fl);
//		read = fread(buff, size16, 80, fp);
//	}
//
//	closeBcg729EncoderChannel(en);
//	fclose(fp);
//	fclose(fl);
//	return 1;
//}
//int testWrite() {
//	FILE *fp = fopen("/home/dccan/Music/audio1.am", "wb");
//	AudioFormat af ;
//	am_audio_m_g729(am_audio_capture,&af);
//	DataLine *cap;
//	am_audio_m_openAudio(&af, &cap);
//	int read;
//	int16_t buff[8000];
//	int size = sizeof(int16_t);
//	for (int i = 0; i < 10; i++) {
//		read = am_audio_m_read(cap, buff, 8000);
//		fwrite(buff, size, read, fp);
//	}
//	am_audio_m_close(cap);
//	fclose(fp);
//	return 1;
//}
