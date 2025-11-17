/* Simple SILK-only encoding/decoding demonstration
 * This program demonstrates encoding and decoding speech using Opus SILK-only mode.
 *
 * Usage: ./silk_demo <sample_rate> <channels> <bitrate>
 * Example: ./silk_demo 16000 1 16000
 *
 * The program:
 * 1. Generates a simple test signal (sine wave)
 * 2. Encodes it using Opus in SILK-only mode
 * 3. Decodes the encoded audio
 * 4. Writes the decoded output to a file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opus.h>

#define FRAME_SIZE_MS 20
#define MAX_PACKET_SIZE 1500

int main(int argc, char *argv[])
{
    int sample_rate = 16000;
    int channels = 1;
    int bitrate = 16000;
    int frame_size;
    int err;
    int i, j;

    OpusEncoder *encoder;
    OpusDecoder *decoder;

    opus_int16 *input_pcm;
    opus_int16 *output_pcm;
    unsigned char packet[MAX_PACKET_SIZE];
    int packet_size;

    FILE *fout;

    /* Parse command line arguments */
    if (argc >= 2) {
        sample_rate = atoi(argv[1]);
    }
    if (argc >= 3) {
        channels = atoi(argv[2]);
    }
    if (argc >= 4) {
        bitrate = atoi(argv[3]);
    }

    /* Validate parameters */
    if (sample_rate != 8000 && sample_rate != 12000 &&
        sample_rate != 16000 && sample_rate != 24000) {
        fprintf(stderr, "Error: Sample rate must be 8000, 12000, 16000, or 24000 Hz\n");
        return 1;
    }

    if (channels < 1 || channels > 2) {
        fprintf(stderr, "Error: Channels must be 1 (mono) or 2 (stereo)\n");
        return 1;
    }

    printf("SILK-only Opus Demo\n");
    printf("===================\n");
    printf("Sample rate: %d Hz\n", sample_rate);
    printf("Channels: %d\n", channels);
    printf("Bitrate: %d bps\n", bitrate);
    printf("\n");

    /* Calculate frame size in samples */
    frame_size = (sample_rate * FRAME_SIZE_MS) / 1000;
    printf("Frame size: %d samples (%d ms)\n", frame_size, FRAME_SIZE_MS);

    /* Allocate buffers */
    input_pcm = (opus_int16 *)malloc(frame_size * channels * sizeof(opus_int16));
    output_pcm = (opus_int16 *)malloc(frame_size * channels * sizeof(opus_int16));

    if (!input_pcm || !output_pcm) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        return 1;
    }

    /* Create encoder */
    encoder = opus_encoder_create(sample_rate, channels, OPUS_APPLICATION_VOIP, &err);
    if (err != OPUS_OK) {
        fprintf(stderr, "Error: Failed to create encoder: %s\n", opus_strerror(err));
        return 1;
    }
    printf("Encoder created successfully\n");

    /* Configure encoder */
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));

#ifdef ENABLE_SILK_ONLY
    printf("SILK-only build detected - encoder will use SILK mode only\n");
#else
    /* Force SILK-only mode in regular builds */
    opus_encoder_ctl(encoder, OPUS_SET_MAX_BANDWIDTH(OPUS_BANDWIDTH_WIDEBAND));
    opus_encoder_ctl(encoder, OPUS_SET_FORCE_CHANNELS(channels));
    printf("Regular build - forcing SILK mode by limiting bandwidth\n");
#endif

    /* Create decoder */
    decoder = opus_decoder_create(sample_rate, channels, &err);
    if (err != OPUS_OK) {
        fprintf(stderr, "Error: Failed to create decoder: %s\n", opus_strerror(err));
        opus_encoder_destroy(encoder);
        return 1;
    }
    printf("Decoder created successfully\n");
    printf("\n");

    /* Open output file */
    fout = fopen("silk_demo_output.raw", "wb");
    if (!fout) {
        fprintf(stderr, "Error: Could not open output file\n");
        opus_encoder_destroy(encoder);
        opus_decoder_destroy(decoder);
        return 1;
    }

    /* Generate and process 5 seconds of audio (test signal: 440 Hz sine wave) */
    int total_frames = (5 * sample_rate) / frame_size;
    printf("Processing %d frames (5 seconds)...\n", total_frames);

    for (i = 0; i < total_frames; i++) {
        /* Generate test signal: 440 Hz sine wave */
        for (j = 0; j < frame_size; j++) {
            double t = (double)(i * frame_size + j) / sample_rate;
            double sample = 10000.0 * sin(2.0 * 3.14159265359 * 440.0 * t);

            for (int ch = 0; ch < channels; ch++) {
                input_pcm[j * channels + ch] = (opus_int16)sample;
            }
        }

        /* Encode */
        packet_size = opus_encode(encoder, input_pcm, frame_size, packet, MAX_PACKET_SIZE);
        if (packet_size < 0) {
            fprintf(stderr, "Error: Encoding failed: %s\n", opus_strerror(packet_size));
            break;
        }

        /* Decode */
        int decoded_samples = opus_decode(decoder, packet, packet_size, output_pcm, frame_size, 0);
        if (decoded_samples < 0) {
            fprintf(stderr, "Error: Decoding failed: %s\n", opus_strerror(decoded_samples));
            break;
        }

        /* Write decoded audio to file */
        fwrite(output_pcm, sizeof(opus_int16), decoded_samples * channels, fout);

        /* Progress indicator */
        if (i % 50 == 0) {
            printf(".");
            fflush(stdout);
        }
    }

    printf("\nDone!\n");
    printf("\nEncoded and decoded %d frames\n", total_frames);
    printf("Average packet size: ~%d bytes\n", packet_size);
    printf("Output written to: silk_demo_output.raw\n");
    printf("\nTo play the output with ffplay:\n");
    printf("  ffplay -f s16le -ar %d -ac %d silk_demo_output.raw\n", sample_rate, channels);

    /* Cleanup */
    fclose(fout);
    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);
    free(input_pcm);
    free(output_pcm);

    return 0;
}
