#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_lib.h"

struct my_stack *stack;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Semáforo

// Función para ejecutar los hilos
void *worker(void *ptr)
{
    pthread_t TID = pthread_self();

#if DEBUG
    printf(CYAN "Thread %u created\n" COLOR_RESET, TID);
#endif
    for (int i = 0; i < N; ++i)
    {
        pthread_mutex_lock(&mutex);

        int *data;
        data = my_stack_pop(stack);
#if DEBUG
        printf(YELLOW "Soy el hilo %u ejecutando pop" COLOR_RESET, TID);
#endif
        pthread_mutex_unlock(&mutex);

        (*data)++;

        pthread_mutex_lock(&mutex);
        my_stack_push(stack, data);
#if DEBUG
        printf(MAGENTA "Soy el hilo %u ejecutando push" COLOR_RESET, TID);
#endif
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf(GREEN "USAGE: ./stack_counters <nombre_fichero>\n" COLOR_RESET);
        return FALLO;
    }

    char *filename = argv[1];

    if (my_stack_len(stack) < NUM_THREADS)
    {
        for (int i = 0; i > NUM_THREADS - my_stack_len(stack); i++)
        {
            int *data = malloc(sizeof(int));
            *data = 0;
            my_stack_push(stack, data);
        }
    }

    // Creamos los hilos
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    my_stack_write(stack, filename);

    int nbytes = my_stack_purge(stack);

    pthread_exit(NULL);

    return EXITO;
}

struct my_stack stack_create(char *filename)
{

    stack = my_stack_read(filename);

    if (stack)
    {
        printf("initial stack length: %d\n", my_stack_len(stack));

        if (my_stack_len(stack) < NUM_THREADS)
        {
            fill();
        }
        printf("new stack length: %d\n", my_stack_len(stack));
    }
    else
    {
        stack = my_stack_init(sizeof(int));
        printf("Stack->size: %ld\n", sizeof(int));
        printf("initial stack length: 0\n");
        fill();
        printf("new stack length: %d\n", my_stack_len(stack));
    }

    return *stack;
}

void fill()
{
    struct my_stack *aux = my_stack_init(stack->size);
    int filled = 0;
    int *data = malloc(sizeof(int));
    if (data == NULL)
    {
        fprintf(stderr, "No hay espacio en memoria\n");
        return;
    }

    printf("initial stack content: \n");

    while (my_stack_len(stack) != 0)
    {
        data = malloc(sizeof(int));
        if(data == NULL)
        {
            fprintf(stderr, "No hay espacio en memoria\n");
            return;
        }
        data = my_stack_pop(stack);
        printf("%d\n", *data);
        my_stack_push(aux, data);
    }
    
    while (my_stack_len(aux) < NUM_THREADS)
    {
        data = malloc(sizeof(int));
        if (data == NULL)
        {
            fprintf(stderr, "No hay espacio en memoria\n");
            return;
        }
        
        filled = 1;
        *data = 0;
        my_stack_push(aux, data);
    }
    
    if (filled)
    {
        while (my_stack_len(aux) != 0)
        {
            data = malloc(sizeof(int));
            if(data == NULL)
            {
                fprintf(stderr, "No hay espacio en memoria\n");
                return;
            }

            data = my_stack_pop(aux);
            printf("%d\n",*data);
            my_stack_push(stack, data);
        }
        
    }
}