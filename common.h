#ifndef __ADIS_COMMON_H__
#define __ADIS_COMMON_H__

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ADIS_MAX(op1, op2) ((op1 < op2) ? op2 : op1)
#define ADIS_MIN(op1, op2) ((op1 < op2) ? op1 : op2)

#define MAX_INSTR_LENGTH 64

void get_condition_string(uint32_t op, char* buffer, int bsize);
void get_shift_string(uint32_t shift, char* buffer, int bsize);

#endif  // __ADIS_COMMON_H__
