#include "my_lib.h"

size_t my_strlen(const char *str){
    size_t len = 0;
    int i = 0;

    while (str[i])
    {
        i++;
        len++;
    }
    return len;
}

int my_strcmp(const char *str1, const char *str2){
    int i = 0;
    int j = 0;

    while (str1[i] && (str1[i] == str2[j]))
    {
        i++;
        j++;
    }
    return (str1[i] - str2[j]);
}