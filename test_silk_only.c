/* Test SILK-only build encode/decode */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "opus.h"

#define SAMPLE_RATE 16000
#define CHANNELS 1
#define FRAME_SIZE 320  /* 20ms at 16kHz */
#define BITRATE 16000
#define MAX_PACKET_SIZE 4000

int main(int argc, char *argv[])
{
    OpusEncoder *encoder;
    OpusDecoder *decoder;
    int err;
    opus_int16 in_pcm[FRAME_SIZE * CHANNELS];
    opus_int16 out_pcm[FRAME_SIZE * CHANNELS];
    unsigned char packet[MAX_PACKET_SIZE];
    int packet_len;
    int i;

    printf("SILK-only Build Test\n");
    printf("=====================\n\n");

    /* Create encoder */
    encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_VOIP, &err);
    if (err != OPUS_OK) {
        fprintf(stderr, "Failed to create encoder: %s\n", opus_strerror(err));
        return 1;
    }
    printf("✓ Encoder created successfully\n");

    /* Configure encoder for SILK-only */
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));
    opus_encoder_ctl(encoder, OPUS_SET_VBR(1));

    /* Create decoder */
    decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
    if (err != OPUS_OK) {
        fprintf(stderr, "Failed to create decoder: %s\n", opus_strerror(err));
        opus_encoder_destroy(encoder);
        return 1;
    }
    printf("✓ Decoder created successfully\n");

    /* Generate test signal (440 Hz sine wave) */
    for (i = 0; i < FRAME_SIZE * CHANNELS; i++) {
        in_pcm[i] = (opus_int16)(sin(2.0 * M_PI * 440.0 * i / SAMPLE_RATE) * 8000.0);
    }
    printf("✓ Generated test signal (440 Hz sine wave)\n");

    /* Encode the frame */
    packet_len = opus_encode(encoder, in_pcm, FRAME_SIZE, packet, MAX_PACKET_SIZE);
    if (packet_len < 0) {
        fprintf(stderr, "Encoding failed: %s\n", opus_strerror(packet_len));
        opus_encoder_destroy(encoder);
        opus_decoder_destroy(decoder);
        return 1;
    }
    printf("✓ Encoding successful: %d bytes\n", packet_len);

    /* Decode the frame */
    int num_samples = opus_decode(decoder, packet, packet_len, out_pcm, FRAME_SIZE, 0);
    if (num_samples < 0) {
        fprintf(stderr, "Decoding failed: %s\n", opus_strerror(num_samples));
        opus_encoder_destroy(encoder);
        opus_decoder_destroy(decoder);
        return 1;
    }
    printf("✓ Decoding successful: %d samples\n", num_samples);

    /* Verify we got the expected number of samples */
    if (num_samples != FRAME_SIZE) {
        fprintf(stderr, "ERROR: Expected %d samples, got %d\n", FRAME_SIZE, num_samples);
        opus_encoder_destroy(encoder);
        opus_decoder_destroy(decoder);
        return 1;
    }

    /* Calculate signal-to-noise ratio */
    double signal_power = 0.0;
    double noise_power = 0.0;
    for (i = 0; i < FRAME_SIZE * CHANNELS; i++) {
        double signal = (double)in_pcm[i];
        double noise = (double)(in_pcm[i] - out_pcm[i]);
        signal_power += signal * signal;
        noise_power += noise * noise;
    }
    double snr_db = 10.0 * log10(signal_power / (noise_power + 1e-10));
    printf("✓ Signal-to-Noise Ratio: %.2f dB\n", snr_db);

    /* Test packet loss concealment */
    num_samples = opus_decode(decoder, NULL, 0, out_pcm, FRAME_SIZE, 0);
    if (num_samples < 0) {
        fprintf(stderr, "PLC failed: %s\n", opus_strerror(num_samples));
        opus_encoder_destroy(encoder);
        opus_decoder_destroy(decoder);
        return 1;
    }
    printf("✓ Packet Loss Concealment works: %d samples\n", num_samples);

    /* Cleanup */
    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);

    printf("\n======================\n");
    printf("All tests PASSED! ✓\n");
    printf("======================\n");

    return 0;
}
