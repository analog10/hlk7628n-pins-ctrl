#include <gpiomux.h>


void __gpiomux_set(unsigned int mask, unsigned int shift, unsigned int val)
{
	volatile uint32_t *gpio_mode = (volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_GPIO1_MODE);
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
	unsigned int gpio1_mode = *(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_GPIO1_MODE);
	unsigned int gpio2_mode = *(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_GPIO2_MODE);
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
