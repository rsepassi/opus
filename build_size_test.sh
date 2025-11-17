#!/bin/bash
# Size optimization test script for SILK-only builds
# Compares different compiler optimization strategies

set -e

echo "======================================"
echo " SILK-Only Build Size Test"
echo "======================================"
echo ""

# Create output directory for results
mkdir -p build_test_results

# Function to build and measure
build_and_measure() {
    local name=$1
    local cflags=$2
    local ldflags=$3

    echo "Building: $name"
    echo "  CFLAGS: $cflags"
    echo "  LDFLAGS: $ldflags"

    make distclean >/dev/null 2>&1 || true
    ./configure \
        --enable-silk-only \
        --disable-extra-programs \
        --disable-doc \
        CFLAGS="$cflags" \
        LDFLAGS="$ldflags" \
        >/dev/null 2>&1

    make -j$(nproc) >/dev/null 2>&1

    cp .libs/libopus.so.0.10.1 "build_test_results/libopus-$name.so"

    # Strip and measure
    strip --strip-unneeded "build_test_results/libopus-$name.so" -o "build_test_results/libopus-$name-stripped.so"

    local size=$(ls -lh "build_test_results/libopus-$name-stripped.so" | awk '{print $5}')
    echo "  Result: $size"
    echo ""
}

# Test 1: Baseline -Os
build_and_measure "baseline-Os" "-Os" ""

# Test 2: -Os with gc-sections
build_and_measure "gc-sections" "-Os -ffunction-sections -fdata-sections" "-Wl,--gc-sections"

# Test 3: -Os with LTO and gc-sections (RECOMMENDED)
build_and_measure "optimized-LTO" "-Os -flto -ffunction-sections -fdata-sections" "-Wl,--gc-sections -flto"

# Test 4: -O3 with LTO (for comparison)
build_and_measure "speed-O3" "-O3 -flto -ffunction-sections -fdata-sections" "-Wl,--gc-sections -flto"

echo "======================================"
echo " Size Comparison Summary"
echo "======================================"
echo ""
ls -lh build_test_results/*-stripped.so | awk '{printf "%-40s %8s\n", $9, $5}' | sort -k2 -h

echo ""
echo "======================================"
echo " Recommendation"
echo "======================================"
echo ""
echo "For production embedded builds, use:"
echo ""
echo "  ./configure --enable-silk-only \\"
echo "    CFLAGS=\"-Os -flto -ffunction-sections -fdata-sections\" \\"
echo "    LDFLAGS=\"-Wl,--gc-sections -flto\""
echo ""
echo "This provides the best balance of size and performance."
echo ""

# Test the optimized build
echo "Testing optimized build functionality..."
make distclean >/dev/null 2>&1 || true
./configure \
    --enable-silk-only \
    --disable-extra-programs \
    --disable-doc \
    CFLAGS="-Os -flto -ffunction-sections -fdata-sections" \
    LDFLAGS="-Wl,--gc-sections -flto" \
    >/dev/null 2>&1
make -j$(nproc) >/dev/null 2>&1

if [ -f "test_silk_only.c" ]; then
    gcc -o test_silk_only test_silk_only.c -I./include -L.libs -lopus -lm -Wl,-rpath,.libs
    ./test_silk_only
else
    echo "test_silk_only.c not found, skipping functional test"
fi

echo ""
echo "Complete! Results saved in build_test_results/"
