#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG 1
#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define PROMPT '$'
#define EXITO 0
#define FALLO -1

#define GRAY        "\x1b[90m"
#define RED         "\x1b[91m"
#define GREEN       "\x1b[92m"
#define YELLOW      "\x1b[93m"
#define BLUE        "\x1b[94m"
#define MAGENTA     "\x1b[95m"
#define CYAN        "\x1b[96m"
#define WHITE       "\x1b[97m"
#define COLOR_RESET "\x1b[0m"
#define BLOND       "\x1b[1m"

char line[COMMAND_LINE_SIZE];

//Functions
char *read_line(char *line);
int execute_line(char *line);
int parse_args(char **args, char *line);
int check_internal(char **args);
int internal_cd(char **args);
int internal_export(char **args);
int internal_source(char **args);
int internal_jobs();
int internal_fg(char **args);
int internal_bg(char **args);
int internal_exit(char **args);

int main()
{
    while (1)
    {
        if (read_line(line))
        {
            execute_line(line);
        }
        
    }
    
}



void print_prompt()
{

    //Get USER
    char *user = getenv("USER");

    char *prompt = malloc(sizeof(char) * COMMAND_LINE_SIZE);

    //Get directory
    getcwd(prompt, COMMAND_LINE_SIZE);
    
    //Print the prompt
    printf(BLOND RED "%s:" BLUE "%c" COLOR_RESET GREEN "%s: " COLOR_RESET, user, PROMPT, prompt);

    free(prompt);

    fflush(stdout);

}



char *read_line(char *line)
{
    int s = 0;
    print_prompt();

    if (fgets(line, COMMAND_LINE_SIZE, stdin) == NULL)
    {
        perror("Error al leer la linea");
    }
    else
    {
        while (line[s])
        {
            if (line[s] == '\n')
            {
                line[s] = '\0';
            }
            s++;
        }
    }
    return line;
}



int execute_line(char *line)
{
    char **args = malloc(sizeof(char *) * ARGS_SIZE);
    if (args == NULL)
    {
        fprintf(stderr, RED"Error: Memoria dinamica insuficiente"COLOR_RESET);
    }
    
    if (args)
    {
        parse_args(args, line);
        if (args[0])
        {
            if (!check_internal(args))
            {
#if DEBUG
            printf("Commando externo\n");
#endif
            }
            
        }
    }
    free(args);
    
    return EXITO;
}



int parse_args(char **args, char*line)
{
    const char deLimiters[] = " \t\n";
    const char comment_char = '#';

    char *token = strtok(line, deLimiters);
    int token_counter = 0;

    while (token != NULL)
    {

#if DEBUG
        printf(GRAY"[Parse_args() -> token %d: %s]\n"COLOR_RESET, token_counter, token);
#endif
        if (token[0] == comment_char)
        {
            args[token_counter] = NULL;
            break;
        }

        args[token_counter] = token;

#if DEBUG
        printf(GRAY"[parse_args() -> token %d corregido: %s]\n"COLOR_RESET, token_counter, token);
#endif
        token = strtok(NULL, deLimiters);
        token_counter++;
    }

    args[token_counter] = NULL;

    printf(GRAY"Numero de tokens: %d\n"COLOR_RESET, token_counter);

    return token_counter;
    
}



int check_internal(char **args)
{
    int internal = 0;

    char cd[] = "cd";
    char export[] = "export";
    char source[] = "source";
    char jobs[] = "jobs";
    char fg[] = "fg";
    char bg[] = "bg";
    char exit[] = "exit";

    if (strcmp(args[0], cd) == 0)
    {
        internal_cd(args);
        internal = 1;
    }
    
    if (strcmp(args[0], export) == 0)
    {
        internal_export(args);
        internal = 1;
    }

    if (strcmp(args[0], source) == 0)
    {
        internal_source(args);
        internal = 1;
    }

    if (strcmp(args[0], jobs) == 0)
    {
        internal_jobs(args);
        internal = 1;
    }
    
    if (strcmp(args[0], fg) == 0)
    {
        internal_fg(args);
        internal = 1;
    }

    if (strcmp(args[0], bg) == 0)
    {
        internal_bg(args);
        internal = 1;
    }

    if (strcmp(args[0], exit) == 0)
    {
        internal_exit(args);
        internal = 1;
    }
    

    return internal;
}



int internal_cd(char **args)
{
    char *home_dir = getenv("HOME");

    if (args[1] == NULL) {
        if (home_dir == NULL) {
            fprintf(stderr, RED"cd: HOME environment variable not set\n"COLOR_RESET);
            return FALLO; // Return -1 to indicate an error
        }

        if (chdir(home_dir) != 0) {
            perror(RED"chdir() error:"COLOR_RESET);
            return FALLO; // Return -1 to indicate an error
        }

#if DEBUG
        printf(GRAY"[internal_cd() -> PWD: %s]\n"COLOR_RESET, home_dir);    
#endif
    } else {
        // Change directory to the specified path
        if (chdir(args[1]) != 0) {
            perror(RED"chdir() error:"COLOR_RESET);
            return FALLO; // Return -1 to indicate an error
        }
#if DEBUG
        printf(GRAY"[internal_cd() -> PWD %s]\n"COLOR_RESET,args[1]);
#endif

    }
    return EXITO;
}
 

int internal_export(char **args)
{
#if DEBUG
    printf(" \n");
#endif

if (args[1] == NULL)
{
    fprintf(stderr, RED"Sintaxis intcorrecta. Uso: export NOMBRE=VALOR\n"COLOR_RESET);
    return FALLO;
}

//Descompone el argumento en nombre y valor
char *arg = args[1];
char *nombre = arg;
char *valor = NULL;


while (*arg != '\0')
{
    if (*arg == '=')
    {
        *arg = '\0';        //Establece el final del nombre
        valor = arg + 1;    //Apunta al comienzo del valor
        break;
    }
    arg++;
}

if (valor == NULL)
{
    fprintf(stderr, RED"Error de sintaxis. Uso: export NOMBRE=VALOR\n"COLOR_RESET);
    return FALLO;
}

char *valorInicial = getenv(nombre);

#if DEBUG
    printf(GRAY"[internal_export() -> nombre: %s]\n", nombre);
    printf("[internal_export() -> valor: %s]\n", valor);
    printf("[internal_export() -> antiguo valor para %s: %s]\n"COLOR_RESET, nombre, valorInicial);
#endif



setenv(nombre, valor, 1);

char *nuevoValor = getenv(nombre);

#if DEBUG
printf(GRAY"[internal_export() -> nuevo valor para %s: %s]\n"COLOR_RESET, nombre, nuevoValor);
#endif

    return EXITO;
}



int internal_source(char **args)
{

    
#if DEBUG
    printf(GRAY"[internal_source() -> Esta funcion ejecutara un fichero de lineas de comando]\n"COLOR_RESET);
#endif
    return EXITO;
}   



int internal_jobs()
{
#if DEBUG
    printf(GRAY"[internal_jobs() -> Esta funcion mostrara el PID de los procesos que no esten en foreground]\n"COLOR_RESET);
#endif

    return 0;
}



int internal_fg(char **args)
{
#if DEBUG
    printf(GRAY"[internal_fg() -> Esta funcion pasara a primer plano procesos]\n"COLOR_RESET);
#endif

    return 0;
}



int internal_bg(char **args)
{
#if DEBUG
    printf(GRAY"[internal_bg() -> Esta funcion pasara a segundo plano procesos]\n"COLOR_RESET);
#endif
    return 0;
}



int internal_exit(char **args)
{
    
#if DEBUG
    printf(GRAY"[internal_exit() -> Esta funcion sale del minishell]\n"COLOR_RESET);
#endif

    printf("Bye Bye\n");
    exit(0);
    return 0;
}
