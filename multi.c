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

static void get_destination_string(uint32_t op, char *buffer, size_t bsize) {
    
    uint32_t regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_first_operand_string(uint32_t op, char *buffer, size_t bsize) {

    uint32_t regnum = (op & 0x00000F00) >> 8;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_second_operand_string(uint32_t op, char *buffer, size_t bsize) {

    uint32_t regnum = (op & 0x0000000F);
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_third_operand_string(uint32_t op, char *buffer, size_t bsize) {
    
    uint32_t regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

void multi_instr(uint32_t op) {

    char cond[4], r_dest[4], r_first[4], r_second[4];
    char *setcond;

    get_condition_string(op, cond, sizeof(cond));
    get_destination_string(op, r_dest, sizeof(r_dest));
    get_first_operand_string(op, r_first, sizeof(r_first));
    get_second_operand_string(op, r_second, sizeof(r_second));

    if (op & 0x00100000) {
        setcond = "S";
    } else {
        setcond = "";
    }

    if (op & 0x00200000) {
        // multiply and accumulate
        char r_third[4];

        get_third_operand_string(op, r_third, sizeof(r_third));
        printf("MLA%s%s %s,%s,%s,%s\n", cond, setcond, r_dest, 
            r_first, r_second, r_third);
    } else {
        printf("MUL%s%s %s,%s,%s\n", cond, setcond, r_dest,
            r_first, r_second);
    }
}
