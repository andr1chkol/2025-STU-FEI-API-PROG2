#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/.test-build"
MODE="${1:-smoke}"

passed=0
failed=0

mkdir -p "$BUILD_DIR"

print_section() {
    printf "\n== %s ==\n" "$1"
}

record_pass() {
    printf "PASS: %s\n" "$1"
    passed=$((passed + 1))
}

record_fail() {
    printf "FAIL: %s\n" "$1"
    failed=$((failed + 1))
}

compile() {
    local name="$1"
    shift

    if "$@"; then
        record_pass "$name compile"
        return 0
    fi

    record_fail "$name compile"
    return 1
}

run_io_tests() {
    local name="$1"
    local executable="$2"
    local input_base="$3"
    local output_base="$4"

    if [[ ! -x "$executable" ]]; then
        record_fail "$name executable missing"
        return
    fi

    if [[ ! -d "$input_base" || ! -d "$output_base" ]]; then
        record_fail "$name test data missing"
        return
    fi

    local scenario
    local input_file
    local expected_file
    local scenario_name
    local filename

    while IFS= read -r scenario; do
        scenario_name="$(basename "$scenario")"

        while IFS= read -r input_file; do
            filename="$(basename "$input_file")"
            expected_file="$output_base/$scenario_name/$filename"

            if [[ ! -f "$expected_file" ]]; then
                record_fail "$name $scenario_name/$filename expected output missing"
                continue
            fi

            if "$executable" < "$input_file" | diff -wB - "$expected_file" >/dev/null; then
                record_pass "$name $scenario_name/$filename"
            else
                record_fail "$name $scenario_name/$filename"
            fi
        done < <(find "$scenario" -maxdepth 1 -type f -name '*.txt' | sort)
    done < <(find "$input_base" -maxdepth 1 -type d -name 'scenar_*' | sort)
}

run_selected_io_tests() {
    local name="$1"
    local executable="$2"
    local input_base="$3"
    local output_base="$4"
    shift 4

    local test_case
    local input_file
    local expected_file

    if [[ ! -x "$executable" ]]; then
        record_fail "$name executable missing"
        return
    fi

    for test_case in "$@"; do
        input_file="$input_base/$test_case"
        expected_file="$output_base/$test_case"

        if [[ ! -f "$input_file" || ! -f "$expected_file" ]]; then
            record_fail "$name $test_case test data missing"
            continue
        fi

        if "$executable" < "$input_file" | diff -wB - "$expected_file" >/dev/null; then
            record_pass "$name $test_case"
        else
            record_fail "$name $test_case"
        fi
    done
}

if [[ "$MODE" != "smoke" && "$MODE" != "--full" && "$MODE" != "full" ]]; then
    printf "Usage: bash scripts/run_tests.sh [--full]\n" >&2
    exit 2
fi

print_section "PS_1"
if compile "PS_1" gcc "$ROOT_DIR/PS_1/z1.c" -lm -o "$BUILD_DIR/ps1-z1"; then
    if printf "0 1 0.5 2 8\n" | "$BUILD_DIR/ps1-z1" >/dev/null; then
        record_pass "PS_1 smoke"
    else
        record_fail "PS_1 smoke"
    fi
fi

print_section "PS_2"
if compile "PS_2" gcc \
    "$ROOT_DIR/PS_2/src/functions.c" \
    "$ROOT_DIR/PS_2/src/data.c" \
    "$ROOT_DIR/PS_2/src/z2.c" \
    -I "$ROOT_DIR/PS_2/include" \
    -lm \
    -o "$BUILD_DIR/ps2-z2"; then
    if [[ "$MODE" == "smoke" ]]; then
        run_selected_io_tests "PS_2" "$BUILD_DIR/ps2-z2" \
            "$ROOT_DIR/PS_2/z2_testing/stdin" \
            "$ROOT_DIR/PS_2/z2_testing/stdout" \
            "scenar_1/example_001_0_1.txt" \
            "scenar_2/example_001_n0.txt" \
            "scenar_4/example_001.txt" \
            "scenar_7/example_001-0c2c5c9c.txt"
    else
        run_io_tests "PS_2" "$BUILD_DIR/ps2-z2" "$ROOT_DIR/PS_2/z2_testing/stdin" "$ROOT_DIR/PS_2/z2_testing/stdout"
    fi
fi

print_section "PS_3"
if compile "PS_3" gcc "$ROOT_DIR/PS_3/z3.c" -o "$BUILD_DIR/ps3-z3"; then
    if [[ "$MODE" == "smoke" ]]; then
        run_selected_io_tests "PS_3" "$BUILD_DIR/ps3-z3" \
            "$ROOT_DIR/PS_3/z3_testing_extra/stdin" \
            "$ROOT_DIR/PS_3/z3_testing_extra/stdout" \
            "scenar_1/example_1.txt" \
            "scenar_2/example_1.txt" \
            "scenar_4/example_1.txt" \
            "scenar_8/example_1.txt"
    else
        run_io_tests "PS_3" "$BUILD_DIR/ps3-z3" "$ROOT_DIR/PS_3/z3_testing_extra/stdin" "$ROOT_DIR/PS_3/z3_testing_extra/stdout"
    fi
fi

print_section "PS_4"
if compile "PS_4" gcc \
    "$ROOT_DIR/PS_4/src/data.c" \
    "$ROOT_DIR/PS_4/src/z4.c" \
    -I "$ROOT_DIR/PS_4/include" \
    -lm \
    -o "$BUILD_DIR/ps4-z4"; then
    if [[ "$MODE" == "smoke" ]]; then
        run_selected_io_tests "PS_4" "$BUILD_DIR/ps4-z4" \
            "$ROOT_DIR/PS_4/z4_testing/stdin" \
            "$ROOT_DIR/PS_4/z4_testing/stdout" \
            "scenar_1/example_1.txt" \
            "scenar_1/example_2.txt" \
            "scenar_3/example_1.txt" \
            "scenar_6/example_1.txt"
    else
        run_io_tests "PS_4" "$BUILD_DIR/ps4-z4" "$ROOT_DIR/PS_4/z4_testing/stdin" "$ROOT_DIR/PS_4/z4_testing/stdout"
    fi
fi

print_section "PS_5"
if compile "PS_5" gcc "$ROOT_DIR/PS_5/z5.c" -o "$BUILD_DIR/ps5-z5"; then
    if (cd "$ROOT_DIR/PS_5" && "$BUILD_DIR/ps5-z5" >/dev/null); then
        record_pass "PS_5 item database smoke"
    else
        record_fail "PS_5 item database smoke"
    fi

    if (cd "$ROOT_DIR/PS_5" && "$BUILD_DIR/ps5-z5" txt/army1.txt txt/army2.txt >/dev/null); then
        record_pass "PS_5 army parser smoke"
    else
        record_fail "PS_5 army parser smoke"
    fi
fi

print_section "Summary"
printf "Passed: %d\n" "$passed"
printf "Failed: %d\n" "$failed"

if [[ "$failed" -eq 0 ]]; then
    exit 0
fi

exit 1
