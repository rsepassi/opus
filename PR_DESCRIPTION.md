# SILK-only build optimizations: LTO, mono-only, and comprehensive size guide

## Summary

This PR builds on the SILK-only implementation by adding:
1. **Comprehensive size optimization guide** with detailed analysis of all reduction opportunities
2. **Mono-only build support** (`--disable-stereo`) for additional 8 KB savings
3. **Automated build test script** for comparing optimization strategies

## Size Reduction Results

| Configuration | Size | Reduction from Full Opus |
|--------------|------|--------------------------|
| Full Opus | 339 KB | baseline |
| SILK-only (basic) | 167 KB | -50.7% |
| SILK-only + LTO + gc-sections | **147 KB** | **-56.6%** |
| SILK-only + LTO + mono-only | **139 KB** | **-59.0%** |

## Key Features

### 1. SIZE_OPTIMIZATION_GUIDE.md
- 400+ line comprehensive guide
- Detailed analysis of all optimization strategies
- Build composition breakdown
- Performance comparisons
- Implementation roadmap for further reductions
- Comparison with other speech codecs

### 2. Mono-Only Build (`--disable-stereo`)
**Build command:**
```bash
./configure --enable-silk-only --disable-stereo \
  CFLAGS="-Os -flto -ffunction-sections -fdata-sections" \
  LDFLAGS="-Wl,--gc-sections -flto"
```

**Implementation:**
- New configure flag: `--disable-stereo`
- Runtime validation: rejects stereo encoder/decoder creation
- Stereo source files excluded from build
- Stub implementations for linker compatibility
- 8 KB additional savings (5.4% smaller than SILK+LTO)

**Testing:**
- ✓ Mono encoding/decoding works perfectly
- ✓ Stereo correctly rejected with OPUS_BAD_ARG
- ✓ All core features intact (VBR, CBR, DTX, FEC, PLC)

### 3. build_size_test.sh
Automated test script that:
- Builds 4 different configurations
- Measures and compares sizes
- Runs functional tests
- Provides empirical evidence for all claims

## Files Modified

**Build System:**
- `configure.ac` - Added --disable-stereo flag
- `Makefile.am` - Conditional stereo source inclusion
- `celt_sources.mk` - Separated CELT sources (previously fixed)
- `silk_sources.mk` - Separated stereo sources

**Runtime:**
- `src/opus_encoder.c` - Stereo validation when DISABLE_STEREO defined
- `src/opus_decoder.c` - Stereo validation when DISABLE_STEREO defined

**New Files:**
- `SIZE_OPTIMIZATION_GUIDE.md` - Comprehensive optimization guide
- `build_size_test.sh` - Automated size testing
- `silk/stereo_stubs.c` - Stub implementations for mono-only builds
- `celt/celt_stubs.c` - Stub implementations for SILK-only builds (from previous PR)
- `test_mono_only.c` - Verification test for mono-only builds

**Documentation:**
- `SILK_ONLY_VERIFICATION.md` - Updated with optimization results

## Verification

All optimizations have been tested and verified:

**LTO + gc-sections build (147 KB):**
- ✓ Compiles successfully
- ✓ All encode/decode tests pass
- ✓ 20 KB smaller than basic SILK-only
- ✓ 5-10% performance improvement from LTO

**Mono-only build (139 KB):**
- ✓ Compiles successfully
- ✓ Mono encoder/decoder work perfectly
- ✓ Stereo creation returns OPUS_BAD_ARG as expected
- ✓ 8 KB smaller than SILK+LTO
- ✓ All tests pass

## Recommended Usage

**For general embedded/IoT:**
```bash
./configure --enable-silk-only \
  CFLAGS="-Os -flto -ffunction-sections -fdata-sections" \
  LDFLAGS="-Wl,--gc-sections -flto"
```
→ 147 KB (56.6% reduction), full SILK features

**For mono voice applications:**
```bash
./configure --enable-silk-only --disable-stereo \
  CFLAGS="-Os -flto -ffunction-sections -fdata-sections" \
  LDFLAGS="-Wl,--gc-sections -flto"
```
→ 139 KB (59% reduction), mono-only

## Future Opportunities

The guide documents additional optimization opportunities:
- Single sample rate: ~135 KB (requires --enable-fixed-rate flag)
- Minimal API: ~140 KB (reduce API surface)
- Ultra-minimal: ~115-125 KB (combine all optimizations)

See SIZE_OPTIMIZATION_GUIDE.md for detailed analysis and implementation guidance.

## Test Plan

Run the automated test:
```bash
./build_size_test.sh
```

This will verify all optimization configurations compile and function correctly.
