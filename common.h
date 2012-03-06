/*
 *  Copyright (C) 2011 Matthew Rheaume
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __ADIS_COMMON_H__
#define __ADIS_COMMON_H__

#include <stdint.h>

#define ADIS_MAX(_op1, _op2)        ((_op1 < _op2) ? _op2 : _op1)
#define ADIS_MIN(_op1, _op2)        ((_op1 < _op2) ? _op1 : _op2)

#define ADIS_RN(_op)                ((_op & 0x000F0000) >> 16)
#define ADIS_RD(_op)                ((_op & 0x0000F000) >> 12)
#define ADIS_RM(_op)                ((_op & 0x0000000F))

#define ADIS_CPNUM(_op)             ((_op & 0x00000F00) >> 8)
#define ADIS_CPINFO(_op)            ((_op & 0x00000E00) >> 5)

#define ADIS_LOAD_BIT(_op)          (_op & 0x00100000)
#define ADIS_SETCOND_BIT(_op)       (_op & 0x00100000)
#define ADIS_WRITE_BIT(_op)         (_op & 0x00200000)
#define ADIS_BYTE_BIT(_op)          (_op & 0x00400000)
#define ADIS_ADDOFFSET_BIT(_op)     (_op & 0x00800000)
#define ADIS_PREINDEX_BIT(_op)      (_op & 0x01000000)

#define MAX_INSTR_LENGTH 64

void get_offset_string(uint32_t op, char *buffer, size_t bsize, uint8_t dp);
char *get_condition_string(uint32_t op);
void get_shift_string(uint32_t shift, char *buffer, size_t bsize);
void get_addr_string(uint32_t op, uint8_t r_base, char *buffer, size_t bsize);

#endif  // __ADIS_COMMON_H__
