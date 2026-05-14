#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv){
    static const char *DEFAULT_DIR = "/bin/";
    static const int buf_sz = 100000;

    static const char *prompt = "atsh> ";
    static const char* exit_msg = "Goodbye!\n";


    char buf[buf_sz];
    chdir(DEFAULT_DIR);

    while(true){
        memset(buf, '\0', sizeof(buf));
        write(0, prompt, strlen(prompt));
        int bytes_read = read(0, buf, buf_sz);

        char *cmd = (char *)malloc(strlen(buf));
        char *token = strtok(buf, "\n ");
        
        int arg_capacity = 1;
        char **args = (char **)malloc(arg_capacity * sizeof(char *));

        if(!strcmp(token, "exit")){
            write(0, exit_msg, strlen(exit_msg));
            
            // Cleanup memory
            free(args);
            free(cmd);

            exit(0);
        }else {
            int argc = 0;
            while(token != NULL){
                if(argc >= arg_capacity){
                    args = (char **)reallocarray(args, arg_capacity *= 2, sizeof(char *));
                }
                args[argc++] = token;
                token = strtok(NULL, "\n ");
            }
            args[argc] = NULL;

            if(!strcmp(args[0], "cd")){

            }
        }

        int pid = fork();
        if(pid < 0){
            char *err_msg = "Failed to fork process\n";
            write(0, err_msg, strlen(err_msg));
        }else if(pid == 0){
            if(execv(args[0], args) == -1){
                perror("Exec Error: ");
            }

        }else{
            int cpid = wait(NULL);
            buf[0] = '\0';
            // Parent
        }

        free(args);
        free(cmd);
    }
}
