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

static inline uint8_t get_coproc_num(uint32_t op) {
    return (uint8_t)((op & 0x00000F00) >> 8);
}

static inline uint8_t get_base_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_srcdest_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

void dt_coproc_instr(uint32_t op) {

    char cond[4], addr[32];
    char *long_bit;
    uint8_t coproc_num, r_srcdest;

    get_condition_string(op, cond, sizeof(cond));
    get_addr_string(op, get_base_register(op), addr, sizeof(addr));

    coproc_num = get_coproc_num(op);
    r_srcdest = get_srcdest_register(op);

    // long bit set?
    if (op & 0x00400000) {
        long_bit = "L";
    } else {
        long_bit = "";
    }

    // load / store
    if (op & 0x00100000) {
        printf("LDC%s%s p%d,c%d,%s\n", cond, long_bit,
            coproc_num, r_srcdest, addr);
    } else {
        printf("STC%s%s p%d,c%d,%s\n", cond, long_bit,
            coproc_num, r_srcdest, addr);
    }
}
