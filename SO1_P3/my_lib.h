/* lib.h librería con las funciones equivalentes a las
de <string.h> y las funciones y estructuras para el
manejo de una pila */

#include <stdio.h>     /* para printf en depurarión */
#include <string.h>    /* para funciones de strings  */
#include <stdlib.h>    /* Funciones malloc(), free(), y valor NULL */
#include <fcntl.h>     /* Modos de apertura de función open()*/
#include <sys/stat.h>  /* Permisos función open() */
#include <sys/types.h> /* Definiciones de tipos de datos como size_t*/
#include <unistd.h>    /* Funciones read(), write(), close()*/
#include <errno.h>     /* COntrol de errores (errno) */
#include <limits.h>

#define DEBUG 0

#define COLOR_RESET "\x1b[0m"
#define GREEN       "\x1b[92m"
#define CYAN        "\x1b[96m"
#define YELLOW      "\x1b[93m"
#define MAGENTA     "\x1b[95m"

#define FALLO -1
#define EXITO 0

#define NUM_THREADS 10
#define N 1000000

//declaraciones funciones libreria string
size_t my_strlen(const char *str);
int my_strcmp(const char *str1, const char *str2);
char *my_strcpy(char *dest, const char *src);
char *my_strncpy(char *dest, const char *src, size_t n);
char *my_strcat(char *dest, const char *src);
char *my_strchr(const char *s, int c);

// char *my_strncat(char *dest, const char *src, size_t n);

//structuras para gestor de pila
struct my_stack_node {      // nodo de la pila (elemento)
    void *data;
    struct my_stack_node *next;
};

struct my_stack {   // pila
    int size;       // tamaño de data, nos lo pasarán por parámetro
    struct my_stack_node *top;  // apunta al nodo de la parte superior
};  

//declaraciones funciones gestor de pila
struct my_stack *my_stack_init(int size);
int my_stack_push(struct my_stack *stack, void *data);
void *my_stack_pop(struct my_stack *stack);
int my_stack_len(struct my_stack *stack);
int my_stack_purge(struct my_stack *stack); 
struct my_stack *my_stack_read(char *filename);
int my_stack_write(struct my_stack *stack, char *filename);

//stack_counter.c
void read_stack(struct my_stack *stack);
struct my_stack *stack_create(char *filename);
void fill();

    
