#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;

    while (1) {
        printf("hello: ");
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        i = 0;
        token = strtok (command," ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;
        /* supportion in command exit */
        if (strcmp(argv[0], "exit")==0){
            printf("bye bye.\n");
            return 1;
        }
        /* got one argument command from user */
        if(i==1) {
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                execvp(argv[0], argv);
            }
            if (pid>0){
                wait(NULL);
                continue;
            }
        }
        if(strcmp(argv[1],">")==0){
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                int file_des = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (file_des == -1) {
                    printf("error while opening the file %s",argv[2]);
                    continue;
                }
                argv[1]=NULL;
                dup2(file_des,1);

                execvp(argv[0], argv);
                close(file_des);
            }
            if (pid>0){
                wait(NULL);
                continue;
            }


        }


    }
    return 1;


}
