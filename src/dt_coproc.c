/*
 *  Copyright (C) 2012 Matthew Rheaume
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

#include <stdio.h>
#include <stdlib.h>

#include "dt_coproc.h"
#include "common.h"

#define ADIS_LONG_BIT(_op)      (_op & 0x00400000)

void dt_coproc_instr(uint32_t op) {

    char addr[32], offset[16], *long_bit, *cond;

    cond = get_condition_string(op);
    get_offset_string(op, offset, sizeof(offset), 0);
    get_addr_string(op, ADIS_RN(op), offset, addr, sizeof(addr));

    // long bit set?
    if (ADIS_LONG_BIT(op)) {
        long_bit = 'L';
    } else {
        long_bit = 0;
    }

    // load / store
    if (ADIS_LOAD_BIT(op)) {
        printf("LDC%s%c p%d,c%d,%s\n", cond, long_bit,
            ADIS_CPNUM(op), ADIS_RD(op), addr);
    } else {
        printf("STC%s%c p%d,c%d,%s\n", cond, long_bit,
            ADIS_CPNUM(op), ADIS_RD(op), addr);
    }
}
