#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define DEBUG 0
#define DEBUG3 1
#define DEBUG4 1
#define DEBUGE 0
#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define PROMPT '$'
#define EXITO 0
#define FALLO -1
#define N_JOBS 64

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

struct info_job {
   pid_t pid;
   char estado; // ‘N’, ’E’, ‘D’, ‘F’ (‘N’: Ninguno, ‘E’: Ejecutándose y ‘D’: Detenido, ‘F’: Finalizado) 
   char cmd[COMMAND_LINE_SIZE]; // línea de comando asociada
};

//Variables
static struct info_job jobs_list[N_JOBS];
static char mi_shell[COMMAND_LINE_SIZE];

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
void reaper(int signum);
void ctrlc(int signum);



int main(int argc, char *argv[])
{
    
    strcpy(mi_shell, argv[0]);
    char line[COMMAND_LINE_SIZE];

    jobs_list[0].pid = 0;
    jobs_list[0].estado = 'N';
    memset(jobs_list[0].cmd, '\0', strlen(jobs_list[0].cmd));



    signal(SIGCHLD, reaper);
    signal(SIGINT, ctrlc);

    while (1)
    {
        if (read_line(line))
        {
            execute_line(line);
        }
        
    }
    
    return 0;
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
    print_prompt();

    char *ptr = fgets(line, COMMAND_LINE_SIZE, stdin);

    if(ptr){
        line[strlen(line) - 1] = '\0';
    }
    else{
        printf("'\r'");
        if (feof(stdin))
        {
            fprintf(stderr,"Bye bye");
            exit(0);
        }
        
    }
    return ptr;
}



int execute_line(char *line) {
    char **args = malloc(sizeof(char *) * ARGS_SIZE);

    char cmd[COMMAND_LINE_SIZE];

    strcpy(cmd, line);

    if (args == NULL) {
        fprintf(stderr, RED "Error: Memoria dinamica insuficiente" COLOR_RESET);
        return EXITO;
    }

    int num_args = parse_args(args, line);

    if (num_args > 0) {
        
        if (!check_internal(args)) {

            pid_t pid = fork();

            if (pid < 0) {
                // Fork error
                perror("fork");
                free(args);
                return EXITO;
            }

            if (pid == 0) {

                signal(SIGCHLD, SIG_DFL);
                signal(SIGINT, SIG_IGN);

                printf(COLOR_RESET);
                if (execvp(args[0], args) == FALLO)
                {
                    perror("execvp error");
                    fflush(stderr);
                    exit(FALLO);
                }
                
                
            } else {
                
                jobs_list[0].pid = pid;
                jobs_list[0].estado = 'E';
                strcpy(jobs_list[0].cmd, cmd);

#if DEBUG3
                printf(GRAY"[execute_line() -> PID padre: %d (%s)]\n"COLOR_RESET, getppid(), mi_shell);
                printf(GRAY"[execute_line() -> PID hijo: %d (%s)]\n"COLOR_RESET, getpid(), jobs_list[0].cmd);
#endif
                    while (jobs_list[0].pid > 0)
                    {
                        pause();
                    }
            }
        }
    }

    

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

#if DEBUG
    printf(GRAY"Numero de tokens: %d\n"COLOR_RESET, token_counter);
#endif
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
            perror(RED"Error cambiando al directorio HOME"COLOR_RESET);
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



#if DEBUG
    char *valorInicial = getenv(nombre);
    printf(GRAY"[internal_export() -> nombre: %s]\n", nombre);
    printf("[internal_export() -> valor: %s]\n", valor);
    printf("[internal_export() -> antiguo valor para %s: %s]\n"COLOR_RESET, nombre, valorInicial);
#endif



setenv(nombre, valor, 1);

#if DEBUG
char *nuevoValor = getenv(nombre);
printf(GRAY"[internal_export() -> nuevo valor para %s: %s]\n"COLOR_RESET, nombre, nuevoValor);
#endif

    return EXITO;
}



int internal_source(char **args)
{

    if (args[1] == NULL)
    {
        fprintf(stderr, RED"Error de sintaxis. Uso: source <nombre_fichero>\n"COLOR_RESET);
        return FALLO;
    }
    
    FILE *file = fopen(args[1], "r");
    if (file == NULL)
    {
        perror(RED"fopen error:"COLOR_RESET);
        return FALLO;
    }

    char line[COMMAND_LINE_SIZE];

    while (fgets(line, COMMAND_LINE_SIZE, file) != NULL)
    {
        size_t length = strlen(line);
        if (length > 0 && line[length -1] == '\n')
        {
            line[length -1] = '\0';
        }
        
#if DEBUG
    printf(GRAY"[internal_source() -> LINE: %s]\n"COLOR_RESET, line);
#endif

        execute_line(line);

    }
    
    fclose(file);

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

void reaper(int signum) {
    signal(SIGCHLD, reaper);

    int status;
    pid_t ended;

    while ((ended = waitpid(-1, &status, WNOHANG)) > 0) {
        if (ended == jobs_list[0].pid) {
            jobs_list[0].pid = 0;
            jobs_list[0].estado = 'F';
            
            if (WIFEXITED(status))
            {
#if DEBUG4
                printf(GRAY"[reaper() -> Proceso hijo %d (%s) finalizado con exit code %d]\n"COLOR_RESET,ended, jobs_list[0].cmd, WEXITSTATUS(status));
#endif
            }
            else if (WIFSIGNALED(status))
            {
#if DEBUG4
                printf(GRAY"[reaper() -> Proceso hijo %d (%s) finalizado con exit code %d]\n"COLOR_RESET, ended, jobs_list[0].cmd, WTERMSIG(status));
#endif
            }
            memset(jobs_list[0].cmd, '\0', strlen(jobs_list[0].cmd));
        }
    }
}

void ctrlc(int signum)
{
    signal(SIGINT, ctrlc);

#if DEBUG4
            printf(GRAY"[ctrlc() -> Soy el proceso con PID %d (%s), el proceso en foreground es %d (%s)]"COLOR_RESET, getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);
#endif

    if(jobs_list[0].pid > 0)
    {
        if (strcmp(jobs_list[0].cmd, mi_shell))
        {
            kill(jobs_list[0].pid, SIGTERM);
        }
        else
        {
#if DEBUG4
            fprintf(stderr, GRAY"\n[ctrlc() -> Señal %d no enviada por %d (%s) debido a que el proceso en foreground es el minishell]\n"COLOR_RESET, SIGTERM, getpid(), mi_shell);
#endif
        }
        
    }
    else
    {
#if DEBUG4
        fprintf(stderr, GRAY"\n[ctrlc() -> Señal %d no enviada por %d (%s) debido a que no hay proceso en foreground]\n"COLOR_RESET, SIGTERM, getpid(), mi_shell);
#endif
    }
    printf("\n");
    fflush(stdout);
}