#include <stdio.h>
#include "my_lib.h"

int main() {
    my_stack_read();
    int stack[STACK_SIZE];

    //Inicializamos las variables
    int suma = 0;
    int minimo = INT_MAX;
    int maximo = INT_MIN;
    int contador = 0;

    //Imprimimos los valores de la pila y realizamos cálculos
    for (int i = 0; i < STACK_SIZE; ++i) {
        printf("Valor %d: %d\n", i + 1, stack[i]);
        suma += stack[i];
        if (stack[i] < minimo) {
            minimo = stack[i];
        }
        if (stack[i] > maximo) {
            maximo = stack[i];
        }
        contador++;
    }

    //Imprimimos los resultados de los cálculos
    printf("Suma: %d\n", suma);
    printf("Mínimo: %d\n", minimo);
    printf("Máximo: %d\n", maximo);
    printf("Media: %.2f\n", (float)suma / contador);

    return 0;
}