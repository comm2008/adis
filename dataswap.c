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

#include "dataswap.h"
#include "common.h"

static inline uint8_t get_destination_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

static inline uint8_t get_source_register(uint32_t op) {
    return (uint8_t)(op & 0x0000000F);
}

static inline uint8_t get_base_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

void data_swap_instr(uint32_t op) {

    char cond[4];
    uint8_t r_dest, r_src, r_base;

    get_condition_string(op, cond, sizeof(cond));

    r_dest = get_destination_register(op);
    r_src = get_source_register(op);
    r_base = get_base_register(op);

    if (op & 0x00400000) {
        // swap byte
        printf("SWP%sB R%d,R%d,[R%d]\n", cond, r_dest, r_src, r_base);
    } else {
        // swap word
        printf("SWP%s R%d,R%d,[R%d]\n", cond, r_dest, r_src, r_base);
    }
}

