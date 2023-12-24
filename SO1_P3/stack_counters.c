#include <pthread.h>
//#define MAX_SIZE 10 // Tamaño máximo de la pila (10)

/*
struct Stack {
    int data[MAX_SIZE];
    int top;
}

void initStack(struct Stack *stack) {
    stack->top = -1;
}

int isEmpty(struct Stack *stack) {
    return stack->top == -1;
}

int isFull(struct Stack *stack) {
    return stack->top == MAX_SIZE - 1;
}

int peek(struct Stack *stack) {
    return stack->data[stack->top];
}

int pop(struct Stack *stack) {
    if (!isEmpty(stack)) {
        int value = stack->data[stack->top];
        stack->top--;
        return value;
    }
    return INT_MIN; // Valor mínimo para indicar error (pila vacía)
}

void push(struct Stack *stack, int value) {
    if (!isFull(stack)) {
        stack->top++;
        stack->data[stack->top] = value;
    }
}
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 10
#define STACK_SIZE 10

// Suponiendo que tienes estas funciones definidas
void my_stack_init();
int my_stack_len();
void my_stack_push(int *data);
int *my_stack_pop();
void my_stack_purge();
void my_stack_read(FILE *file);
void my_stack_write(FILE *file);

// Función para crear o verificar la pila
void prepare_stack(char *filename) {
    // Verificar si se proporcionó un nombre de archivo
    if (filename == NULL) {
        printf("Error: Sintaxis incorrecta. Uso: ./stack_counters <nombre_fichero>\n");
        return;
    }

    // Verificar si la pila existe en el archivo
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // Si no existe, crearla e inicializarla
        my_stack_init();
        printf("Pila creada y inicializada.\n");
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error al abrir el archivo para escritura.\n");
            return;
        }
        // Agregar punteros a cero en la pila hasta llegar a STACK_SIZE
        for (int i = 0; i < STACK_SIZE; ++i) {
            int *data = (int *)malloc(sizeof(int)); // Reservar espacio para el entero
            *data = 0; // Inicializar a cero
            my_stack_push(data);
            fprintf(file, "%p\n", (void *)data); // Guardar la dirección en el archivo
        }
        fclose(file);
        printf("Pila inicializada con punteros a cero.\n");
    } else {
        // Si existe, leer la pila y verificar su tamaño
        my_stack_read(file);
        fclose(file);
        int stack_len = my_stack_len();
        if (stack_len < STACK_SIZE) {
            // Agregar punteros a cero hasta STACK_SIZE
            file = fopen(filename, "a");
            if (file == NULL) {
                printf("Error al abrir el archivo para escritura.\n");
                return;
            }
            for (int i = 0; i < STACK_SIZE - stack_len; ++i) {
                int *data = (int *)malloc(sizeof(int));
                *data = 0;
                my_stack_push(data);
                fprintf(file, "%p\n", (void *)data);
            }
            fclose(file);
            printf("Pila actualizada con punteros a cero.\n");
        } else if (stack_len > STACK_SIZE) {
            printf("La pila contiene más de %d elementos. Ignorando elementos adicionales.\n", STACK_SIZE);
        } else {
            printf("La pila ya tiene %d elementos.\n", STACK_SIZE);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Sintaxis incorrecta. Uso: ./stack_counters <nombre_fichero>\n");
        return 1;
    }

    char *filename = argv[1];
    prepare_stack(filename);

    return 0;
}
