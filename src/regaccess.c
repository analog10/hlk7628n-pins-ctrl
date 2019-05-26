#include <regaccess.h>

uint8_t* mmap_reg = NULL;;
int mmap_fd = 0;

int mmap_open(void)
{
	if ((mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "unable to open mmap file");
		return EXIT_FAILURE;
	}

	mmap_reg = (uint8_t*) mmap(NULL, 1024, PROT_READ | PROT_WRITE,
		MAP_FILE | MAP_SHARED, mmap_fd, MT76X8_SYSCTL_BASE);
	if (mmap_reg == MAP_FAILED) {
		perror("failed to mmap");
		fprintf(stderr, "failed to mmap");
		mmap_reg = NULL;
		close(mmap_fd);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void mmap_close(void) 
{
	close(mmap_fd);		
}
