#include "my_lib.h"

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
        printf("Couldn't open stack file %s\n", filename);
        return FALLO;
    }
    

    if (my_stack_len(stack) != NUM_THREADS)
    {
        printf("Stack size is %d, when it has to be %i\n", my_stack_len(stack), NUM_THREADS);
    }
    
    int length = my_stack_len(stack);

    //Inicializamos las variables
    int suma = 0;
    int minimo = INT_MAX;
    int maximo = INT_MIN;

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
    printf("Items: %d ", my_stack_len(stack));
    printf("Sum: ");
    printf(YELLOW"%d "COLOR_RESET,suma);
    printf("Min: %d", minimo);
    printf("Max: %d", maximo);
    printf("Average: ");
    printf(YELLOW"%d\n"COLOR_RESET, (suma/NUM_THREADS));
    return 0;
}