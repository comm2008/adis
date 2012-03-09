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

#include <stdio.h>

#include "dt_single.h"
#include "common.h"

void dt_single_instr(uint32_t op) {

    char addr[32], offset[16], *cond, *tsize;

    cond = get_condition_string(op);
    get_offset_string(op, offset, sizeof(offset), 0);
    get_addr_string(op, ADIS_RN(op), offset, addr, sizeof(addr));

    // byte / word
    if (ADIS_BYTE_BIT(op)) {
        tsize = "B";
    } else {
        tsize = "";
    }

    // load / store
    if (ADIS_LOAD_BIT(op)) {
        printf("LDR%s%s R%d,%s\n", cond, tsize, ADIS_RD(op), addr);
    } else {
        printf("STR%s%s R%d,%s\n", cond, tsize, ADIS_RD(op), addr);
    }
}
