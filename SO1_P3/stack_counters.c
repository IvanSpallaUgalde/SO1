#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 10
#define STACK_SIZE 10

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////CREO QUE NO HACE FALTA ESTA PARTE, HAY QUE HACER ALGO PARA USAR FUNCIONES DE OTRO FICHERO, PERO NO SÉ HACERLO////////////////
void my_stack_init();
int my_stack_len();
void my_stack_push(int *data);
int *my_stack_pop();
void my_stack_purge();
void my_stack_read(FILE *file);
void my_stack_write(FILE *file);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
