#!/bin/bash

CMD_DIR="z5_testing/cmd"
OUT_DIR="z5_testing/output"
JSON_DIR="json"
EXE="./z5"

# Підрахунок
success=0
fail=0

# Пройдемо всі приклади
for cmd_file in $(find "$CMD_DIR" -name "example*.txt" | sort); do
    echo "▶️  Тест: $cmd_file"

    # Витягнути команду запуску
    run_cmd=$(grep -i "Spustenie:" "$cmd_file" | cut -d':' -f2- | xargs)

    # Витягнути JSON
    json_file=$(grep -i "Databaza predmetov:" "$cmd_file" | cut -d':' -f2 | cut -d'(' -f1 | xargs)

    if [[ -z "$run_cmd" || -z "$json_file" ]]; then
        echo "  ❌ Не вдалося прочитати команду або JSON"
        ((fail++))
        continue
    fi

    if [[ ! -f "$JSON_DIR/$json_file" ]]; then
        echo "  ❌ JSON  не знайдено"
        ((fail++))
        continue
    fi

    # Копіювати JSON → items.json
    cp "$JSON_DIR/$json_file" items.json

    # Запустити команду
    $EXE $run_cmd > tmp_output.txt 2>&1
    exit_code=$?

    # Очікуваний файл
    rel_path="${cmd_file#$CMD_DIR/}"
    expected_file="$OUT_DIR/$rel_path"

    # Перевірити вихід
    if [[ ! -f "$expected_file" ]]; then
        echo "  ⚠️ Очікуваний файл не знайдено: $expected_file"
        ((fail++))
        continue
    fi

    # diff
    if diff -q "$expected_file" tmp_output.txt > /dev/null; then
        echo "  ✅ Passed"
        ((success++))
    else
        echo "  ❌ Failed"
        diff "$expected_file" tmp_output.txt | head -n 20
        ((fail++))
    fi
done

echo
echo "✅ Успішно: $success"
echo "❌ Не пройшли: $fail"
