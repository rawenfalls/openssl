#!/bin/bash

quiet=false

while [ "$#" -gt 0 ]; do
    case "$1" in
        -q|--quiet)
            quiet=true
            shift
            ;;
        *)
            source_file="$1"
            char_count="$2"
            destination_file="$3"
            shift 3
            ;;
    esac
done

if [ -z "$source_file" ] || [ -z "$char_count" ] || [ -z "$destination_file" ]; then
    echo "Использование: $0 [-q|--quiet] исходный_файл количество_символов файл_назначения"
    exit 1
fi

# Проверка существования исходного файла
if [ ! -e "$source_file" ]; then
    echo "Ошибка: Файл '$source_file' не существует."
    exit 1
fi

# Получение длины файла
file_length=$(wc -c < "$source_file")

# Проверка, что количество символов для удаления не больше длины файла
if [ "$char_count" -gt "$file_length" ]; then
    echo "Ошибка: Количество символов для удаления больше длины файла."
    exit 1
fi

# Вырезание символов из конца файла и сохранение их в отдельный файл
cut_content=$(tail -c -d '\0' +"$((file_length - char_count + 1))" "$source_file")

# Сохранение вырезанных символов в другой файл
echo "$cut_content" > "$destination_file"

if [ "$quiet" = false ]; then
    # Вывести только сообщение об успешном выполнении, если установлен флаг quiet
    echo "Удалено и сохранено $char_count символов из конца файла '$source_file' в '$destination_file'."
fi
