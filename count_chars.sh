#!/bin/sh

quiet=false

while [ "$#" -gt 0 ]; do
    case "$1" in
        -q|--quiet)
            quiet=true
            shift
            ;;
        *)
            file_path="$1"
            shift
            ;;
    esac
done

if [ -z "$file_path" ]; then
    echo "Использование: $0 [-q|--quiet] путь_к_файлу"
    exit 1
fi

# Проверка существования файла
if [ ! -e "$file_path" ]; then
    echo "Ошибка: Файл '$file_path' не существует."
    exit 1
fi

# Подсчет количества символов в файле
char_count=$(wc -c < "$file_path")

if [ "$quiet" = true ]; then
    # Вывести только цифру, если установлен флаг quite
    echo "$char_count"
else
    # Вывести сообщение с количеством символов
    echo "Количество символов в файле '$file_path': $char_count"
fi