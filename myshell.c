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

void exec_000(char *argv[10], int *possition);

void exec_010(char *argv[10], int *possition);

void exec_001(char *argv[10], int *possition);

void exec_100(char *pString[10], int pInt[4]);

void exec_110(char *argv[10], int possition[4]);

void exec_101(char *argv[10], int possition[4]);

void exec_200(char *argv[10], int possition[4]);

void exec_210(char *pString[10], int pInt[4]);
void exec_201(char *pString[10], int pInt[4]);



//Gets a string and returns how many "|"s it has
int find_pipe(char *argv[], int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],"|")==0){
            cnt++;
        }
    }
    return cnt;
}
//Gets a string and returns how many ">"s it has
int find_redirection(char *argv[],int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],">")==0){
            cnt++;
        }
    }
    return cnt;
}
//Gets a string and returns how many ">>"s it has
int find_redirection_2(char *argv[],int end){
    int cnt=0;
    for (int i = 0; i <end ; ++i) {
        if(strcmp(argv[i],">>")==0){
            cnt++;
        }
    }
    return cnt;
}
//Receives a string and an array and initializes the array to be:
//a[0] = The index of the "|" in the string
//a[1] = The index of the second "|" in the string
//a[2] = The index of the ">" in the string
//a[3] = The index of the ">>" in the string
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
        /* get the command from the user */
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* i will contain the command size */
        i = 0;
        token = strtok (command," ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
        }
        argv[i] = NULL;


        /* find amount of the special commands and the position of them  */
        int cnt_p= find_pipe(argv,i);
        int cnt_red= find_redirection(argv,i);
        int cnt_red_2= find_redirection_2(argv,i);
        int possition[4]={0};
        find_index(argv,i,possition);

        /* Is command empty */
        if (i==0)
            continue;
            /* supports on command exit */
        else if (strcmp(argv[0], "exit")==0){
            return 1;
        }
            /* got one argument command from user */
        else if (cnt_p==0&&cnt_red==0&&cnt_red_2==0) {
            exec_000(argv,possition);
        }
            /* got one command with one pipe as argument  */
        else if (cnt_p==1&&cnt_red==0&&cnt_red_2==0){
            exec_100(argv,possition);

        }
            /* got one command with one redirection as argument  */
        else if (cnt_p==0&&cnt_red==1&&cnt_red_2==0) {
            exec_010(argv, possition);
        }
            /* got one command with one redirection_2 as argument  */
        else if (cnt_p==0&&cnt_red==0&&cnt_red_2==1){
            exec_001(argv,possition);
        }
            /* got one command with two pipe as arguments  */
        else if (cnt_p==2&&cnt_red==0&&cnt_red_2==0){
            exec_200(argv,possition);
        }
            /* got one command with one pipe and one redirection as arguments  */
        else if (cnt_p==1&&cnt_red==1&&cnt_red_2==0){
            exec_110(argv,possition);
        }
            /* got one command with one pipe and redirection_2 as arguments  */
        else if (cnt_p==1&&cnt_red==0&&cnt_red_2==1){
            exec_101(argv,possition);
        }
            /* got one command with two pipes and one redirection arguments  */
        else if (cnt_p==2&&cnt_red==1&&cnt_red_2==0){
            exec_210(argv,possition);
        }
            /* got one command with two pipes and one redirection_2 as arguments  */
        else if (cnt_p==2&&cnt_red==0&&cnt_red_2==1){
            exec_201(argv,possition);
        } else{
            /* not valid command */
            printf("Invalid input!!\n");
        }
    }
    return 1;



}
//  take care of commands with two pipes and with one redirection '>' commands.
void exec_210(char *argv[10], int possition[4]) {
    int des_p[2];
    int des_p2[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    if(pipe(des_p2) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);        //closing pipe write
        close(des_p2[0]);       //closing pipe read
        close(des_p2[1]);       //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0){            //creating 2nd child
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        argv[possition[1]]=NULL;
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p2[1],1);         //replacing stdout with pipe write
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p[0],0);   //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);        //closing pipe read
        close(des_p2[0]);       //closing pipe read
        close(des_p2[1]);       //closing pipe write

        execvp(argv[possition[0]+1], argv+possition[0]+1);
    }
    if(fork()==0){
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        int file_des = open(argv[possition[2]+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[2]+1]);
            return;
        }
        argv[possition[2]]=NULL;
        dup2(file_des,1);       //replacing stdout with file
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p2[0],0);      //replacing stdin with pipe read
        close(des_p[1]);        //closing pipe write
        close(des_p[0]);        //closing pipe rad
        close(des_p2[1]);       //closing pipe write
        close(des_p2[0]);       //closing pipe read
        execvp(argv[possition[1]+1], argv+possition[1]+1);
        close(file_des);        //closing file

    }


    close(des_p[1]);        //closing pipe write
    close(des_p[0]);        //closing pipe read
    close(des_p2[1]);       //closing pipe write
    close(des_p2[0]);       //closing pipe read
    wait(0);
    wait(0);
    wait(0);
    return;

}
//  take care of commands with two pipes and with one redirection '>>' commands.
void exec_201(char *argv[10], int possition[4]) {
    int des_p[2];
    int des_p2[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    if(pipe(des_p2) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);        //closing pipe read
        close(des_p[1]);        //closing pipe write        
        close(des_p2[0]);       //closing pipe read
        close(des_p2[1]);       //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0){            //creating 2nd child
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        argv[possition[1]]=NULL;
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p2[1],1);         //replacing stdout with pipe write
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p[0],0);   //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);
        close(des_p2[0]);       //closing pipe read
        close(des_p2[1]);

        execvp(argv[possition[0]+1], argv+possition[0]+1);
    }
    if(fork()==0){
        signal(SIGINT,SIG_DFL);
        int file_des = open(argv[possition[3]+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[3]+1]);
            return;
        }
        argv[possition[3]]=NULL;
        dup2(file_des,1);      //replacing stdout with file
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p2[0],0);      //replacing stdin with pipe read
        close(des_p[1]);        //closing pipe write
        close(des_p[0]);        //closing pipe read
        close(des_p2[1]);       //closing pipe write
        close(des_p2[0]);       //closing pipe read
        execvp(argv[possition[1]+1], argv+possition[1]+1);
        close(file_des);     //closing file

    }


    close(des_p[1]);       //closing pipe write
    close(des_p[0]);        //closing pipe read
    close(des_p2[1]);       //closing pipe write
    close(des_p2[0]);       //closing pipe read
    wait(0);
    wait(0);
    wait(0);
    return;

}
//  take care of commands with two pipes commands.
void exec_200(char *argv[10], int possition[4]) {
    int des_p[2];
    int des_p2[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    if(pipe(des_p2) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);   //replacing stdout with pipe write
        close(des_p[0]);    //closing pipe read
        close(des_p[1]);    //closing pipe write
        close(des_p2[0]);   //closing pipe read
        close(des_p2[1]);   //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0){            //creating 2nd child
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        argv[possition[1]]=NULL;
        close(STDOUT_FILENO);    //closing stdout
        dup2(des_p2[1],1);  //replacing stdout with pipe write
        close(STDIN_FILENO);    //closing stdin
        dup2(des_p[0],0);   //replacing stdin with pipe read
        close(des_p[1]);    //closing pipe write
        close(des_p[0]);    //closing pipe read
        close(des_p2[0]);   //closing pipe read
        close(des_p2[1]);   //closing pipe write

        execvp(argv[possition[0]+1], argv+possition[0]+1);
    }
    if(fork()==0){
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);    //closing stdin
        dup2(des_p2[0],0);  //replacing stdin with pipe read
        close(des_p[1]);    //closing pipe write
        close(des_p[0]);    //closing pipe read
        close(des_p2[1]);   //closing pipe write
        close(des_p2[0]);   //closing pipe read
        execvp(argv[possition[1]+1], argv+possition[1]+1);
    }


    close(des_p[1]);    //closing pipe write
    close(des_p[0]);    //closing pipe read
    close(des_p2[1]);   //closing pipe write
    close(des_p2[0]);   //closing pipe read
    wait(0);
    wait(0);
    wait(0);
    return;

}
//  take care of commands with one pipe and with one redirection '>>' commands.
void exec_101(char *argv[10], int possition[4]) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);   //replacing stdout with pipe write
        close(des_p[0]);    //closing pipe read
        close(des_p[1]);    //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);

        int file_des = open(argv[possition[3]+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[3]+1]);
            return;
        }
        argv[possition[3]]=NULL;
        dup2(file_des,1);   //replacing stdout with file
        dup2(des_p[0],0);   //replacing stdin with pipe read
        close(des_p[1]);    //closing pipe write
        close(des_p[0]);    //closing pipe read    
        execvp(argv[possition[0]+1], argv+possition[0]+1);
        close(file_des);    //closing file   

    }

    close(des_p[0]);    //closing pipe read
    close(des_p[1]);    //closing pipe write
    wait(0);
    wait(0);
    return;


}
//  take care of commands with one pipe and with one redirection '>' commands.
void exec_110(char *argv[10], int possition[4]) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);   //closing stdout
        dup2(des_p[1],1);       //replacing stdout with pipe write
        close(des_p[0]);        //closing pipe read
        close(des_p[1]);        //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);

        int file_des = open(argv[possition[2]+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (file_des == -1) {
            printf("error while opening the file %s",argv[possition[2]+1]);
            return;
        }
        argv[possition[2]]=NULL;
        dup2(file_des,1);    //replacing stdout with file   
        dup2(des_p[0],0);   //replacing stdin with pipe read
        close(des_p[1]);    //closing pipe write
        close(des_p[0]);    //closing pipe read
        execvp(argv[possition[0]+1], argv+possition[0]+1);
        close(file_des);

    }

    close(des_p[0]);    //closing pipe read
    close(des_p[1]);    //closing pipe write
    wait(0);
    wait(0);
    return;


}
//  take care of commands with one pipe commands.
void exec_100(char *argv[10], int possition[4]) {
    int des_p[2];
    argv[possition[0]]=NULL;
    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if(fork() == 0)            //first fork
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDOUT_FILENO);  //closing stdout
        dup2(des_p[1],1);         //replacing stdout with pipe write
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);    //closing pipe write
        execvp(argv[0], argv);

    }

    if(fork() == 0)            //creating 2nd child
    {
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        close(STDIN_FILENO);   //closing stdin
        dup2(des_p[0],0);         //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);    //closing pipe read
        execvp(argv[possition[0]+1], argv+possition[0]+1);


    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    wait(0);
    return;

}







//  take care of commands without anny pipe and redirection commands.
void exec_000(char *argv[10], int *possition) {
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
        /* in child */
        /* back to default after control+c signal  */
        signal(SIGINT,SIG_DFL);
        /* open the file  */
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
        /* back to main */
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
    if (pid == 0) {
        /* in child */
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
    /* back to main */
    if (pid>0){
        wait(NULL);
        return;
    }
}





