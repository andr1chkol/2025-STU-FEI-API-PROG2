#!/bin/zsh

PROGRAM="./z3"
BASE_INPUT="z3_testing_extra/stdin"
BASE_OUTPUT="z3_testing_extra/stdout"

total_passed=0
total_failed=0

for scenar in $BASE_INPUT/scenar_*; do
    scenar_name=$(basename "$scenar")
    INPUT_DIR="$BASE_INPUT/$scenar_name"
    OUTPUT_DIR="$BASE_OUTPUT/$scenar_name"

    echo "=== Тестуємо $scenar_name ==="
    passed=0
    failed=0

    for input_file in $INPUT_DIR/*.txt; do
        filename=$(basename "$input_file")
        expected_output="$OUTPUT_DIR/$filename"

        $PROGRAM < "$input_file" | diff -wB - "$expected_output" > /dev/null

        if [[ $? -eq 0 ]]; then
            echo "✅ $filename — OK"
            ((passed++))
        else
            echo "❌ $filename — FAIL"
            ((failed++))
        fi
    done

    echo "--- $scenar_name: ✅ $passed | ❌ $failed ---"
    ((total_passed += passed))
    ((total_failed += failed))
    echo
done

echo "=============================="
echo "🔎 Загалом:"
echo "✅ Успішних: $total_passed"
echo "❌ Помилок:  $total_failed"
echo "=============================="

if [[ $total_failed -eq 0 ]]; then
    exit 0
else
    exit 1
fi
