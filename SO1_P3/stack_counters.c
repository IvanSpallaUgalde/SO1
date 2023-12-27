#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_lib.h"

#define MAX_THREADS 10
#define STACK_SIZE 10

int *stack[STACK_SIZE];                             //Pila
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //Semáforo


//Función para crear o verificar la pila
void prepare_stack(char *filename) {
    // Verificar si se proporcionó un nombre de archivo
    if (filename == NULL) {
        printf("Error: Sintaxis incorrecta. Uso: ./stack_counters <nombre_fichero>\n");
        return;
    }

    //Verificar si la pila existe en el archivo
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        //Si no existe, crearla e inicializarla
        my_stack_init();                                ////////////////¿Aquí no falta que una pila reciba el resultado de my_stack_init()?
        printf("Pila creada y inicializada.\n");
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error al abrir el archivo para escritura.\n");
            return;
        }
        //Agregar punteros a cero en la pila hasta llegar a STACK_SIZE
        for (int i = 0; i < STACK_SIZE; ++i) {
            int *data = (int *)malloc(sizeof(int)); //Reservar espacio para el entero
            *data = 0; // Inicializar a cero
            my_stack_push(data);
            fprintf(file, "%p\n", (void *)data); //Guardar la dirección en el archivo
        }
        fclose(file);
        printf("Pila inicializada con punteros a cero.\n");
    } else {
        //Si existe, leer la pila y verificar su tamaño
        my_stack_read(file);
        fclose(file);
        int stack_len = my_stack_len();
        if (stack_len < STACK_SIZE) {
            //Agregar punteros a cero hasta STACK_SIZE
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



//Función para crear los hilos
void create_threads(pthread_t *threads) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, NULL, NULL) != 0) {
            perror("Error al crear el hilo");  /////////////////////////////////MANEJAR ERRORES BIEN!!!!/////////////////        
        }
    }
}

//Función para ejecutar los hilos
void *worker(void *ptr) {
    for (int i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&mutex);
        int *data = my_stack_pop();
        if (data == NULL) {
            fprintf(stderr, "Error al extraer dato de la pila\n");
            exit(EXIT_FAILURE);
        }
        (*data)++;
        my_stack_push(data);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}




int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Sintaxis incorrecta. Uso: ./stack_counters <nombre_fichero>\n");
        return 1;
    }

    char *filename = argv[1];
    prepare_stack(filename);

    pthread_t threads[MAX_THREADS]; //Array para almacenar el PID de los hilos
    create_threads(threads);        //Creamos los hilos


    for (int i = 0; i < MAX_THREADS; ++i) {  //Esperamos a que acaben los hilos
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar a que el hilo termine");
                                                            //////////////////////////////GESTIÓN DE ERRORES!!!///////////////////
        }
    }

    //Volcam la pila en el fichero
    my_stack_write();

    //Liberar el espacio de la pila de la memoria principal
    my_stack_purge();

    // Salir del hilo principal
    pthread_exit(NULL);

    return 0;
}
