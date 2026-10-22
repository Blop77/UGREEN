#!/bin/sh
# End-to-end test: build a small folder of files and check what nasweep finds.
set -eu

BIN=${1:-build/nasweep}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

mkdir -p "$TMP/photos/trip" "$TMP/docs"
printf 'beach photo bytes' > "$TMP/photos/a.jpg"
printf 'beach photo bytes' > "$TMP/photos/trip/a_copy.jpg"
printf 'beach photo bytes' > "$TMP/docs/a_again.jpg"
printf 'beach photo BYTES' > "$TMP/photos/different.jpg"   # same size, different content
printf 'report v1' > "$TMP/docs/report.txt"
printf 'report v1' > "$TMP/docs/report (1).txt"
: > "$TMP/empty1"
: > "$TMP/empty2"                                          # empty files are ignored

fail=0
check() {
    if printf '%s' "$OUT" | grep -q "$1"; then
        echo "ok   - $2"
    else
        echo "FAIL - $2"; fail=1
    fi
}

OUT=$("$BIN" "$TMP")
check '8 files scanned, 2 duplicate groups, 43 bytes reclaimable' "finds both duplicate groups"
check 'group 1: 2 copies, 9 bytes each' "smaller group listed first"
check 'group 2: 3 copies, 17 bytes each' "three-copy group"

OUT=$("$BIN" --json "$TMP")
check '"wasted_bytes":43' "json output"
check 'report (1).txt' "paths with spaces"

if "$BIN" >/dev/null 2>&1; then
    echo "FAIL - missing directory should be a usage error"; fail=1
else
    echo "ok   - missing directory is a usage error"
fi

exit $fail
