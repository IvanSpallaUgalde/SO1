#include "my_lib.h"
#include <stdlib.h>

size_t my_strlen(const char *str){
    int idx = 0; //Índice con el que recorremos el string
    size_t len = 0;

    while (str[idx]){ //Mientras el string en la posición idx sea diferente a /0, entramos
        idx++; //Avanzamos al siguiente
        len++;
    }
    return len;
}

int my_strcmp(const char *str1, const char *str2){
    int idx1 = 0;
    int idx2 = 0;

    while (str1[idx1] && (str1[idx1] == str2[idx2])){
        idx1++;
        idx2++;
    }
    return (str1[idx1] - str2[idx2]);
}

char *my_strcat(char *dest, const char *str){
    int contador = 0;
    
    size_t tam = sizeof(char) * (my_strlen(str) + my_strlen(dest) + 1); //Tamaño = tamaño string, carácteres del destino y carácter nulo
    char *puntero = (char *)malloc(tam); //Creamos un puntero del tamaño que acabamos de definir

    while (*dest){
        contador++; //Incrementamos el contador en 1
        *puntero = *dest; //Copiamos a lo que apunta el destino en lo que apunta el puntero
        puntero++; //Pasamos a la siguiente posición de la pila
        dest++;  //Pasamos a la siguiente posición del destino
    }

    while (*str){ //Al igual que antes, incrementamos las variables en 1 y copiamos lo de str en el puntero
        contador++;
        *puntero = *str;
        puntero++;
        str++;
    }
    
    *puntero = '\0'; //Copiamos el caracter nulo al final del puntero
    
    while (contador > 0){ //Ponemos el puntero de vuelta a su posicón original para devolverlo entero
        contador--;
        puntero--;
    }
    
    dest = puntero;
    return (char *)dest;  //Devolvemos puntero dest
}


char *my_strchr(const char *str, int c){  
    //Mientras str =! 0
    while (*str){
        
        if (*str == c){
            //Casting y return del caracter de str
            return (char *)str;
        }     
        //Siguiente caracter de la cadena str
        str++;
    }
    return 0;
}

char *my_strcpy(char *dest, const char *src){
    int idx = 0; //Índice que declaramos fuera del for para poder introducir el carácter nulo al final
    for (int temp = 0; src[temp]; temp++, idx++){
        dest[idx] = src[idx]; //Copiamos toda la src en el destino
    }
    dest[idx] = '\0'; //Carácter nulo
    return dest; //Devolvemos el destino que contiene todo el source copiado
}


char *my_strncpy(char *dest, const char *src, size_t n){
    
    int len = my_strlen(src); //Longitud del source
    char *pilAux = dest;
    
    if (n <= len){ //Copiamos n caracteres si n es menor a la longitud de la source
        while (n){ 
            *pilAux = *src; //Copiamos el contenido apuntado por source en el contenido apuntado por la pila auxiliar
            n--; //Reducimos la cantidad de carácteres que quedan por copiar en 1
            pilAux++; //Movemos donde apunta la pila 
            src++;    //Movemos donde apunta el source
        }
    }
    else{
        int exceso = n - (len + 1); //Canitdad de carácteres que "nos sobran" y tenemos que rellenar con "\0"
        while (len){ //De mientras la longitud no sea 0, operamos
            len--;
            *pilAux = *src;
            pilAux++;
            src++;
        }
        
        *pilAux = '\0'; //Metemos el carácter nulo
        while (exceso){ //Metemos el carácter nulo en las posiciones restantes
            exceso--;
            pilAux++;
            *pilAux = '\0';
        }
    }
    return dest;
}


struct my_stack *my_stack_init(int size){
    struct my_stack *pila = malloc(sizeof(struct my_stack)); //Creamos la pila que devolveremos 
    
    if (stack == NULL){ //Detectamos si ha habido un error (gestión de errores)
        fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n");
        return NULL;
    }
    pila->size = size; //Declaramos el tamaño de la pila con el size pasado por parámetro
    pila->top = NULL;  //Ponemos el top a null
    return pila;
}


int my_stack_push(struct my_stack *stack, void *data){
    
    int solucion = -1; //Valor de retorno de la función
    
    if (stack){ //Si la pila pasada por parámetro está inicializada, operamos
        if (stack.size > 0){
            struct my_stack_node *nodoNuevo = malloc(stack.size); //Creamos el nuevo nodo de tamaño de los datos de la pila pasada por parámetro
            
            if (nodoNuevo == NULL){ //Si el nodo vale null, devolvemos un error
                fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n"); 
                return solucion; //Solución vale -1
            }

            solucion = 0;
            nodoNuevo.data = data; //Metemos los datos donde toca
            nodoNuevo.next = stack.top; //El siguiente elemento es el top actual
            stack.top = nodoNuevo;  //Y ponemos el nuevo nodo en el top
        }
    }
    return solucion;
}

/*
Elimina el nodo superior de los elementos de la pila (y libera la memoria que ocupaba ese nodo!).
Devuelve el puntero a los datos del elemento eliminado. 

Si no existe nodo superior (pila vacía), retorna NULL.
*/
void *my_stack_pop(struct my_stack *stack)
{
    if (stack->top)
    {
        //Creamos un nodo temporal
        struct my_stack_node *temp_node = stack->top;

        //Copiamos los datos que a continuación se eliminarán
        void *datos = temp_node->data;

        //Asignamos el nuevo "top" del stack y así "borramos" el primer nodo
        stack->top = temp_node->next;

        free(temp_node);
        return datos;
    }
    else
    {
        return NULL;
    }
}

/*
Recorre la pila y retorna el número de nodos totales que hay en los elementos de la pila.
*/
int my_stack_len(struct my_stack *stack)
{
    //Contador de nodos
    int i = 0;
    struct my_stack_node *temp_node = stack->top;

    //Recorrido hasta el final de la pila y aumentamos el i
    while (temp_node)
    {
        temp_node = temp_node->next;
        i++;
    }
    return i;
}

/*
Recorre la pila liberando la memoria que habíamos reservado para cada uno de los datos 
(data) y la de cada nodo. Finalmente libera también la memoria que ocupa la pila. Es decir, 
toda la memoria que se reservó con malloc() en algún momento, se libera con la función free(). 

Devuelve el número de bytes liberados.
*/
int my_stack_purge(struct my_stack *stack)
{
    int bytes = sizeof(struct my_stack); //Bytes de la pila

    //Recorrido por toda la pila
    while (stack->top)
    {
        bytes += sizeof(struct my_stack_node); //Bytes de cada nodo
        bytes += stack->size;                  //Bytes de los datos
        free(my_stack_pop(stack));             //my_stack_pop retorna la posición de los datos
                                               //que eliminamos de la pila
    }

    free(stack); //Liberamos toda la pila
    return bytes;
}

/*
Función auxiliar del write
*/
int aux_my_stack_write(struct my_stack_node *node, int fichero, int size)
{
    // Inicializa a 0 i mira si no es el último nodo
    int bytes = 0;
    if (node->next)
    {
        // Llama de nuevo a la funcion con el nodo como entrada
        bytes = aux_my_stack_write(node->next, fichero, size);
    }

    // Escribe el nodo en el fichero y devuelve los bytes escritos
    return write(fichero, node->data, size) + bytes;
}

/*
Almacena los datos de la pila en el fichero indicado por "filename"
*/
int my_stack_write(struct my_stack *stack, char *filename)
{
    int bytes = -1;
    // Creamos el enlace al fichero
    int fichero = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    //Control de error de open
    if (fichero < 0)
    {
        fprintf(stderr, "Error al abrir el fichero\n");
        return bytes;
    }

    bytes = write(fichero, &stack->size, sizeof(stack->size)); //write(fichero, dato a escribir, tamaño del tado)
                                                               //y nos devuelve los bytes escritos
    bytes = aux_my_stack_write(stack->top, fichero, stack->size);

    // Cerramos el enlace al fichero y devolvemos el número de datos escritos, con control de errores
    if (close(fichero) == -1)
    {
        fprintf(stderr, "Error al cerrar el fichero\n");
        return -1;
    }
    //Control de errores al escribir
    if (bytes == -1)
    {
        fprintf(stderr, "Error al escribir en el archivo\n");
        return bytes;
    }
    else
    {
        return bytes / stack->size;
    }
}

/*
Lee los datos de la pila almacenados en el fichero indicado por "filename"
*/
struct my_stack *my_stack_read(char *filename)
{
    int size;
    struct my_stack *stack;
    void *data;

    // Creamos el enlace al fichero
    int fichero = open(filename, O_RDONLY);

    // Control de errores
    if (fichero < 0)
    {
        fprintf(stderr, "Error al abrir el fichero\n");
        return NULL;
    }

    // Cogemos el tamaño del data del fichero
    read(fichero, &size, sizeof(int));

    // Inicializamos el stack (usando init) y reservamos tamaño para el data
    stack = my_stack_init(size);
    data = malloc(size);
    if (data == NULL)
    {
        fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n");
        return NULL;
    }

    //Bucle para restaurar los nodos
    while (read(fichero, data, size) > 0)
    {
        //Reservamos memoria para el data
        my_stack_push(stack, data);
        data = malloc(size);
        if (data == NULL)
        {
            fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n");
            return NULL;
        }
    }
    //Cerramos el enlace con el fichero
    if (close(fichero) < 0)
    {
        fprintf(stderr, "Error al cerrar el fichero\n");
    }

    free(data);
    return stack;
}
