#include <agpio.h>




void __agpio_set(unsigned int mask, unsigned int shift, unsigned int val)
{
	volatile uint32_t *agpio_cfg = (volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_AGPIO_CFG);
	unsigned int v;

	v = *agpio_cfg;
	v &= ~(mask << shift);
	v |= (val << shift);
	*(volatile uint32_t*) (agpio_cfg) = v;
}

int agpio_set(char *group, char *name)
{
	int i;
	int id;

	for (id = 0; id < _O2_NUM_GPIO_MUX; id ++)
		if (!strcmp(AGpio[id].name, group))
			break;

	if (id < _O2_NUM_GPIO_MUX) for (i = 0; i < _MAX_FUNC; i++) {
		if (!AGpio[id].func[i] || strcmp(AGpio[id].func[i], name))
			continue;
		__agpio_set(AGpio[id].mask, AGpio[id].shift, i);
		printf("set agpio %s -> %s\n", AGpio[id].name, name);
		return EXIT_SUCCESS;
	}
	fprintf(stderr, "unknown group/function combination\n");
	return EXIT_FAILURE;
}

int agpio_get(void)
{
	unsigned int agpio_cfg = *(volatile uint32_t*) (mmap_reg + MT76X8_SYSCTL_AGPIO_CFG);
	int id;

	for (id = 0; id < _O2_NUM_AGPIO; id ++) {
		unsigned int val;
		int i;

		
		val = (agpio_cfg >> AGpio[id].shift) & AGpio[id].mask;

		printf("Group %s - ", AGpio[id].name);
		for (i = 0; i < _MAX_FUNC; i++) {
			if (!AGpio[id].func[i])
				continue;
			if (i == val)
				printf("[%s] ", AGpio[id].func[i]);
			else
				printf("%s ", AGpio[id].func[i]);
		}
		printf("\n");
	}

	return EXIT_SUCCESS;
}





