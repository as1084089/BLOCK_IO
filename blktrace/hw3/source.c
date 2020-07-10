#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    int tmp = open("tmp", O_RDWR | O_CREAT , 0777);
    char pid[16] = { 0x00 };
    sprintf(pid, "%d", getpid());
    write(tmp, pid, strlen(pid));
    close(tmp);

    char file_path[1024];
    printf("FILE_PATH: ");
    scanf("%s", file_path);

    int fd = open(file_path, O_DIRECT | O_RDONLY );
    int *buf = (int*)aligned_alloc(512, 512 * sizeof(int));

    while(1) {
        if (read(fd, buf, 512) <= 0) {
            break;
        }
    }

    free(buf);
    close(fd);

    return 0;
}
