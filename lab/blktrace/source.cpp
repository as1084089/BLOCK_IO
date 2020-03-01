#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define PRESET 512

int main(int argc, char** argv) {

	int start_lba = atoi(argv[1]);
	int size = atoi(argv[2]);

    int fd = open("/dev/sda", O_DIRECT | O_RDONLY);
    int *buf = (int*)aligned_alloc(PRESET, size * PRESET * sizeof(int));

	lseek(fd, start_lba * PRESET, SEEK_SET);
	read(fd, buf, PRESET * size);

    free(buf);
    close(fd);

    return 0;
}
