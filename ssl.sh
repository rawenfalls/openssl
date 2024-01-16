#!/bin/bash

#файл источник
source_file=signature/readme.signature
#файл назначения
destination_file=ts3000.ima

#создание подписи
openssl dgst -sha256 -sign key/privatekey -out signature/readme.signature ts3000.ima 
#проверка подписи
openssl dgst -sha256 -verify key/publickey -signature signature/readme.signature ts3000.ima

# Проверка существования файла источника
if [ ! -e "$source_file" ]; then
    echo "Ошибка: Файл '$source_file' не существует."
    exit 1
fi
# Проверка существования файла назначения
if [ ! -e "$destination_file" ]; then
    echo "Ошибка: Файл '$destination_file' не существует."
    exit 1
fi

cat "$source_file" >> "$destination_file"

#test
gcc main2.c && ./a.out && rm -rf a.out
