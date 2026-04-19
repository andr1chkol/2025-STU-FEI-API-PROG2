#!/bin/zsh

# Шлях до папок з тестами
INPUT_DIR="z2_testing/stdin/scenar_3/"
OUTPUT_DIR="z2_testing/stdout/scenar_3/"

# Лічильники тестів
passed=0
failed=0

# Проходимо через всі вхідні файли
for input_file in $INPUT_DIR/*.txt; do
    # Отримуємо ім'я файлу без шляху
    filename=$(basename -- "$input_file")
    
    # Очікуваний вихідний файл
    expected_output="$OUTPUT_DIR/$filename"
    
    # Запускаємо програму, зчитуємо вхідний файл і порівнюємо з очікуваним виходом
    ./z2 < "$input_file" | diff -wB - "$expected_output" > /dev/null

    # Перевіряємо результат diff
    if [[ $? -eq 0 ]]; then
        echo "✅ Тест $filename пройдено!"
        ((passed++))
    else
        echo "❌ Тест $filename НЕ пройдено!"
        ((failed++))
    fi
done

# Виводимо підсумок
echo "=============================="
echo "Успішні тести: $passed"
echo "Неуспішні тести: $failed"
echo "=============================="

# Повертаємо статус (0, якщо всі тести пройдено, 1 якщо є помилки)
if [[ $failed -eq 0 ]]; then
    exit 0
else
    exit 1
fi

