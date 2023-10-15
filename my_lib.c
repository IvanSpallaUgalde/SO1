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
   #include <stdio.h>
#include <string.h>


int main() {
        //Inicialización temporal (para ver que funciona bien)
        char *dest= {"holasoygermanjuasjuastomato"}; //Borrar
        char *src = {"TextoEjemplo"}; //Borrar
        size_t n = 10; //Borrar

        //Inicializamos una variable auxiliar que nos permitirá modificar
        //el contenido de dest. Lo inicializamos con "null"
        char *auxDest = (char *)malloc(5 * sizeof(char));
        
        for(int idx=0; idx<sizeof(auxDest); idx++) {
            auxDest[idx] = '\0';
        }
    
        
        char *auxInic=src;
        
        for(int idx=0;idx<n;idx++){
          auxDest[idx] = *auxInic;
          auxInic++;
        }
        
        dest=auxDest;
        printf("Destino: %s\n", dest); //Borrar. Comprueba el resultado
        return 0;
    }
}
