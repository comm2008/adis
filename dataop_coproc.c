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

#include "dataop_coproc.h"
#include "common.h"

static inline uint8_t get_opcode(uint32_t op) {
    return (uint8_t)((op & 0x00F00000) >> 20);
}

static inline uint8_t get_first_operand_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_second_operand_register(uint32_t op) {
    return (uint8_t)(op & 0x0000000F);
}

static inline uint8_t get_destination_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

static inline uint8_t get_coproc_num(uint32_t op) {
    return (uint8_t)((op & 0x00000F00) >> 8);
}

static inline uint8_t get_coproc_info(uint32_t op) {
    return (uint8_t)((op & 0x000000E0) >> 4);
}

void dataop_coproc_instr(uint32_t op) {
    uint8_t opcode, r_op1, r_op2, r_dest, coproc_num, coproc_info;
    char *cond;

    cond = get_condition_string(op);

    opcode = get_opcode(op);

    r_op1 = get_first_operand_register(op);
    r_op2 = get_second_operand_register(op);
    r_dest = get_destination_register(op);

    coproc_num = get_coproc_num(op);
    coproc_info = get_coproc_info(op);

    if (coproc_info > 0) {
        printf("CDP%s p%d,%d,c%d,c%d,c%d\n", cond, coproc_num,
            opcode, r_dest, r_op1, r_op2);
    } else {
        printf("CDP%s, p%d,%d,c%d,c%d,c%d,%d\n", cond, coproc_num,
            opcode, r_dest, r_op1, r_op2, coproc_info);
    }
}
