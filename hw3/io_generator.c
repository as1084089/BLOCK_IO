#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    if(argc < 3) {
      printf("usage: ./io_generator [FILE_PATH] [I/O_SIZE]\n");
      return 0;
    }
    int BLOCKSIZE = atoi(argv[2]);
    int tmp = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC , 0644);
    char pid[16] = { 0x00 };
    sprintf(pid, "%d", getpid());
    write(tmp, pid, strlen(pid));
    close(tmp);

    printf("file path: %s\n", argv[1]);
    printf("i/o size: %s\n", argv[2]);

    int fd = open(argv[1], O_DIRECT | O_RDONLY );
    unsigned long fsize = lseek(fd, 0, SEEK_END);
    printf("file size: %lu bytes\n\n", fsize);
    lseek(fd, 0, SEEK_SET);
    int *buf = (int*)aligned_alloc(BLOCKSIZE, BLOCKSIZE * sizeof(int));

    unsigned long curr_point = 0;
    double curr_percent = 0;
    double last = 0;
    while(1) {
        curr_point += read(fd, buf, BLOCKSIZE);
        curr_percent = ((double)curr_point / (double)fsize) * 100;
        if(curr_percent - last >= 5) {
          printf("\t%11lu / %-11lu [%6.1lf%% ]\n", curr_point, fsize, curr_percent);
          if(curr_percent >= 100.0) break;
          last += 5;
        }
    }
    printf("\n");

    free(buf);
    close(fd);

    return 0;
}
