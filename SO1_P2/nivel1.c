#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 1
#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define PROMPT '$'

#define RED         "\x1b[91m"
#define GREEN       "\x1b[92m"
#define YELLOW      "\x1b[93m"
#define BLUE        "\x1b[94m"
#define MAGENTA     "\x1b[95m"
#define CYAN        "\x1b[96m"
#define WHITE       "\x1b[97m"
#define COLOR_RESET "\x1b[0m"

#define BLOND "\x1b[1m"

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
    printf(BLOND RED "%s", BLUE "%s " COLOR_RESET YELLOW "%c: " COLOR_RESET, user, prompt, PROMPT);

    free(prompt);

    fflush(stdout);

}

char *read_line(char *line)
{
    print_prompt();

    if (fgets(line, COMMAND_LINE_SIZE, stdin) == NULL)
    {
        perror("Error al leer la linea");
    }
    
    return line;
}

int execute_line(char *line)
{

}

int parse_args(char **args, char*line)
{

}

int check_internal(char **args)
{
    
}

int internal_cd(char **args)
{

}

int internal_export(char **args)
{

}
int internal_source(char **args)
{

}

int internal_jobs()
{

}

int internal_fg(char **args)
{

}

int internal_bg(char **args)
{
    
}