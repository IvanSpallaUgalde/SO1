/*
    Practica 3 Sistemas Operativos 1

    Nombre participantes del trabajo:
    -Ivan Spalla Ugalde
    -Marc Nadal Sastre Gondar
    -Veimar Israel Flores Rios

*/
#include "my_lib.h"
#include <limits.h>

//Defines

#define DEBUG 0

#define COLOR_RESET "\x1b[0m"
#define GREEN       "\x1b[92m"
#define CYAN        "\x1b[96m"
#define YELLOW      "\x1b[93m"
#define MAGENTA     "\x1b[95m"

#define FALLO -1
#define EXITO 0

#if (DEBUG == 1)
#define NUM_THREADS 3
#define N 5
#endif

#if (DEBUG == 0)
#define NUM_THREADS 10
#define N 1000000
#endif

//Declaracion de funciones
struct my_stack *invert_stack(struct my_stack *stack);


int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        printf(GREEN"USAGE: ./reader <stack>\n"COLOR_RESET);
        return FALLO;
    }

    char *filename = argv[1]; 
    
    struct my_stack *stack = my_stack_read(filename);

    if (stack == NULL)
    {
        printf(GREEN"Couldn't open stack file %s\n"COLOR_RESET, filename);
        return FALLO;
    }
    
    int length = my_stack_len(stack);
    printf("Stack length: %d\n", length);

    //Inicializamos las variables
    int suma = 0;
    int minimo = INT_MAX;
    int maximo = INT_MIN;

    stack = invert_stack(stack);

    //Imprimimos los valores de la pila y realizamos cálculos
    for (int i = 0; i < length; ++i) {

        int *dato;
        dato = my_stack_pop(stack);

        printf("%d\n", *dato);

        suma += *dato;

        if (*(dato) < minimo) {
            minimo = *dato;
        }
        else if (*(dato) > maximo) {
            maximo = *dato;
        }

    }

    //Imprimimos los resultados de los cálculos
    printf("\nItems: %d ", length);
    printf("Sum: ");
    printf(YELLOW"%d "COLOR_RESET,suma);
    printf("Min: %d ", minimo);
    printf("Max: %d ", maximo);
    printf("Average: ");
    printf(YELLOW"%d\n\n"COLOR_RESET, (suma/NUM_THREADS));
    return 0;
}

struct my_stack *invert_stack(struct my_stack *stack)
{
    struct my_stack *aux = my_stack_init(stack->size);
    int *dato;
    while (my_stack_len(stack) != 0)
    {
        dato = my_stack_pop(stack);
        my_stack_push(aux, dato);
    }
    
    return aux;
}