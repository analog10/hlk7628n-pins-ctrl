#ifndef _GPIOMUX_H_
#define _GPIOMUX_H_

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
int 			gpiomux_set					(char *group, char *name);
int 			gpiomux_get					(void);

int 			gpiomux_mmap_open			(void);
void 			gpiomux_mmap_close			(void);

// structure
static struct gpiomux {
	char *name;
	char *func[4];
	unsigned int shift;
	unsigned int mask;
} omega2GpioMux[] =
{
	{
		.name = "i2c",
		.func = { "i2c", "gpio", "s-uart", NULL },
		.shift = MT76X8_GPIO1_MODE_I2C_MODE,
		.mask = MT76X8_GPIO1_MODE_I2C_MODE_MASK,
	},
	{
		.name = "uart0",
		.func = { "uart", "gpio", NULL, NULL },
		.shift = MT76X8_GPIO1_MODE_UART0_MODE,
		.mask = MT76X8_GPIO1_MODE_UART0_MODE_MASK,
	},
	{
		.name = "uart1",
		// what is "sw_r_sw_t" ?
		//.func = { "uart", "gpio", "pwm0_pwm1", NULL },
		.func = { "uart", "gpio", "pwm0_pwm1", "sw_r_sw_t" },
		.shift = MT76X8_GPIO1_MODE_UART1_MODE,
		.mask = MT76X8_GPIO1_MODE_UART1_MODE_MASK,
	},
	{
		.name = "uart2",
		// "sdxc_d5_d4" not useful for omega2
		//.func = { "uart", "gpio", "pwm2_pwm3", NULL },
		.func = { "uart", "gpio", "pwm2_pwm3", "sdxc_d5_d4" },
		.shift = MT76X8_GPIO1_MODE_UART2_MODE,
		.mask = MT76X8_GPIO1_MODE_UART2_MODE_MASK,
	},
	{
		.name = "pwm0",
		// "utif[4]", "sdxc_d7" not useful for omega2
		//.func = { "pwm", "gpio", NULL, NULL },
		.func = { "pwm", "gpio", "utif[4]", "sdxc_d7" },
		.shift = MT76X8_GPIO1_MODE_PWM0_MODE,
		.mask = MT76X8_GPIO1_MODE_PWM0_MODE_MASK,
	},
	{
		.name = "pwm1",
		// "utif[5]", "sdxc_d6" not useful for omega2
		//.func = { "pwm1", "gpio", NULL, NULL },
		.func = { "pwm1", "gpio", "utif[5]", "sdxc_d6" },
		.shift = MT76X8_GPIO1_MODE_PWM1_MODE,
		.mask = MT76X8_GPIO1_MODE_PWM1_MODE_MASK,
	},
	{
		.name = "refclk",
		.func = { "refclk", "gpio", NULL, NULL },
		.shift = MT76X8_GPIO1_MODE_REFCLK_MODE,
		.mask = MT76X8_GPIO1_MODE_REFCLK_MODE_MASK,
	},
	{
		.name = "spi_s",
		.func = { "spi_s", "gpio", "utif[3:0]", "pwm01_uart2" },
		.shift = MT76X8_GPIO1_MODE_SPIS_MODE,
		.mask = MT76X8_GPIO1_MODE_SPIS_MODE_MASK,
	},
	{
		.name = "spi_cs1",
		.func = { "spi_cs1", "gpio", "refclk", NULL },
		.shift = MT76X8_GPIO1_MODE_SPI_CS1_MODE,
		.mask = MT76X8_GPIO1_MODE_SPI_CS1_MODE_MASK,
	},
	{
		.name = "i2s",
		.func = { "i2s", "gpio", "pcm", "antsel[5:2]" },
		.shift = MT76X8_GPIO1_MODE_I2S_MODE,
		.mask = MT76X8_GPIO1_MODE_I2S_MODE_MASK,
	},
	{
		.name = "ephy0_led",
		.func = { "ephy0_led", "gpio", NULL, "jtag_jtdo" },
		.shift = MT76X8_SYSCTL_REGISTERS_WIDTH + MT76X8_GPIO2_MODE_P0_LED_AN_MODE,
		.mask = MT76X8_GPIO2_MODE_P0_LED_AN_MODE_MASK,
	},
	{
		.name = "wled",
		.func = { "wled", "gpio", NULL, NULL },
		.shift = MT76X8_SYSCTL_REGISTERS_WIDTH + MT76X8_GPIO2_MODE_WLED_AN_MODE,
		.mask = MT76X8_GPIO2_MODE_WLED_AN_MODE_MASK,
	}
};


#endif // _GPIOMUX_H_