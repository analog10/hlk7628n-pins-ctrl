#ifndef _REFCLK_H_
#define _REFCLK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <omega2_ctrl_types.h>
#include <mt76x8_mem.h>
#include <regaccess.h>

// functions
int 		refclk_set						(unsigned int rate);
int 		refclk_get						(void);

#endif // _REFCLK_H_