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

static inline uint8_t get_base_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_srcdest_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

void dt_single_instr(uint32_t op) {

    char addr[32], *cond, *tsize;
    uint8_t r_srcdest;

    r_srcdest = get_srcdest_register(op);

    cond = get_condition_string(op);
    get_addr_string(op, get_base_register(op), addr, sizeof(addr));

    // byte / word
    if (op & 0x00400000) {
        tsize = "B";
    } else {
        tsize = "";
    }

    // load / store
    if (op & 0x00100000) {
        printf("LDR%s%s R%d,%s\n", cond, tsize, r_srcdest, addr);
    } else {
        printf("STR%s%s R%d,%s\n", cond, tsize, r_srcdest, addr);
    }
}
