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

#include "dt_halfword.h"
#include "dataswap.h"
#include "common.h"

#define ADIS_HW_BIT(_op)            (_op & 0x00000020)
#define ADIS_SIGNED_BIT(_op)        (_op & 0x00000040)

static void dthw_get_offset_string(uint32_t op, char *offset, size_t bsize) {
    snprintf(offset, ADIS_MIN(bsize, sizeof("=0xF")),
        ADIS_IMMOP_BIT(op) ? "=0x%.1X" : "R%d", ADIS_RM(op));
}

void dt_halfword_instr(uint32_t op) {
    char addr[32], offset[8], *subinstr, *cond;

    if (ADIS_HW_BIT(op) && ADIS_SIGNED_BIT(op)) {
        subinstr = "SH";
    } else if (!ADIS_HW_BIT(op) && ADIS_SIGNED_BIT(op)) {
        subinstr = "SB";
    } else if (ADIS_HW_BIT(op) && !ADIS_SIGNED_BIT(op)) {
        subinstr = "H";
    } else {
        data_swap_instr(op);
        return;
    }
    
    cond = get_condition_string(op);
    dthw_get_offset_string(op, offset, sizeof(offset));
    get_addr_string(op, ADIS_RN(op), offset, addr, sizeof(addr));

    if (ADIS_LOAD_BIT(op)) {
        printf("LDR%s%s R%d,%s\n", cond, subinstr, ADIS_RD(op), addr);
    } else {
        printf("STR%s%s R%d,%s\n", cond, subinstr, ADIS_RD(op), addr);
    }
}
