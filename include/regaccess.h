#ifndef _REGACCESS_H_
#define _REGACCESS_H_

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

// variables

extern uint8_t* mmap_reg;
extern int mmap_fd;

// functions

int 			mmap_open		(void);
void 			mmap_close		(void);


#endif // _OMEGA2_CTRL_TYPES_H_