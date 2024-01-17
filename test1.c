#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Макросы для цветов
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

int foo(){
    return 1;
}

int main() {
    if (foo()){printf("ttttttttt");}
    return 0;
}