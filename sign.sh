#!/bin/sh


# Получаем каталог, в котором находится скрипт
script_dir="$( cd "$( dirname $0 )" && pwd )"
#$0 предоставляет имя запущенного скрипта.
#dirname извлекает каталог из полного пути.
#cd сменяет текущий каталог на указанный.
#pwd возвращает текущий каталог.

#инициализация флага quiet (false выводит сообщение; true не выводит сообщение)
quiet=false

#путь к файлу источника(файл в котором храниться подпись)
source_file="$script_dir/../signature/signature"

#путь к файлу в котором храниться публичный ключ
open_key="$script_dir/../key/publickey"

#путь к файлу в котором храниться приватный ключ
privat_key="$script_dir/../key/privatekey"

#считывает флаг -q|--quiet 
while [ "$#" -gt 0 ]; do
    case "$1" in
        -q|--quiet)
            quiet=true
            shift
            ;;
        *)
            destination_file="$1"
            shift 
            ;;
    esac
done

#считывает путь к файлу который надо подписать
if [ -z "$destination_file" ]; then
    echo "Использование: $0 [-q|--quiet] файл_назначения(обычно *.ima) подпись будет скопированна в конец файла назначения"
    exit 1
fi

#путь к дериктории в которой храниться файл(*.ima)
directory="$script_dir/../$(dirname "$destination_file")"

#проверка существования публичного ключа
if [ ! -e "$open_key" ]; then
    echo "Ошибка: Файл "$open_key" не существует.\nДля создания публичного ключа выполните команду\nopenssl rsa -in "$privat_key" -outform PEM -pubout -out "$open_key""
    exit 1
fi

#проверка существования закрытого ключа
if [ ! -e "$privat_key" ]; then
    echo "Ошибка: Файл '$privat_key' не существует."
    exit 1
fi

#создание подписи
openssl dgst -sha256 -sign "$privat_key" -out "$source_file" "$destination_file" 

#проверка подписи(выведет ОК в консоль если проверка прошла успешно)
openssl dgst -sha256 -verify "$open_key" -signature "$source_file" "$destination_file"

#проверка существования файла_источника
if [ ! -e "$source_file" ]; then
    echo "Ошибка: Файл '$source_file' не существует."
    exit 1
fi

#проверка существования файла_назначения
if [ ! -e "$destination_file" ]; then
    echo "Ошибка: Файл '$destination_file' не существует."
    exit 1
fi

# Добавление содержимого файла_источника в конец файла_назначения
cat "$source_file" >> "$destination_file"

#создаем папку key в дериктории с файлом (*.ima)
mkdir -p "$directory"/rootfs/key

#создаем папку signature в дериктории с файлом (*.ima)
mkdir -p "$directory"/rootfs/signature

#копируем публичный ключ в папку key 
cp "$open_key" "$directory/rootfs/key/"

#копируем подпись в папку  signature
#cp signature "$directory/rootfs/signature/"

rm -rf "$source_file"

#если флаг quiet == false выведет сообщение в консоль
if [ "$quiet" = false ]; then
    echo "Содержимое файла '$source_file' успешно добавлено в конец файла '$destination_file'."
fi