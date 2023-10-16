#include "my_lib.h"
#include <stdlib.h>

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

char *my_strcat(char *dest, const char *str){

    //Tamaño es igual al num de char de str + num de char de dest + 1 (caracter nulo)
    size_t tam = sizeof(char) * (my_strlen(str) + my_strlen(dest) + 1);
    int i = 0; //Contador

    char *a = (char *)malloc(tam);

    while (*dest)
    {
        *a = *dest;
        dest++;
        a++;
        i++;
    }

    while (*str)
    {
        *a = *str;
        str++;
        a++;
        i++;
    }
    
    *a = '\0';
    
    while (i > 0)
    {
        a--;
        i--;
    }

    dest = a;

    return (char *)dest;
}

char *my_strchr(const char *str, int c){
    
    //Mientras str =! 0
    while (*str)
    {
        if (*str == c)
        {
            //Casting y return del caracter de str
            return (char *)str;
        }
        
        //Siguiente caracter de la cadena str
        str++;
    }
    
}

char *my_strncpy(char *dest, const char *src, size_t n){
    //Tamaño del destino
    int tamany = 0;
    while (dest[tamany] != '\0') {
        tamany++;
    }
    tamany++; //Sumamos 1 para incluir la posición cuyo valor es null y, así, no perder 1 char de información


    //Inicializamos una variable auxiliar dinámica que nos permitirá modificar el contenido del destino
    char *auxDest = (char *)malloc(tamany * sizeof(char));
    for(int idx=0; idx<tamany; idx++) { //La inicializamos con nulls
        auxDest[idx] = '\0';
    }

    char *auxInic=src; //Para no modificar el valor de la fuente

    
    //Inicializamos el bucle que nos permitirá copiar los valores de src en dest
    for(int idx=0; idx<n; idx++) {
        if(strcmp(auxInic, "\0") == 0) {
            //Si hemos llegado ya al null, no escribimos nada más
            
        } else if(idx>=tamany-1) { //tamany-1 porque el último valor debe ser null
            //Si se intenan escribir más valores de los que caben en el destino, se para de escribir
            
        } else {
            auxDest[idx] = *auxInic; //Se copia el valor[idx] de src en dest
            auxInic++;
        }
    }
    dest=auxDest; //Hacemos que el destino apunte al nuevo string
}
