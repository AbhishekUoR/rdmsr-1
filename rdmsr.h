#ifndef __RDMSR_INCLUDE
#define __RDMSR_INCLUDE

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <fcntl.h>

int read_msr(int cpu, off_t msr, uint64_t *val);
int write_msr(int cpu, off_t msr, uint64_t val);


#endif /** __RDMSR_INCLUDE **/
