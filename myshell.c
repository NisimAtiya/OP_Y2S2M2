#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

void ecec1(char *argv[10], int *possition);

void exec_010(char *argv[10], int *possition);

void exec_001(char *argv[10], int *possition);

void exec_100(char *pString[10], int pInt[4],int i);

void exec_110(char *argv[10], int possition[4]);

void exec_101(char *argv[10], int possition[4]);

int find_pipe(char *argv[], int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],"|")==0){
            cnt++;
        }
    }
    return cnt;
}
int find_redirection(char *argv[],int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],">")==0){
            cnt++;
        }
    }
    return cnt;
}
int find_redirection_2(char *argv[],int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],">>")==0){
            cnt++;
        }
    }
    return cnt;
}
void find_index(char *argv[],int end,int* a){

    int cnt=0;
    for (int i = 0; i <end; ++i) {
        if(strcmp(argv[i],"|")==0){
            a[cnt]=i;
            cnt++;
        }

    }
    cnt=2;
    for (int i = 0; i <end; ++i) {
        if(strcmp(argv[i],">")==0){
            a[cnt]=i;
        }
    }
    cnt=3;
    for (int i = 0; i <end; ++i) {
        if(strcmp(argv[i],">>")==0){
            a[cnt]=i;
        }
    }
}


int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;


    while (1) {
        /* ignored control+c signal */
        signal(SIGINT,SIG_IGN);
        printf("\nEN_SHELL: ");
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


        /* find amount of the special commands and the possition of them  */
        int cnt_p= find_pipe(argv,i);
        int cnt_red= find_redirection(argv,i);
        int cnt_red_2= find_redirection_2(argv,i);
        int possition[4]={0};
        find_index(argv,i,possition);



        /* Is command empty */
        if (i==0)
            continue;
        /* supports on command exit */
        if (strcmp(argv[0], "exit")==0){
            printf("bye bye.\n");
            return 1;
        }
        /* got one argument command from user */
        if(cnt_p==0&&cnt_red==0&&cnt_red_2==0) {
            printf("exec_000\n");
            exec_000(argv,possition,i);
        }
        if(cnt_p==1&&cnt_red==0&&cnt_red_2==0){
            printf("exec_100\n");
            exec_100(argv,possition,i);

        }
        if(cnt_p==0&&cnt_red==1&&cnt_red_2==0) {
            printf("exec_010\n");
            exec_010(argv, possition);
        }


        if(cnt_p==0&&cnt_red==0&&cnt_red_2==1){
            printf("exec_001\n");
            exec_001(argv,possition);
        }
        if(cnt_p==2&&cnt_red==0&&cnt_red_2==0){
            printf("exec_200\n");


        }
        if(cnt_p==1&&cnt_red==1&&cnt_red_2==0){
            printf("exec_110\n");
            exec_110(argv,possition);
        }
        if(cnt_p==1&&cnt_red==0&&cnt_red_2==1){
            printf("exec_101\n");
            exec_101(argv,possition);


        }
        if(cnt_p==2&&cnt_red==1&&cnt_red_2==0){
            printf("exec_210\n");

        }
        if(cnt_p==2&&cnt_red==0&&cnt_red_2==1){
            printf("exec_101\n");

        }
    }
    return 1;



}

void exec_101(char *argv[10], int possition[4]) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);

        int file_des = open(argv[possition[3]+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[2]+1]);
            return;
        }
        argv[possition[3]]=NULL;
        dup2(file_des,1);
        dup2(des_p[0],0);         //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);



        execvp(argv[possition[0]+1], argv+possition[0]+1);
        close(file_des);

    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    wait(0);
    return;


}

void exec_110(char *argv[10], int possition[4]) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);

        int file_des = open(argv[possition[2]+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[2]+1]);
            return;
        }
        argv[possition[2]]=NULL;
        dup2(file_des,1);
        dup2(des_p[0],0);         //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);



        execvp(argv[possition[0]+1], argv+possition[0]+1);
        close(file_des);

    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    wait(0);
    return;


}

void exec_100(char *argv[10], int possition[4],int i) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {

        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p[0],0);         //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);

        execvp(argv[possition[0]+1], argv+possition[0]+1);


    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    wait(0);
    return;

}



//void sendto(char *argv[], int start, int end);
////A function that returns 0 if it is a clean command and 1 otherwise
//int find(char *argv[],int start,int end){
//    for (int i = start+1; i < end; ++i) {
//        if(strcmp(argv[i],">")==0){
//            return 1;
//        }
//        if(strcmp(argv[i],">>")==0){
//            return 1;
//        }
//        if(strcmp(argv[i],"<")==0){
//            return 1;
//        }
//        if(strcmp(argv[i],"<<")==0){
//            return 1;
//        }
//        if(strcmp(argv[i],"5")==0){
//            return 1;
//        }
//    }
//    return 0;
//}
//
//void exec_clean_command(char *argv[10]) {
//    printf("exec_clean_command\n");
//    int pid = fork();
//    if (pid == -1) {
//        printf("Error: A problem executing the command\n");
//        return;
//    }
//    /* for commands not part of the shell command language */
//    if (pid == 0) {
//        /* back to default after control+c signal  */
//        signal(SIGINT,SIG_DFL);
//        execvp(argv[0], argv);
//    }
//    if (pid>0){
//        wait(NULL);
//        return;
//    }
//}
//
//void f1(char *argv[], int com_index,int operator,int end) {
//    printf("f1\n");
//    int pid = fork();
//    if (pid == -1) {
//        printf("Error: A problem executing the command\n");
//        return;
//    }
//    /* for commands not part of the shell command language */
//    if (pid == 0) {
//        /* back to default after control+c signal  */
//        signal(SIGINT, SIG_DFL);
//        int file_des = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
//        if (file_des == -1) {
//            printf("error while opening the file\n");
//            return;
//        }
//        dup2(file_des,1);
//
//        execvp(argv[com_index], argv);
//        close(file_des);
//    }
//    if (pid>0){
//        wait(NULL);
//        if(operator+1!=end) {
//            sendto(argv,operator+2,end);
//        }
//    }
//}
//void f2(char *argv[], int com_index,int operator,int end){
//    printf("f2\n");
//    int pid = fork();
//    if (pid == -1) {
//        printf("Error: A problem executing the command\n");
//        return;
//    }
//    /* for commands not part of the shell command language */
//    if (pid == 0) {
//        /* back to default after control+c signal  */
//        signal(SIGINT, SIG_DFL);
//        int file_des = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0777);
//
//        if (file_des == -1) {
//            printf("error while opening the file\n");
//            return;
//        }
//        dup2(file_des,1);
//        execvp(argv[com_index], argv);
//        close(file_des);
//
//    }
//}
//void sendto(char *argv[], int start, int end){
//    printf("sendto\n");
//    for (int i = start; i < end; ++i) {
//        if(strcmp(argv[i],">")==0){
//            printf("the command is with >\n");
//            argv[i] = NULL;
//            f1(argv,start,i,end);
//        }
//        if(strcmp(argv[i],">>")==0){
//            printf("the command is with ><\n");
//            argv[i] = NULL;
//            f2(argv,start,i,end);
//        }
//        if(strcmp(argv[i],"<")==0){
//            argv[i] = NULL;
//            f3(argv,i);
//        }
//        if(strcmp(argv[i],"<<")==0){
//            argv[i] = NULL;
//            f4(argv,i);
//        }
//        if(strcmp(argv[i],"5")==0){
//            argv[i] = NULL;
//            f1(argv,i);
//        }
//
//    }
//}
//
//
//
//int main() {
//    int i;
//    char *argv[10];
//    char command[1024];
//    char *token;
//
//    while (1) {
//        /* ignored control+c signal */
//        signal(SIGINT,SIG_IGN);
//        printf("EN_SHELL: ");
//        fgets(command, 1024, stdin);
//        command[strlen(command) - 1] = '\0'; // replace \n with \0
//
//        /* parse command line */
//        i = 0;
//        token = strtok (command," ");
//        while (token != NULL)
//        {
//            argv[i] = token;
//            token = strtok (NULL, " ");
//            i++;
//        }
//        argv[i] = NULL;
//
//        /* Is command empty */
//        if (i == 0)
//            continue;
//        /* supportion in command exit */
//        if (strcmp(argv[0], "exit")==0){
//            printf("bye bye.\n");
//            return 1;
//        }
//        //Checks if the command is clean
//        if (find(argv,0,i)==0){
//            printf("command is clean\n");
//            exec_clean_command(argv);
//        } else{
//            printf("command is not clean\n");
//            sendto(argv,0,i);
//        }
//
//        /* implementing of command '<' */
//        if(strcmp(argv[1],"<")==0){
//            int pid = fork();
//            if (pid == -1) {
//                printf("Error: A problem executing the command\n");
//                continue;
//            }
//            /* for commands not part of the shell command language */
//            if (pid == 0) {
//                /* back to default after control+c signal  */
//                signal(SIGINT,SIG_DFL);
//                int file_des = open(argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
//                if (file_des == -1) {
//                    printf("error while opening the file %s",argv[0]);
//                    continue;
//                }
//                argv[1]=NULL;
//                dup2(file_des,1);
//
//                execvp(argv[2], argv);
//                close(file_des);
//            }
//            if (pid>0){
//                wait(NULL);
//                continue;
//            }
//        }
//        /* implementing of command '>>' */
//        if(strcmp(argv[1],">>")==0){
//            int pid = fork();
//            if (pid == -1) {
//                printf("Error: A problem executing the command\n");
//                continue;
//            }
//            /* for commands not part of the shell command language */
//            if (pid == 0) {
//                /* back to default after control+c signal  */
//                signal(SIGINT,SIG_DFL);
//                int file_des = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0777);
//
//                if (file_des == -1) {
//                    printf("error while opening the file %s",argv[2]);
//                    continue;
//                }
//                argv[1]=NULL;
//                dup2(file_des,1);
//
//                execvp(argv[0], argv);
//                close(file_des);
//            }
//            if (pid>0){
//                wait(NULL);
//                continue;
//            }
//        }
//        /* implementing of command '<<' */
//        if(strcmp(argv[1],"<<")==0){
//            int pid = fork();
//            if (pid == -1) {
//                printf("Error: A problem executing the command\n");
//                continue;
//            }
//            /* for commands not part of the shell command language */
//            if (pid == 0) {
//                /* back to default after control+c signal  */
//                signal(SIGINT,SIG_DFL);
//                int file_des = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0777);
//                if (file_des == -1) {
//                    printf("error while opening the file %s",argv[0]);
//                    continue;
//                }
//                argv[1]=NULL;
//                dup2(file_des,1);
//
//                execvp(argv[2], argv);
//                close(file_des);
//            }
//            if (pid>0){
//                wait(NULL);
//                continue;
//            }
//        }
//        if( (strcmp(argv[1],"|")==0) && (i==3) ){
//            argv[1]=NULL;
//            int pid = fork();
//            if (pid == -1) {
//                printf("Error: A problem executing the command\n");
//                continue;
//            }
//            if(pid==0){
//                /* back to default after control+c signal  */
//                signal(SIGINT,SIG_DFL);
//                int a[2];
//                int r = pipe(a);
//                if(r==-1){
//                    printf("error: cannot initialing pipe");
//                    continue;
//                }
//                int pid_2 = fork();
//                if (pid_2 == -1) {
//                    printf("Error: A problem executing the command\n");
//                    continue;
//                }
//                if(pid_2==0){
//                    close(a[0]);
//                    dup2(a[1],1);
//                    close(a[1]);
//                    execvp(argv[2], argv);
//                }
//                if (pid_2>0){
//                    argv[2] = NULL;
//                    close(a[1]);
//                    dup2(a[0],0);
//                    close(a[0]);
//                    execvp(argv[0], argv);
//                }
//
//            }
//            if (pid>0){
//                wait(NULL);
//                continue;
//            }
//
//        }
//
//
//    }
//
//




//  take care of commands without anny pipe and redirection commands.
void exec_000(char *argv[10], int *possition) {
    printf("in\n");
    int pid = fork();
    if (pid == -1) {
        printf("Error: A problem executing the command\n");
        return;
    }
    /* for commands not part of the shell command language */
    if (pid == 0) {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        execvp(argv[0], argv);
    }
    if (pid>0){
        wait(NULL);
        return;
    }

}
//  take care of commands without anny pipe and one redirection '>' commands.
void exec_010(char *argv[10], int *possition) {
    int pid = fork();
    if (pid == -1) {
        printf("Error: A problem executing the command\n");
        return;
    }
    /* for commands not part of the shell command language */
    if (pid == 0) {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        int file_des = open(argv[possition[2]+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[2]+1]);
            return;
        }
        argv[possition[2]]=NULL;
        dup2(file_des,1);

        execvp(argv[0], argv);
        close(file_des);
    }
    if (pid>0){
        wait(NULL);
        return;
    }
}
//  take care of commands without anny pipe and one redirection '>>' commands.
void exec_001(char *argv[10], int *possition) {
    int pid = fork();
    if (pid == -1) {
        printf("Error: A problem executing the command\n");
        return ;
    }
    /* for commands not part of the shell command language */
    if (pid == 0) {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        int file_des = open(argv[possition[3]+1], O_WRONLY | O_CREAT | O_APPEND, 0777);

        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[3]+1]);
            return ;
        }
        argv[possition[3]]=NULL;
        dup2(file_des,1);

        execvp(argv[0], argv);
        close(file_des);
    }
    if (pid>0){
        wait(NULL);
        return;
    }
}





