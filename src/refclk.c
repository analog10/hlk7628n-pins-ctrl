#include <refclk.h>

static unsigned int refclk_rates[] = { 40, 12, 25, 40, 48 };
static char *refclk_names[] = { "xtal", "12", "25", "40", "48" };

void __refclk_set(unsigned int val)
{
	unsigned int clkcfg0 = *(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_CLKCFG0);

	clkcfg0 &= ~(0x7 << 9);
	clkcfg0 |= (val << 9);
	*(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_CLKCFG0) = clkcfg0;
}

int refclk_set(unsigned int rate)
{
	int id;

	for (id = 1; id < __CLK_MAX; id ++) {
		if (refclk_rates[id] != rate)
			continue;
		__refclk_set(id);
		fprintf(stderr, "set reclk to %uMHz\n", rate);
		return EXIT_SUCCESS;
	}
	__refclk_set(CLK_XTAL);
	fprintf(stderr, "failed to set reclk to %uMHz using xtal instead\n", rate);
	return EXIT_FAILURE;
}

int refclk_get(void)
{
	unsigned int clkcfg0 = *(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_CLKCFG0);
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
	return EXIT_SUCCESS;
}

