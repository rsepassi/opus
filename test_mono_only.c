/* Test that mono-only build rejects stereo */
#include <stdio.h>
#include <stdlib.h>
#include "opus.h"

int main(void)
{
    OpusEncoder *encoder;
    OpusDecoder *decoder;
    int error;

    printf("Testing mono-only build restrictions\n");
    printf("=====================================\n\n");

    /* Test 1: Mono should work */
    printf("Test 1: Creating mono encoder... ");
    encoder = opus_encoder_create(16000, 1, OPUS_APPLICATION_VOIP, &error);
    if (error == OPUS_OK && encoder != NULL) {
        printf("✓ SUCCESS\n");
        opus_encoder_destroy(encoder);
    } else {
        printf("✗ FAILED (error: %s)\n", opus_strerror(error));
        return 1;
    }

    printf("Test 2: Creating mono decoder... ");
    decoder = opus_decoder_create(16000, 1, &error);
    if (error == OPUS_OK && decoder != NULL) {
        printf("✓ SUCCESS\n");
        opus_decoder_destroy(decoder);
    } else {
        printf("✗ FAILED (error: %s)\n", opus_strerror(error));
        return 1;
    }

#ifdef DISABLE_STEREO
    /* Test 3: Stereo should be rejected */
    printf("Test 3: Creating stereo encoder (should FAIL)... ");
    encoder = opus_encoder_create(16000, 2, OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK || encoder == NULL) {
        printf("✓ Correctly rejected (error: %s)\n", opus_strerror(error));
    } else {
        printf("✗ FAILED - stereo was accepted!\n");
        opus_encoder_destroy(encoder);
        return 1;
    }

    printf("Test 4: Creating stereo decoder (should FAIL)... ");
    decoder = opus_decoder_create(16000, 2, &error);
    if (error != OPUS_OK || decoder == NULL) {
        printf("✓ Correctly rejected (error: %s)\n", opus_strerror(error));
    } else {
        printf("✗ FAILED - stereo was accepted!\n");
        opus_decoder_destroy(decoder);
        return 1;
    }

    printf("\n✓ Mono-only build verified: stereo correctly disabled\n");
#else
    printf("\nNote: Stereo support is enabled (not a mono-only build)\n");
#endif

    return 0;
}
