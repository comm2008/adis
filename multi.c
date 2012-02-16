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

#include "multi.h"
#include "common.h"

// This is the exact same as get_destination_register
// I'll be changing this once I move to common functions
// or macros for retrieving register numbers
static inline uint8_t get_destination_hiregister(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_destination_loregister(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

static inline uint8_t get_destination_register(uint32_t op) {
    return (uint8_t)((op & 0x000F0000) >> 16);
}

static inline uint8_t get_first_operand_register(uint32_t op) {
    return (uint8_t)(op & 0x0000000F);
}

static inline uint8_t get_second_operand_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F00) >> 8);
}

static inline uint8_t get_third_operand_register(uint32_t op) {
    return (uint8_t)((op & 0x0000F000) >> 12);
}

static char *get_operation_string(uint32_t op) {

    char *ret;

    if ((op & 0x00800000) && (op & 0x00400000)) {
        ret = "UMLAL";
    } else if ((op & 0x00800000) && !(op & 0x00400000)) {
        ret = "UMULL";
    } else if (!(op & 0x00800000) && (op & 0x00400000)) {
        ret = "SMLAL";
    } else {
        ret = "SMULL";
    }

    return ret;
}

void multi_instr(uint32_t op) {

    char cond[4], *setcond;
    uint8_t r_first, r_second, r_dest;

    get_condition_string(op, cond, sizeof(cond));
    r_dest = get_destination_register(op);
    r_first = get_first_operand_register(op);
    r_second = get_second_operand_register(op);

    if (op & 0x00100000) {
        setcond = "S";
    } else {
        setcond = "";
    }

    if (op & 0x00200000) {
        // multiply and accumulate
        uint8_t r_third = get_third_operand_register(op);

        printf("MLA%s%s R%d,R%d,R%d,R%d\n", cond, setcond, r_dest, 
            r_first, r_second, r_third);
    } else {
        printf("MUL%s%s R%d,R%d,R%d\n", cond, setcond, r_dest,
            r_first, r_second);
    }
}

void long_multi_instr(uint32_t op) {

    char cond[4], *opstr, *setcond;
    uint8_t r_desthi, r_destlo, r_first, r_second;

    r_first = get_first_operand_register(op);
    r_second = get_second_operand_register(op);

    r_desthi = get_destination_hiregister(op);
    r_destlo = get_destination_loregister(op);
    
    get_condition_string(op, cond, sizeof(cond));
    opstr = get_operation_string(op);
    
    if (op & 0x00100000) {
        setcond = "S";
    } else {
        setcond = "";
    }

    printf("%s%s%s R%d,R%d,R%d,R%d\n", opstr, cond, setcond,
        r_destlo, r_desthi, r_first, r_second);
}
