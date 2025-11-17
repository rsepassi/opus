# Core OPUS sources (required for both full and SILK-only builds)
OPUS_SOURCES_CORE = \
src/opus.c \
src/opus_decoder.c \
src/opus_encoder.c \
src/extensions.c \
src/repacketizer.c

# Multistream/projection sources (excluded in SILK-only builds)
OPUS_SOURCES_MULTISTREAM = \
src/opus_multistream.c \
src/opus_multistream_encoder.c \
src/opus_multistream_decoder.c \
src/opus_projection_encoder.c \
src/opus_projection_decoder.c \
src/mapping_matrix.c

# Base OPUS sources (will be extended based on build configuration)
OPUS_SOURCES = $(OPUS_SOURCES_CORE)

# Analysis and mode selection (excluded in SILK-only builds, also requires float)
OPUS_SOURCES_FLOAT = \
src/analysis.c \
src/mlp.c \
src/mlp_data.c
