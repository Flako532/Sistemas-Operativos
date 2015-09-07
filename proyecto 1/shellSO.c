#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// tomo la cadena cmd y usando strsep() la separo por cada espacio " " en distintos parametros que se guardaran en el arreglo params
void arregloCmd(char* cmd, char** params)
{       
    for(int i = 0; i < 10; i++) 
    {
        params[i] = strsep(&cmd, " ");
        
        if(params[i] == NULL) 
            break;
    }
}

int ejecutaCmd(char** params)
{
    // se hace un fork ya que al ejecutar los comandos usando execvp se elimina el proceso una vez terminada la ejecucion
    // y es por eso que al proceso hijo es a quien se elimina una vez ejecutado en execvp
    pid_t pid = fork();
            
    // mensaje de error en caso de que el fork falle
    if (pid == -1) 
    {
        char* error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    // el proceso hijo entra en acción
    else if (pid == 0) 
    {
        // ejecutar comandos
        execvp(params[0], params);  

        // mensaje de error en caso de que el comando a ejecutar no corresponda con alguno de los directorios de UNIX
        char* error = strerror(errno);
        printf("shell: %s: %s\n", params[0], error);
        return 0;
    }

    // si es el proceso padre solo queda esperar, para asi no cortar el proceso luego de ser ejecutado
    else 
    {
        // esperar a que termine el proceso hijo para continuar
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}

int main()
{
    char cmd[101];
    char* params[11];

    while(1) 
    {
        // imprimir linea de texto para la terminal
        char* username = getenv("USER");
        printf("MiPrimeraTerminal@%s~~> ", username);

        // leer comandos por teclado
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) 
            break;

        // quitar el "enter" o "nueva linea" en caso de quedar en el arreglo
        if(cmd[strlen(cmd)-1] == '\n') 
        {
            cmd[strlen(cmd)-1] = '\0';
        }

        // enviar la cadena de texto a la funcion implementada mas arriba ^ para separarla en parametros individuales
        arregloCmd(cmd, params);

        // salir de la terminal voluntariamente
        if(strcmp(params[0], "exit") == 0) 
            break;

        if(strcmp(params[0], "help") == 0) 
        {
            printf("\n\nBienvenido a MiPrimeraTerminal, la terminal creada por mi para ti\n\n");
            printf("->solo debes escribir comandos relativos a UNIX!!!\n");
            printf("->comandos de BASH (como 'cd') no son funcionales!\n");
            printf("->si escribes 'exit' estaras diciendole adios a MiPrimeraTerminal, pues con eso cierras la terminal\n");
            printf("->si escribes 'help' podras ver la ayuda (que por alguna razon misteriosa ya estas viendo ahora...)\n");
            printf("->disfruta tu estadia en MiPrimeraTerminal! pues es la mejor terminal creada por mi para ti\n\n\n");
        }

        // si hubo un error en la ejecución de los comandos, salir de la terminal
        if(ejecutaCmd(params) == 0) 
            break;
    }

    return 0;
}

