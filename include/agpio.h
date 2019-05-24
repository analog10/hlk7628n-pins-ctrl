#ifndef _APIO_H_
#define _APIO_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <omega2_ctrl_types.h>
#include <mt76x8_mem.h>

// functions
int 			agpio_set					(char *group, char *name);
int 			agpio_get					(void);

int 			agpio_mmap_open			(void);
void 			agpio_mmap_close			(void);

#define _MAX_FUNC	16

// structure
static struct agpio {
	char *name;
	char *func[_MAX_FUNC];
	unsigned int shift;
	unsigned int mask;
} AGpio[] =
{
	{
		.name = "wled_open_drain",
		.func = { "enabled", "disabled", NULL, NULL, 
				NULL, NULL, NULL, NULL,
				NULL, NULL, NULL, NULL,
				NULL, NULL, NULL, NULL
		},
		.shift = MT76X8_AGPIO_WLED_OD_EN,
		.mask = MT76X8_AGPIO_WLED_OD_EN_MASK,
	},
	{
		.name = "ephy0",
		.func = { "enabled", "disabled", NULL, NULL,
				NULL, NULL, NULL, NULL,
				NULL, NULL, NULL, NULL,
				NULL, NULL, NULL, NULL
		},
		.shift = MT76X8_AGPIO_EPHY_P0_DIS,
		.mask = MT76X8_AGPIO_EPHY_P0_DIS_MASK,
	},
	{
		.name = "ephy_1_4",
		.func = { "enabled", "d_p1", "d_p2", "d_p12",
				"d_p3", "d_p13", "d_p23", "d_p123",
				"d_p4", "d_p14", "d_p24", "d_p124",
				"d_p34", "d_p134", "d_p234", "disabled"
		 },
		.shift = MT76X8_AGPIO_EPHY_GPIO_AIO_EN,
		.mask = MT76X8_AGPIO_EPHY_GPIO_AIO_EN_MASK,
	},
	
};



#endif // _APIO_H_