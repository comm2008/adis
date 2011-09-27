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
#include <string.h>
#include <stdio.h>

#define ADIS_MAX(op1, op2) ((op1 < op2) ? op2 : op1)
#define ADIS_MIN(op1, op2) ((op1 < op2) ? op1 : op2)

#define MAX_INSTR_LENGTH 64

void get_condition_string(uint32_t op, char* buffer, int bsize);
void get_shift_string(uint32_t shift, char* buffer, int bsize);

#endif  // __ADIS_COMMON_H__
