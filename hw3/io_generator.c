#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    int BLOCKSIZE = 0;
    int tmp = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC , 0644);
    char pid[16] = { 0x00 };
    sprintf(pid, "%d", getpid());
    write(tmp, pid, strlen(pid));
    close(tmp);

    char file_path[1024];
    printf("FILE_PATH: ");
    scanf("%s", file_path);
    printf("BLOCKSIZE: ");
    scanf("%d", &BLOCKSIZE);

    int fd = open(file_path, O_DIRECT | O_RDONLY );
    int *buf = (int*)aligned_alloc(BLOCKSIZE, BLOCKSIZE * sizeof(int));

    while(1) {
        if (read(fd, buf, BLOCKSIZE) <= 0) {
            break;
        }
    }

    free(buf);
    close(fd);

    return 0;
}
