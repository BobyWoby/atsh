#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    static const char* BIN_DIR = "/bin/";
    static const char* HOME_DIR = "/home/bobywoby/";
    static const int buf_sz = 100000;

    static const char* prompt = "atsh> ";
    static const char* exit_msg = "Goodbye!\n";

    char buf[buf_sz];

    char* cmd = (char*)malloc(strlen(BIN_DIR));
    strcpy(cmd, BIN_DIR);

    int arg_capacity = 1;
    char** args = (char**)malloc(arg_capacity * sizeof(char*));

    while (true) {
        memset(buf, '\0', sizeof(buf));
        write(0, prompt, strlen(prompt));
        int bytes_read = read(0, buf, buf_sz);

        char* token = strtok(buf, "\n ");

        if (!strcmp(token, "exit")) {
            write(0, exit_msg, strlen(exit_msg));

            // Cleanup memory
            free(args);
            free(cmd);

            exit(0);
        } else {
            int argc = 0;
            while (token != NULL) {
                if (argc >= arg_capacity) {
                    args = (char**)reallocarray(args, arg_capacity *= 2, sizeof(char*));
                }
                args[argc++] = token;
                token = strtok(NULL, "\n ");
            }
            args[argc] = NULL;

            if (!strcmp(args[0], "cd")) {
                if (argc != 2) {
                    char* msg = "Wrong number of args in cd\n";
                    write(0, msg, strlen(msg));
                    continue;
                } else {
                    if (chdir(args[1]) != 0) {
                        perror("cd Error: ");
                    }
                    continue;
                }
            }
        }

        int pid = fork();
        if (pid < 0) {
            char* err_msg = "Failed to fork process\n";
            write(0, err_msg, strlen(err_msg));
        } else if (pid == 0) {
            if (execv(args[0], args) == -1) {
                // search in /bin/
                if (errno == 2) {
                    cmd = (char*)realloc(cmd, strlen(cmd) + strlen(args[0] + 1));
                    strcat(cmd, args[0]);
                    if (execv(cmd, args) == -1) {
                        perror("Exec Error: ");

                        free(args);
                        free(cmd);
                        exit(1);
                    }
                } else {
                    perror("Exec Error: ");
                    free(args);
                    free(cmd);
                    exit(1);
                }
            }
        } else {
            int cpid = wait(NULL);
            buf[0] = '\0';
            // Parent
        }
    }
}
