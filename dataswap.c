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

static void get_destination_string(uint32_t op, char *buffer, size_t bsize) {

    uint32_t regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_source_string(uint32_t op, char *buffer, size_t bsize) {

    uint32_t regnum = (op & 0x0000000F);
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_base_string(uint32_t op, char *buffer, size_t bsize) {

    uint32_t regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

void data_swap_instr(uint32_t op) {

    char cond[4], r_dest[4], r_src[4], r_base[4];

    get_condition_string(op, cond, sizeof(cond));
    get_destination_string(op, cond, sizeof(cond));
    get_source_string(op, cond, sizeof(cond));
    get_base_string(op, cond, sizeof(r_base));

    if (op & 0x00400000) {
        // swap byte
        printf("SWP%sB %s,%s,[%s]\n", cond, r_dest, r_src, r_base);
    } else {
        // swap word
        printf("SWP%s %s,%s,[%s]\n", cond, r_dest, r_src, r_base);
    }
}

