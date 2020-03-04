#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define PRESET 512

int main(int argc, char** argv) {

	int start_lba = 0;
	int size = 0;

	if (argc > 2) {
		start_lba = atoi(argv[1]);
		size = atoi(argv[2]);
	}
	else {
		printf("START_LBA = ");
		scanf("%d", &start_lba);
		printf("REQUEST SIZE = ");
		scanf("%d", &size);
	}

    int fd = open("/dev/sda", O_DIRECT | O_RDONLY);
    int *buf = (int*)aligned_alloc(PRESET, size * PRESET * sizeof(int));

	lseek(fd, start_lba * PRESET, SEEK_SET);
	read(fd, buf, PRESET * size);

    free(buf);
    close(fd);

    return 0;
}
