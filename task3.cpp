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
        setsid();
        int devnull_fd = open("/dev/null", O_RDWR);
        if (devnull_fd == -1) {
            std::cerr << "Failed to open /dev/null\n";
            return 1;
        };
        if (dup2(devnull_fd, STDOUT_FILENO) != -1 ||
            dup2(devnull_fd, STDERR_FILENO) != -1) {
            close(devnull_fd);
            std::cerr << "Failed to redirect i/o\n";
            return 1;
        };
        close(devnull_fd);
        execvp(argv[1], &argv[1]);
        std::cerr << "Program execution failed\n";
        return 1;
    };
};
