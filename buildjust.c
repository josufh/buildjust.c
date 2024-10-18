#ifndef __BUILD_JUST_H__
#define __BUILD_JUST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <sched.h>
#include <sys/types.h>

#define NOT_IMPLEMENTED(msg) do { fprintf(stderr, "%s:%d: To implement: %s\n", __FILE__, __LINE__, msg); abort(); } while(0)

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))
#define SAFE_ARRAY_GET(array, index) (assert((size_t)index < ARRAY_LEN(array)), array[(size_t)index])

typedef char* String;
typedef const char* CString;

int RUN_COMMAND(CString* command, String* output, size_t output_size);
int run_command();

#endif // __BUILD_JUST_H__

int RUN_COMMAND(CString* command, String* output, size_t output_size) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], stdout);
        close(pipefd[1]);

        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        close(pipefd[1]);

        ssize_t bytes_read = read(pipefd[0], output, output_size - 1);
        if (bytes_read == -1) {
            perror("read");
            return -1;
        }

        output[bytes_read] = '\0';
        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}

int run_command() {
    String output[1024];
    if (RUN_COMMAND("ls", output, sizeof(output)) == 0) {
        printf("Command output:\n%s\n", output);
    } else {
        fprintf(stderr, "Failed to run the command\n");
        return -1;
    }
    return 0;
}