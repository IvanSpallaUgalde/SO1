/*
    Practica 3 Sistemas Operativos 1

    Nombre participantes del trabajo:
    -Ivan Spalla Ugalde
    -Marc Nadal Sastre Gondar
    -Veimar Israel Flores Rios

    Observaciones: 
    Se ha modificado int my_stack_purge(struct my_stack *stack) de my_lib.c. 
    Se ha eliminado la siguiente linea de codigo:
    cantByt = cantByt + sizeof(struct my_stack);

    Ya que daba un resultado incorrecto a la hora de contar los Bytes liberados

    Y los includes
*/
#include <pthread.h>
#include "my_lib.h"

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
void read_stack();
struct my_stack *stack_create(char *filename);
void fill();


static struct my_stack *stack;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Semáforo

// Función para ejecutar los hilos
void *worker(void *ptr)
{
    for (int i = 0; i < N; ++i)
    {
        pthread_mutex_lock(&mutex);

#if DEBUG
        pthread_t TID = pthread_self();
        printf(YELLOW "Soy el hilo %lu ejecutando pop\n" COLOR_RESET, TID);
#endif
        int *data;
        data = my_stack_pop(stack);

        pthread_mutex_unlock(&mutex);

        (*data)++;
#if DEBUG
        sleep(0.001);
#endif
        pthread_mutex_lock(&mutex);
#if DEBUG
        printf(MAGENTA "Soy el hilo %lu ejecutando push\n" COLOR_RESET, TID);
#endif
        my_stack_push(stack, data);
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

    printf("Threads: %d, Iterations: %d\n", NUM_THREADS, N);

    stack = stack_create(filename);

    struct my_stack *aux = my_stack_init(stack->size);
    int *dato;

    if (my_stack_len(stack) < NUM_THREADS)
    {
        for (int i = 0; i > NUM_THREADS - my_stack_len(stack); i++)
        {
            int *data = malloc(sizeof(int));
            *data = 0;
            my_stack_push(stack, data);
        }
    }
    else if (my_stack_len(stack) > NUM_THREADS)
    {
        while (my_stack_len(stack) != NUM_THREADS)
        {
            dato = my_stack_pop(stack);
            my_stack_push(aux, dato);
        }
    }

    // Creamos los hilos
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker, NULL);
        printf(CYAN "%d) Thread %ld created\n" COLOR_RESET, i, threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    if (my_stack_len(aux) != 0)
    {
        while (my_stack_len(aux) != 0)
        {
            dato = my_stack_pop(aux);
            my_stack_push(stack, dato);
        }
    }

    printf("\nstack content after threads iterations\n");
    read_stack();
    printf("stack length: %d\n", my_stack_len(stack));

    int elements = my_stack_write(stack, filename);
    printf("\nWritten elements form stack to file: %d\n", elements);

    int nbytes = my_stack_purge(stack);
    printf("Released Bytes: %d\n", nbytes);

    printf("Bye from main\n");
    pthread_exit(NULL);

    return EXITO;
}

void read_stack()
{
    struct my_stack *aux = my_stack_init(stack->size);
    while (my_stack_len(stack) != 0)
    {
        int *dato;
        dato = my_stack_pop(stack);
        my_stack_push(aux, dato);
    }

    while (my_stack_len(aux) != 0)
    {
        int *dato;
        dato = my_stack_pop(aux);
        printf("%d\n", *(dato));
        my_stack_push(stack, dato);
    }
}

struct my_stack *stack_create(char *filename)
{

    stack = my_stack_read(filename);

    if (stack)
    {
        if (my_stack_len(stack) < NUM_THREADS)
        {
            printf("initial stack length: %d\n", my_stack_len(stack));
        }
        printf("original stack content: \n");
        read_stack();

        if (my_stack_len(stack) < NUM_THREADS)
        {
            fill();
        }
        else
        {
            printf("original stack length: %d\n\n", my_stack_len(stack));
        }
    }
    else
    {
        stack = my_stack_init(sizeof(int));
        printf("Stack->size: %ld\n", sizeof(int));
        printf("initial stack length: 0\n");
        printf("initial stack content: \n");
        fill();
        printf("new stack length: %d\n\n", my_stack_len(stack));
    }

    return stack;
}

void fill()
{
    struct my_stack *aux = my_stack_init(stack->size);
    int filled = 0;
    int cont = 0;
    int *data = malloc(sizeof(int));
    if (data == NULL)
    {
        fprintf(stderr, "No hay espacio en memoria\n");
        return;
    }

    while (my_stack_len(stack) != 0)
    {
        data = malloc(sizeof(int));
        if (data == NULL)
        {
            fprintf(stderr, "No hay espacio en memoria\n");
            return;
        }
        data = my_stack_pop(stack);
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
        cont++;
    }

    if (filled)
    {
        if (cont < NUM_THREADS)
        {
            printf("\nNumber of elements added to initial stack: %d\n", cont);
        }

        while (my_stack_len(aux) != 0)
        {
            data = malloc(sizeof(int));
            if (data == NULL)
            {
                fprintf(stderr, "No hay espacio en memoria\n");
                return;
            }

            data = my_stack_pop(aux);
            my_stack_push(stack, data);
        }
        printf("stack content for treatment:\n");
        read_stack();
        if (cont < NUM_THREADS)
        {
            printf("stack length: %d\n\n", my_stack_len(stack));
        }
    }
    else
    {
        printf("original stack content: \n");
        read_stack();
    }
}