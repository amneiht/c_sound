/*
 * g711Array.c
 *
 *  Created on: Sep 8, 2020
 *      Author: dccan
 */
#include "amneiht/g711.h"

void UlawArrayEncode(int16_t *in, int size, int8_t *out) {
	for (int i = 0; i < size; i++) {
		out[i] = MuLaw_Encode(in[i]);
	}
}
void UlawArrayDecode(int8_t *in, int size, int16_t *out) {
	for (int i = 0; i < size; i++) {
		out[i] = MuLaw_Decode(in[i]);
	}
}
void AlawArrayEncode(int16_t *in, int size, int8_t *out) {
	for (int i = 0; i < size; i++) {
		out[i] = ALaw_Encode(in[i]);
	}
}
void AlawArrayDecode(int8_t *in, int size, int16_t *out) {
	for (int i = 0; i < size; i++) {
		out[i] = ALaw_Decode(in[i]);
	}
}
