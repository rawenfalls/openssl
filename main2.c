#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Макросы для цветов
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void read_and_edit_file(const char *source_path, long delete_len, const char *signature_path){
    //открытие файла для чтения и записи
    FILE *source = fopen(source_path, "rb");
    if (source == NULL) {
        error("Не удалось открыть файл для чтения");
    }

    //определение длинны файла
    fseek(source, 0, SEEK_END);
    long file_size = ftell(source);
    fseek(source, 0, SEEK_SET);

    //выделение памяти
    char *buffer = (char *)malloc(file_size);
    if (buffer == NULL) {
        error("Ошибка выделения памяти");
    }

    // Чтение содержимого файла в буфер
    fread(buffer, 1, file_size, source);

    //зыкрытие файла
    fclose(source);

    //открытие файла для записи
    source = fopen(source_path, "wb");
    if (source == NULL) {
        error("Не удалось открыть файл для записи");
    }

    // Выбор символов которые будут записаны в файл источник
    if (delete_len > file_size) {
        error("Количество символов для удаления больше размера файла");
    }
    long len_sourse = file_size - delete_len;

    //запись выбранных символов обратно в файл
    fwrite(buffer, 1, len_sourse, source);

    //зыкрытие файла
    fclose(source);

    //создание файла куда будут записанны оставшиеся символы
    source = fopen(signature_path, "wb");
    if (source == NULL) {
        error("Не удалось открыть файл для записи");
    }
    
    //перемещение оставшихся символов вначало буфера
    for (int i=0; i<delete_len; i++){
        buffer[i] = buffer[len_sourse+i];
    }

    //запись оставшихся символов в файл
    fwrite(buffer, 1, delete_len, source);
    
    //зыкрытие файла
    fclose(source);

    //освобождение выделенной памяти
    free(buffer);
}

int main() {
    char sys_string[1024];  // Буфер для строки
    const char *public_key_path = "key/publickey";//путь к публичному ключу
    const char *ima_path = "ts3000.ima";//путь к *.ima файлу
    const char *signature_path = "signature/signature";//путь к подписи


    read_and_edit_file(ima_path, 256, signature_path);//убераем подпись из файла *.ima,
        //создаем новый файл с путем signature_path куда записываем новую подпись

    //создаем строку для system()
    sprintf(sys_string, "openssl dgst -sha256 -verify %s -signature %s %s > /dev/null 2>&1", public_key_path, signature_path, ima_path);

    int result = system(sys_string);//проверим подпись файла

    //вывод
    if (result == 0) {
        printf(GREEN_TEXT "Signature is valid!\n" RESET_COLOR);
        return 0;
    } else if (result == 1) {
        printf(RED_TEXT "Signature is invalid.\n" RESET_COLOR);
        return 1;
    } else {
        perror(RED_TEXT "Error verifying signature" RESET_COLOR);
        return 1;
    }
    return 0;
}