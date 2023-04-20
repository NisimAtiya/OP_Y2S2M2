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
        /* ignored control+c signal */
        signal(SIGINT,SIG_IGN);
        printf("EN_SHELL: ");
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
        if (i == 0)
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
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
                execvp(argv[0], argv);
            }
            if (pid>0){
                wait(NULL);
                continue;
            }
        }
        /* implementing of command '>' */
        if(strcmp(argv[1],">")==0){
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
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
        /* implementing of command '<' */
        if(strcmp(argv[1],"<")==0){
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
                int file_des = open(argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (file_des == -1) {
                    printf("error while opening the file %s",argv[0]);
                    continue;
                }
                argv[1]=NULL;
                dup2(file_des,1);

                execvp(argv[2], argv);
                close(file_des);
            }
            if (pid>0){
                wait(NULL);
                continue;
            }
        }
        /* implementing of command '>>' */
        if(strcmp(argv[1],">>")==0){
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
                int file_des = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0777);

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
        /* implementing of command '<<' */
        if(strcmp(argv[1],"<<")==0){
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            /* for commands not part of the shell command language */
            if (pid == 0) {
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
                int file_des = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0777);
                if (file_des == -1) {
                    printf("error while opening the file %s",argv[0]);
                    continue;
                }
                argv[1]=NULL;
                dup2(file_des,1);

                execvp(argv[2], argv);
                close(file_des);
            }
            if (pid>0){
                wait(NULL);
                continue;
            }
        }
        if( (strcmp(argv[1],"|")==0) && (i==3) ){
            argv[1]=NULL;
            int pid = fork();
            if (pid == -1) {
                printf("Error: A problem executing the command\n");
                continue;
            }
            if(pid==0){
                /* back to default after control+c signal  */
                signal(SIGINT,SIG_DFL);
                int a[2];
                int r = pipe(a);
                if(r==-1){
                    printf("error: cannot initialing pipe");
                    continue;
                }
                int pid_2 = fork();
                if (pid_2 == -1) {
                    printf("Error: A problem executing the command\n");
                    continue;
                }
                if(pid_2==0){
                    close(a[0]);
                    dup2(a[1],1);
                    close(a[1]);
                    execvp(argv[2], argv);
                }
                if (pid_2>0){
                    argv[2] = NULL;
                    close(a[1]);
                    dup2(a[0],0);
                    close(a[0]);
                    execvp(argv[0], argv);
                }

            }
            if (pid>0){
                wait(NULL);
                wait(NULL);
                continue;
            }

        }


    }
    return 1;


}
