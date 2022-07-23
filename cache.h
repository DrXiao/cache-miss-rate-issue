#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdint.h>
#include <stdbool.h>

extern uint32_t cycle, mem_address, hit;

void system_init(char **);

void system_terminate(void);

bool cache_address_access(uint32_t);

#endif