#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    };
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed\n";
        return 1;
    } else if (pid == 0) {
        int out_fd = open("out.txt", O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
        int err_fd = open("err.txt", O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
        if (out_fd == -1 || err_fd == -1) {
            std::cerr << "Failed to open output files\n";
            return 1;
        };
        if (dup2(out_fd, STDOUT_FILENO) == -1 || dup2(err_fd, STDERR_FILENO) == -1) {
            std::cerr << "Failed to redirect output\n";
            close(out_fd);
            close(err_fd);
            return 1;
        };
        close(out_fd);
        close(err_fd);
        execvp(argv[1], &argv[1]);
        std::cerr << "Program execution failed\n";
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
    };
};
