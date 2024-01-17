static int read_and_edit_file(const char *source_path, long delete_len, const char *signature_path){
    //открытие файла для чтения и записи
    FILE *source = fopen(source_path, "rb");
    if (source == NULL) {
        printv("Не удалось открыть файл для чтения");
		return -1;
    }

    //определение длинны файла
    fseek(source, 0, SEEK_END);
    long file_size = ftell(source);
    fseek(source, 0, SEEK_SET);

    //выделение памяти
    char *buffer = (char *)malloc(file_size);
    if (buffer == NULL) {
        printv("Ошибка выделения памяти");
		return -1;
    }

    // Чтение содержимого файла в буфер
    fread(buffer, 1, file_size, source);

    //зыкрытие файла
    fclose(source);

    //открытие файла для записи
    source = fopen(source_path, "wb");
    if (source == NULL) {
        printv("Не удалось открыть файл для записи");
		return -1;
    }

    // Выбор символов которые будут записаны в файл источник
    if (delete_len > file_size) {
        printv("Количество символов для удаления больше размера файла");
		return -1;
    }

	//вычисляем длинну файла *.ima
    long len_sourse = file_size - delete_len;

    //запись выбранных символов обратно в файл
    fwrite(buffer, 1, len_sourse, source);

    //зыкрытие файла
    fclose(source);

    //создание файла куда будут записанны оставшиеся символы
    source = fopen(signature_path, "wb");
    if (source == NULL) {
        printv("Не удалось открыть файл для записи");
		return -1;
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
	return 0;
}

static int verify_signature(const char *name)
{
		//буфер для строки
	char 		sys_string[1024];
	//путь к публичному ключу
    const char *public_key_path = "/key/publickey";
	//путь к *.ima файлу
    const char *ima_path 		= name;
	//путь к подписи
    const char *signature_path 	= "/signature/signature";
	//длинна подписи 256 символов
	long		signature_len	= 256;

	//убераем подпись из файла *.ima,создаем новый файл 
	//с путем signature_path куда записываем новую подпись
    if (read_and_edit_file(ima_path, signature_len, signature_path))
	{
		printv("ERROR: read_and_edit_file()");
		return -1;
	}

    //создаем строку для system()
    sprintf(sys_string, "openssl dgst -sha256 -verify %s -signature %s %s > /dev/null 2>&1", public_key_path, signature_path, ima_path);

	//проверяем подпись файла, если подпись валидна вернет 0
	//если подпись не валидна вернет 1, любой другой вывод это ошибка
    return(system(sys_string));
}

int verify_image(const char *name)//верификация *.img
{
	if (verify_signature(name)) //верификация подписи
	{
		printv("ERROR verify_signature()");
		return -1;
	}