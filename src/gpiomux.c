#include <gpiomux.h>


static uint8_t* gpio_mmap_reg = NULL;
static int gpio_mmap_fd = 0;

void __gpiomux_set(unsigned int mask, unsigned int shift, unsigned int val)
{
	volatile uint32_t *gpio_mode = (volatile uint32_t*) (gpio_mmap_reg + MT76X8_SYSCTL_GPIO1_MODE);
	unsigned int v;

	if (shift > (MT76X8_SYSCTL_REGISTERS_WIDTH - 1)) {
		shift -= MT76X8_SYSCTL_REGISTERS_WIDTH;
		gpio_mode++;
	}

	v = *gpio_mode;
	v &= ~(mask << shift);
	v |= (val << shift);
	*(volatile uint32_t*) (gpio_mode) = v;
}

int gpiomux_set(char *group, char *name)
{
	int i;
	int id;

	for (id = 0; id < _O2_NUM_GPIO_MUX; id ++)
		if (!strcmp(GpioMux[id].name, group))
			break;

	if (id < _O2_NUM_GPIO_MUX) for (i = 0; i < 4; i++) {
		if (!GpioMux[id].func[i] || strcmp(GpioMux[id].func[i], name))
			continue;
		__gpiomux_set(GpioMux[id].mask, GpioMux[id].shift, i);
		printf("set gpiomux %s -> %s\n", GpioMux[id].name, name);
		return EXIT_SUCCESS;
	}
	fprintf(stderr, "unknown group/function combination\n");
	return EXIT_FAILURE;
}

int gpiomux_get(void)
{
	unsigned int gpio1_mode = *(volatile uint32_t*) (gpio_mmap_reg + MT76X8_SYSCTL_GPIO1_MODE);
	unsigned int gpio2_mode = *(volatile uint32_t*) (gpio_mmap_reg + MT76X8_SYSCTL_GPIO2_MODE);
	int id;

	for (id = 0; id < _O2_NUM_GPIO_MUX; id ++) {
		unsigned int val;
		int i;

		if (GpioMux[id].shift < MT76X8_SYSCTL_REGISTERS_WIDTH)
			val = (gpio1_mode >> GpioMux[id].shift) & GpioMux[id].mask;
		else
			val = (gpio2_mode >> (GpioMux[id].shift - MT76X8_SYSCTL_REGISTERS_WIDTH)) & GpioMux[id].mask;

		printf("Group %s - ", GpioMux[id].name);
		for (i = 0; i < 4; i++) {
			if (!GpioMux[id].func[i])
				continue;
			if (i == val)
				printf("[%s] ", GpioMux[id].func[i]);
			else
				printf("%s ", GpioMux[id].func[i]);
		}
		printf("\n");
	}

	return EXIT_SUCCESS;
}

int gpiomux_mmap_open(void)
{
	if ((gpio_mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "unable to open mmap file");
		return EXIT_FAILURE;
	}

	gpio_mmap_reg = (uint8_t*) mmap(NULL, 1024, PROT_READ | PROT_WRITE,
		MAP_FILE | MAP_SHARED, gpio_mmap_fd, MT76X8_SYSCTL_BASE);
	if (gpio_mmap_reg == MAP_FAILED) {
		perror("failed to mmap");
		fprintf(stderr, "failed to mmap");
		gpio_mmap_reg = NULL;
		close(gpio_mmap_fd);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void gpiomux_mmap_close(void) 
{
	close(gpio_mmap_fd);		
}

