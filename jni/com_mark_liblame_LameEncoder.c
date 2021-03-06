/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "lame.h"
/* Header for class com_mark_liblame_LameEncoder */

#define LOG_TAG "LAME ENCODER BY MARK."
#define LOGD(format, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##args);
#define BUFFER_SIZE 8192

// Convert type.
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))

lame_t lame;	// lame_t is lame_global_flags.

#ifdef __cplusplus
extern "C" {
#endif


	int lame_read_samples(FILE *input_file, short *input)
	{
		int nb_read;
		nb_read = fread(input, 1, sizeof(short), input_file) / sizeof(short);

		int i = 0;
		while (i < nb_read) {
			input[i] = be_short(input[i]);
			i++;
		}

		return nb_read;
	}
/*
 * Class:     com_mark_liblame_LameEncoder
 * Method:    lameInit
 * Signature: (IIIII)I
 */
 JNIEXPORT jint JNICALL Java_com_mark_liblame_LameEncoder_lameInit
 (JNIEnv *env, jobject obj, jint in_sample_rate, jint in_channel, jint in_bit_rate, jint mode, jint quality)
 {
 	lame = lame_init();

 	LOGD("Init parameters:");
 	lame_set_num_channels(lame, in_channel);
 	LOGD("Number of channels: %d", in_channel);
 	lame_set_in_samplerate(lame, in_sample_rate);
 	LOGD("Sample rate: %d", in_sample_rate);
 	lame_set_brate(lame, in_bit_rate);
 	LOGD("Bitrate: %d", in_bit_rate);
 	lame_set_mode(lame, mode);
 	LOGD("Mode: %d", mode);
 	lame_set_quality(lame, quality);
 	LOGD("Quality: %d", quality);

 	int res = lame_init_params(lame);
 	LOGD("Init returned: %d", res);

 }

/*
 * Class:     com_mark_liblame_LameEncoder
 * Method:    lameEncode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
 JNIEXPORT jint JNICALL Java_com_mark_liblame_LameEncoder_lameEncode
 (JNIEnv *env, jobject obj, jstring src_file_path, jstring target_file_path)
 {
 	const char *source_path, *target_path;
 	source_path = (*env)->GetStringUTFChars(env, src_file_path, NULL);
 	target_path = (*env)->GetStringUTFChars(env, target_file_path, NULL);

 	FILE *input_file, *output_file;
 	input_file = fopen(source_path, "rb");
 	output_file = fopen(target_path, "wb");

 	short input[BUFFER_SIZE];
 	char output[BUFFER_SIZE];
 	int nb_read = 0;
 	int nb_write = 0;
 	int nb_total = 0;

 	LOGD("Encoding started");
 	while (nb_read = lame_read_samples(input_file, input)) {
 		nb_write = lame_encode_buffer(lame, input, input, nb_read, output,
 			BUFFER_SIZE);
 		fwrite(output, nb_write, 1, output_file);
 		nb_total += nb_write;
 	}
 	LOGD("Encoded %d bytes", nb_total);

 	nb_write = lame_encode_flush(lame, output, BUFFER_SIZE);
 	fwrite(output, nb_write, 1, output_file);
 	LOGD("Flushed %d bytes", nb_write);

 	fclose(input_file);
 	fclose(output_file);
 }

/*
 * Class:     com_mark_liblame_LameEncoder
 * Method:    close
 * Signature: (I)V
 */
 JNIEXPORT void JNICALL Java_com_mark_liblame_LameEncoder_lameClose
 (JNIEnv *env, jobject obj)
 {
 	int result = lame_close(lame);
 	LOGD("Lame close, return %d", result);
 }

#ifdef __cplusplus
}
#endif
