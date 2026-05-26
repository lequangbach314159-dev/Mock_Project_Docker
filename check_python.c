#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int ret = system("python3.9 --version > /tmp/python_ver.log 2>&1");

    if (ret == -1) {
        printf("Error: cannot execute command\n");
        return 1;
    }

    if (WIFEXITED(ret) && WEXITSTATUS(ret) == 0) {
        FILE *fp = fopen("/tmp/python_ver.log", "r");
        char buf[256];

        if (fp && fgets(buf, sizeof(buf), fp)) {
            printf("Detected Python Version: %s", buf);
            fclose(fp);
        }

        return 0;
    } else {
        printf("Error: Python 3.9 not found\n");
        return 1;
    }
}

