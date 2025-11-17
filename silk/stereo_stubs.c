/* Copyright (c) 2024 Opus contributors
   Stub implementations for stereo functions when --disable-stereo is used */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef DISABLE_STEREO

#include "main.h"

/* Stub implementations for stereo functions when stereo is disabled */
/* These should never be called, but we provide them to avoid link errors */

void silk_stereo_LR_to_MS(
    stereo_enc_state        *state,
    opus_int16              x1[],
    opus_int16              x2[],
    opus_int8               ix[ 2 ][ 3 ],
    opus_int8               *mid_only_flag,
    opus_int32              mid_side_rates_bps[],
    opus_int32              total_rate_bps,
    opus_int                prev_speech_act_Q8,
    opus_int                toMono,
    opus_int                fs_kHz,
    opus_int                frame_length
)
{
    (void)state;
    (void)x1;
    (void)x2;
    (void)ix;
    (void)mid_only_flag;
    (void)mid_side_rates_bps;
    (void)total_rate_bps;
    (void)prev_speech_act_Q8;
    (void)toMono;
    (void)fs_kHz;
    (void)frame_length;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

void silk_stereo_MS_to_LR(
    stereo_dec_state        *state,
    opus_int16              x1[],
    opus_int16              x2[],
    const opus_int32        pred_Q13[],
    opus_int                fs_kHz,
    opus_int                frame_length
)
{
    (void)state;
    (void)x1;
    (void)x2;
    (void)pred_Q13;
    (void)fs_kHz;
    (void)frame_length;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

void silk_stereo_encode_pred(
    ec_enc                  *psRangeEnc,
    opus_int8               ix[ 2 ][ 3 ]
)
{
    (void)psRangeEnc;
    (void)ix;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

void silk_stereo_encode_mid_only(
    ec_enc                  *psRangeEnc,
    opus_int8               mid_only_flag
)
{
    (void)psRangeEnc;
    (void)mid_only_flag;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

void silk_stereo_decode_pred(
    ec_dec                  *psRangeDec,
    opus_int32              pred_Q13[]
)
{
    (void)psRangeDec;
    (void)pred_Q13;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

void silk_stereo_decode_mid_only(
    ec_dec                  *psRangeDec,
    opus_int                *decode_only_mid
)
{
    (void)psRangeDec;
    (void)decode_only_mid;
    /* Should never be called in mono-only build */
    celt_assert(0);
}

#endif /* DISABLE_STEREO */
