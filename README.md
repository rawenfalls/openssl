openssl genrsa -out privatekey 2048                                             создание закрытого ключа
openssl dgst -sha256 -sign privatekey -out readme.signature readme.txt          подпись
openssl rsa -in privatekey -outform PEM -pubout -out publickey                  генерация открытого ключа
openssl dgst -sha256 -verify publickey -signature readme.signature readme.txt   для проверки

## Оглавление

1. [Доверенное обновление BMC](#доверенное-обновление-bmc)
    * 1.1. [Скрипт для подписания файлов](#signsh-путь-signaturesignsh)
    * 1.2. [Проверка подписи на сервере](#проверка-подписи-на-сервере)

## Доверенное обновление BMC

### Скрипт для подписания файлов
#### sign.sh (путь signature/sign.sh)
#### Запуск (*-название прошивки)
```bash
./signature/sign.sh out/*/*.ima

```
#### Принцип работы
```mermaid
    flowchart TB
    node1([начало]) --> node2[/необходимы файлы: out/*/*.ima\nkey/privatekey\nkey/publickey /];
    node2 --> node3["./signature/sign.sh out/*/*.ima\n(создаем подпись\nдобавляем ее в конец файла *.ima\nсоздаем папку key по пути out/*/rootfs/,\nкуда помещаем publickey)"]
    node3-->node4[/"файлы:\nмодифицирован out/*/*.ima(добавлена подпись)\nсоздан out/*/rootfs/key/publickey"/]
    node4-->node5([конец])
```
###### [К оглавлению](#Оглавление)
---
### Проверка подписи на сервере
### verify_signature (путь lib/flash/flashlib.cpp)
static int verify_signature(const char *name) - проверяет подпись, вызывает extracts_signature_in_separate_file

static int extracts_signature_in_separate_file(const char *source_path, long delete_len, const char *signature) - вырезает подпись из файла и помещает ее в отдельный файл

#### Принцип работы
```mermaid
    flowchart TB
    node1([начало]) --> node2[/"файл: *.ima(с подписью)\n(для проверки подписи необходим publickey,\nпредпологается, что он уже лежит на сервере в папке key/)"/];
    node2 --> node3["из конца файла *.ima(с подписью), вырезается подпись,\nфайл *.ima сохраняется без подписи, создается фаайл signature,\nпроисходит проверка с использованием файлов: *.ima, signature, publickey\n, файл signature удаляется"];
    node3-->node4[/"файл:\nмодифицирован *.ima(вырезанна подпись)"/]
    node4-->node5{"проверка подписи\nуспешна?"};
    node5-- "да" --> node6["разрешает обновление"]
    node5-- "нет" --> node7["запрещает обновление"]
    node6-->node8([конец])
    node7-->node8([конец])
    style node6 fill:#3f3,stroke:#333,stroke-width:4px
    style node7 fill:#ff2400,stroke:#333,stroke-width:4px,color:#fff
```
###### [К оглавлению](#Оглавление)
---
