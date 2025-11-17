/* Copyright (c) 2024 Opus contributors
   Stub implementations for SILK-only builds */
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

#ifdef ENABLE_SILK_ONLY

#include <stdarg.h>
#include "celt.h"
#include "opus_defines.h"

/* Stub implementation of opus_custom_encoder_ctl for SILK-only builds */
int opus_custom_encoder_ctl(CELTEncoder *OPUS_RESTRICT st, int request, ...)
{
   (void)st;
   (void)request;
   /* In SILK-only builds, CELT encoder doesn't exist, so all CTL calls are no-ops */
   return OPUS_OK;
}

/* Stub implementation of opus_custom_decoder_ctl for SILK-only builds */
int opus_custom_decoder_ctl(CELTDecoder *OPUS_RESTRICT st, int request, ...)
{
   (void)st;
   (void)request;
   /* In SILK-only builds, CELT decoder doesn't exist, so all CTL calls are no-ops */
   return OPUS_OK;
}

/* opus_strerror implementation for SILK-only builds */
const char *opus_strerror(int error)
{
   static const char * const error_strings[8] = {
      "success",
      "invalid argument",
      "buffer too small",
      "internal error",
      "corrupted stream",
      "request not implemented",
      "invalid state",
      "memory allocation failed"
   };
   if (error > 0 || error < -7)
      return "unknown error";
   else
      return error_strings[-error];
}

/* opus_get_version_string implementation for SILK-only builds */
const char *opus_get_version_string(void)
{
   return "libopus SILK-only";
}

#endif /* ENABLE_SILK_ONLY */
