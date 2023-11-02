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


void *my_stack_pop(struct my_stack *stack){
    if (stack.top){ //Si existe el top, operamos
        
        struct my_stack_node *nodAux = stack.top; //Creamos un nodo auxiliar que nos permitirá acabar eliminando el top actual

        void *datos = nodAux.data; //Guardamos los datos en una variable para devolverlos

        stack.top = nodAux.next; //Hacemos que el top sea ahora el siguiente nodo
        free(nodAux); //Eliminamos el nodo que se encontraba anteriormente en el top
        
        return datos;
}
    else{
        return NULL; //Si el top no existe, devolvemos null
    }
}


int my_stack_len(struct my_stack *stack){

    int cantNod = 0;
    struct my_stack_node *nodAux = stack.top; //Copiamos la pila desde el top en una pila auxiliar
    
    while (nodAux){ //Bucle para recorrer toda la pila
        nodAux = nodAux.next; //Pasamos al siguiente nodo
        cantNod++; //+1 nodo encontrado
    }
    
    return cantNod;
}


int my_stack_purge(struct my_stack *stack){
    
    int cantByt = sizeof(struct my_stack); //Bytes de la pila

    while (stack.top){ //Recorremos toda la pila
        cantByt = cantByt + sizeof(struct my_stack_node); //Sumamos los bytes que ocupa un solo nodo (cada vez que iteramos)
        cantByt = cantByt + stack.size;                   //Sumamos los bytes que ocupa 1 solo dato de un nodo (en cada iteración)
        free(my_stack_pop(stack));                        //Eliminamos el top actual de la pila
    }
    
    cantByt = cantByt + sizeof(struct my_stack);    //Acabamos sumando lo que ocupa la propia pila como tal

    free(stack);               //Eliminamos la propia pila
    return cantByt;
}

int bytesWrite(struct my_stack_node *node, int fichero, int size){ //Se utiliza en my_stack_write
    int bytes = 0;
    if (node.next){ //Si el nodo tiene siguiente, iteramos
        bytes = bytesWrite(node.next, fichero, size); //Si tiene siguiente, volvemos a llamar a la función hasta llegar al último
    }

    return write(fichero, node.data, size) + bytes; //Una vez llegado al último nodo(realmente el primero), este se escribe y se devuleven la cantidad de bytes. El
                                                    //proceso se repite consecutivamente hasta llegar al TOS (Top Of Stack)
}

int my_stack_write(struct my_stack *stack, char *filename){
    
    int cantByt = -1;                         //Valor inicial -1 para gestionar errores    
    int fichero = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); //Se abre/crea el fichero sobre el que realizaremos todo el proceso

    if (fichero < 0){ //Comprobamos que el fichero se haya creado
        fprintf(stderr, "No ha sido posible abrir o crear el fichero\n");
        return cantByt;
    }
                                                      
    bytes = bytesWrite(stack.top, fichero, stack.size);     //Utilizamos una función auxiliar para escribir la pila recursivamente
    
    if (cantByt == -1){ //Comprobamos si se han producido errores al escribir
        fprintf(stderr, "Ha ocurrido un error al escribir en el archivo\n");
        return cantByt;
    }

    if (close(fichero) == -1){ //Cerramos el fichero y controlamos un posible error al cerrar el fichero
        fprintf(stderr, "Error al cerrar el fichero\n");
        return -1;
    }
    
    return cantByt / stack.size; //Devolvemos la cantidad de elementos
}


struct my_stack *my_stack_read(char *filename){
    int size;
    struct my_stack *stack;
    void *data;

    // Creamos el enlace al fichero
    int fichero = open(filename, O_RDONLY);

    // Control de errores
    if (fichero < 0){
        fprintf(stderr, "Error al abrir el fichero\n");
        return NULL;
    }

    // Cogemos el tamaño del data del fichero
    read(fichero, &size, sizeof(int));

    // Inicializamos el stack (usando init) y reservamos tamaño para el data
    stack = my_stack_init(size);
    data = malloc(size);
    if (data == NULL){
        fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n");
        return NULL;
    }

    //Bucle para restaurar los nodos
    while (read(fichero, data, size) > 0){
        //Reservamos memoria para el data
        my_stack_push(stack, data);
        data = malloc(size);
        if (data == NULL){
            fprintf(stderr, "No hay espacio en memoria dinámica disponible en este momento.\n");
            return NULL;
        }
    }
    //Cerramos el enlace con el fichero
    if (close(fichero) < 0){
        fprintf(stderr, "Error al cerrar el fichero\n");
    }

    free(data);
    return stack;
}
