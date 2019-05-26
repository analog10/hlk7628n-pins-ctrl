#include <refclk.h>

#define MMAP_PATH			"/dev/mem"

static unsigned int refclk_rates[] = { 40, 12, 25, 40, 48 };
static char *refclk_names[] = { "xtal", "12", "25", "40", "48" };

static uint8_t* refclk_mmap_clkcfg0 = NULL;
static int refclk_mmap_fd = 0;

void __refclk_set(unsigned int val)
{
	unsigned int clkcfg0 = *(volatile uint32_t*) (refclk_mmap_clkcfg0 + 0x2c);

	clkcfg0 &= ~(0x7 << 9);
	clkcfg0 |= (val << 9);
	*(volatile uint32_t*) (refclk_mmap_clkcfg0 + 0x2c) = clkcfg0;
}

int refclk_set(unsigned int rate)
{
	int id;

	for (id = 1; id < __CLK_MAX; id ++) {
		if (refclk_rates[id] != rate)
			continue;
		__refclk_set(id);
		fprintf(stderr, "set reclk to %uMHz\n", rate);
		return 0;
	}
	__refclk_set(CLK_XTAL);
	fprintf(stderr, "failed to set reclk to %uMHz using xtal instead\n", rate);
	return -1;
}

int refclk_get(void)
{
	unsigned int clkcfg0 = *(volatile uint32_t*) (refclk_mmap_clkcfg0 + MT76X8_SYSCTL_CLKCFG0);
	int id;

	clkcfg0 >>= 9;
	clkcfg0 &= 7;

	fprintf(stderr, "refclk rates in MHz: ");
        for (id = 0; id < __CLK_MAX; id ++) {
		if (id == clkcfg0)
			fprintf(stderr, "[%s] ", refclk_names[id]);
		else
			fprintf(stderr, "%s ", refclk_names[id]);
	}
	fprintf(stderr, "\n");
	return 0;
}

int refclk_mmap_open(void)
{
	if ((refclk_mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "unable to open mmap file");
		return -1;
	}

	refclk_mmap_clkcfg0 = (uint8_t*) mmap(NULL, 1024, PROT_READ | PROT_WRITE,
		MAP_FILE | MAP_SHARED, refclk_mmap_fd, MT76X8_SYSCTL_BASE);
	if (refclk_mmap_clkcfg0 == MAP_FAILED) {
		perror("foo");
		fprintf(stderr, "failed to mmap");
		refclk_mmap_clkcfg0 = NULL;
		close(refclk_mmap_fd);
		return -1;
	}

	return 0;
}

void refclk_mmap_close(void) 
{
	close(refclk_mmap_fd);		
}


