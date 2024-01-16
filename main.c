#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Макросы для цветов
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int verify_signature(const char *message, const char *signature, const char *public_key_path) {
    FILE *key_file = fopen(public_key_path, "r");
    if (!key_file) {
        perror("Error opening public key file");
        return -1;
    }

    EVP_PKEY *public_key = PEM_read_PUBKEY(key_file, NULL, NULL, NULL);
    fclose(key_file);

    if (!public_key) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_MD_CTX_init(md_ctx);

    if (EVP_VerifyInit(md_ctx, EVP_sha256()) != 1) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    if (EVP_VerifyUpdate(md_ctx, message, strlen(message)) != 1) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    int result = EVP_VerifyFinal(md_ctx, (const unsigned char *)signature, strlen(signature), public_key);

    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(public_key);

    return result;
}

char* read_from_file(const char *path){
        FILE *file = fopen(path, "r");
    if (!file) {
        error("Error opening file");
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_content = (char *)malloc(file_size + 1);
    if (!file_content) {
        fclose(file);
        error("Error allocating memory");
    }
    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';
    return(file_content);
}

void read_and_edit_file(const char *source_path, long delete_len, const char *signature_path){
    //открытие файла для чтения и записи
    FILE *source = fopen(source_path, "r");
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
    source = fopen(source_path, "w");
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
    source = fopen(signature_path, "w");
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
    //объявление путей к файлам
    //const char *message = "for_check/readme.txt";//к файлу
    const char *signature = "signature/readme.signature";//путь к подписи
    const char *public_key_path = "key/publickey";//путь к публичному ключу
    const char *ima_path = "a.ima";
    const char *signature_path = "signature/signature";

    read_and_edit_file(ima_path, 256, signature_path);//убераем подпись из файла *.ima,
    //создаем новый файл с путем signature_path куда записываем новую подпись
    char *messageS = read_from_file(ima_path);//чтение из *ima файла
    char *signatureS = read_from_file(signature_path);//чтение подписи из файла

    //файл для проверки messageS, подпись signatureS, путь до публичного ключа
    int result = verify_signature(messageS, signatureS, public_key_path);

    //освобождаем память которая выделялась в функции read_from_file()
    free(messageS);
    free(signatureS);

    if (result == 1) {
        printf(GREEN_TEXT "Signature is valid!\n" RESET_COLOR);
        return 0;
    } else if (result == 0) {
        printf(RED_TEXT "Signature is invalid.\n" RESET_COLOR);
        return 1;
    } else {
        perror(RED_TEXT "Error verifying signature" RESET_COLOR);
        return 1;
    }
    return 0;
}