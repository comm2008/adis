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

#include "rt_coproc.h"
#include "common.h"

static inline uint8_t get_coproc_num(uint32_t op) {
    return (uint8_t)((op & 0x00000F00) >> 8);
}

static inline uint8_t get_coproc_operand_register(uint32_t op) {
    return (uint8_t)(op & 0x0000000F);
}

static inline uint8_t get_coproc_srcdest_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_arm_srcdest_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

static inline uint8_t get_coproc_info(uint32_t op) {
    return (uint8_t)((op & 0x00000E00) >> 5);
}

static inline uint8_t get_coproc_opmode(uint32_t op) {
    return (uint8_t)((op & 0x00E00000) >> 21);
}

void rt_coproc_instr(uint32_t op) {

    char cond[4], *opstr;
    uint8_t coproc_num, coproc_opmode, coproc_srcdest, coproc_operand;
    uint8_t coproc_info, r_srcdest;

    get_condition_string(op, cond, sizeof(cond));
    
    coproc_num = get_coproc_num(op);
    coproc_info = get_coproc_info(op);

    coproc_opmode = get_coproc_opmode(op);
    coproc_srcdest = get_coproc_srcdest_register(op);
    coproc_operand = get_coproc_operand_register(op);
    r_srcdest = get_arm_srcdest_register(op);

    if (op & 0x00100000) {
        opstr = "MRC";
    } else {
        opstr = "MCR";
    }

    if (coproc_info > 0) {
        printf("%s%s %d,%d,R%d,c%d,c%d,%d\n", opstr, cond, coproc_num,
            coproc_opmode, r_srcdest, coproc_srcdest, coproc_operand,
            coproc_info);
    } else {
        printf("%s%s %d,0x%x,R%d,c%d,c%d\n", opstr, cond, coproc_num,
            coproc_opmode, r_srcdest, coproc_srcdest, coproc_operand);
    }
}

