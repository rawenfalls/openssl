#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

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
        perror("Error opening file");
        return "";
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_content = (char *)malloc(file_size + 1);
    if (!file_content) {
        fclose(file);
        perror("Error allocating memory");
        return "";
    }
    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';
    return(file_content);
}

int main() {
    const char *message = "for_check/readme.txt";//к файлу
    const char *signature = "signature/readme.signature";//путь к подписи
    const char *public_key_path = "key/publickey";//путь к публичному ключу
    const char *ima_path = "a.ima";
    const char *signature_path = "signature/signature";

    char *messageS = read_from_file(message);//чтение из файла
    char *signatureS = read_from_file(signature);//чтение из файла
    if (messageS == "" || signatureS == "") return 1;

    int result = verify_signature(messageS, signatureS, public_key_path);
    free(messageS);
    free(signatureS);

    if (result == 1) {
        printf("Signature is valid!\n");
        return 0;
    } else if (result == 0) {
        printf("Signature is invalid.\n");
        return 1;
    } else {
        perror("Error verifying signature");
        return 1;
    }
    return 0;
}