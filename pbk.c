#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

static char* extractFolderPath(const char *filePath, char *folderPath) {
    size_t i = strlen(filePath) - 1;
    // Ищем последний символ '/' в пути
    while (i > 0 && filePath[i] != '/') {
        printf("%c\n", filePath[i]);
        i--;
    }
    strncpy(folderPath, filePath, i);
    folderPath[i] = '\0';  // Добавляем нулевой символ в конец строки
}

static int publickey_exists (const char *publickey_path){
    char folderPath[100];
	//проверяем существует ли файл
	if (access(publickey_path, F_OK) != -1){
		//если файл существует выходим из функции
		return 0;
	} else {
		//формируем путь до папки в которой должен лежать файл
        extractFolderPath(publickey_path, folderPath);
		//если файл не существует создаем папку в которой он должен лежать
		if (mkdir(folderPath, 0777) == 0) { 
			FILE *file = fopen(publickey_path, "wb");//создаем файл
			if (file != NULL) {
				//публичный ключ
				char * publickey="-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw8WCLe2jhWyoUsbd1THa\nH8NJWmD9CtdEdGXTMUuKtr0dXZ5FGpxe32UTJQr7NE88lfefWCyJbZHHquwuhU4Q\nyRsanoB7fz8/D/DhEmrtc/BXX9ldpD6YYeV3x2gMD7ttaDoiZW2Rv6jBtdE8rR9k\nnAcdTKzA/UG99xBleIuw5UTLyYARXhhUbkabZ4dtiwtTV1RXCSv2qDo7jyofnTme\nZVGbB+5v3pzIky2Wh6PeDxWlgRq3iBTQyNFyv/iPCf81HlFpSjNHol+0CPkC1t84\ni6zsEuX5JzCIIWCEK5af/u5imbuEZgiA/RR7QXs9OcR+qiMpx6VSQhxKn/mBUqoT\nJQIDAQAB\n-----END PUBLIC KEY-----\n";
				fwrite(publickey, 1, strlen(publickey), file);//записываем в файл публичный ключ
            	fclose(file);//закрываем файл
        	} else {return 1;}//не удалось создать файл
		} else {return 2;}//не удалось создать папку
	}
	return 0;
}

int main(){
    const char *publickey_path = "key/publickey";//не более 100 символов
    int i =0;
    if (i=publickey_exists(publickey_path))
	{
        printf("ERROR: publickey_check");
        printf("\n|%d|", i);
		return -1;
	}
    return 0;
}