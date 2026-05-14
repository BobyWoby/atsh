#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv){
    const char *DEFAULT_DIR = "/bin/";
    const int buf_sz = 100000;

    char *prompt = "atsh> ";
    char buf[buf_sz];
    chdir(DEFAULT_DIR);

    while(true){
        memset(buf, '\0', sizeof(buf));
        write(0, prompt, strlen(prompt));
        int bytes_read = read(0, buf, buf_sz);

        int pid = fork();
        if(pid < 0){
            char *err_msg = "Failed to fork process\n";
            write(0, err_msg, strlen(err_msg));
        }else if(pid == 0){
            // Child Process
            // char *cmd;
            char *args[1000] = {NULL};
            char *cmd = (char *)malloc(strlen(buf));

            char *token = strtok(buf, "\n ");
            int argc = 0;
            while(token != NULL){
                // printf("%s\n", token);
                // memset(args[argc], '\0', strlen(args[argc]));
                args[argc++] = token;
                token = strtok(NULL, "\n ");
            }
            args[argc] = NULL;

            int idx = 0;
            while(args[idx] != NULL){
                printf("%s\n", args[idx++]);
            }

            if(execv(args[0], args) == -1){
                perror("Exec Error: ");
            }

        }else{
            int cpid = wait(NULL);
            buf[0] = '\0';
            // Parent
        }
    }
}
