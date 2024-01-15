#!/bin/sh

quiet=false

while [ "$#" -gt 0 ]; do
    case "$1" in
        -q|--quiet)
            quiet=true
            shift
            ;;
        *)
            source_file="$1"
            destination_file="$2"
            shift 2
            ;;
    esac
done

if [ -z "$source_file" ] || [ -z "$destination_file" ]; then
    echo "Использование: $0 [-q|--quiet] файл_источник(откуда) файл_назначение(куда)\nисточник будет скопирован в конец файла назначения"
    exit 1
fi

# Проверка существования файла_источника
if [ ! -e "$source_file" ]; then
    echo "Ошибка: Файл '$source_file' не существует."
    exit 1
fi

if [ ! -e "$destination_file" ]; then
    echo "Ошибка: Файл '$destination_file' не существует."
    exit 1
fi

# Добавление содержимого файла_источника в конец файла_назначения
cat "$source_file" >> "$destination_file"
if [ "$quiet" = false ]; then
echo "Содержимое файла '$source_file' успешно добавлено в конец файла '$destination_file'."
fi