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

#include "sdt.h"

static void get_srcdest_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_base_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_offset_string(uint32_t op, char* buffer, int bsize) {

    if (op & 0x02000000) {
        // shift + register
        int reg = op & 0x0000000F;
        int shift = (op & 0x00000FF0) >> 4;

        snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", reg);

        if (reg < 10) {
            get_shift_string(shift, buffer + 2, bsize - 2);
        } else {
            get_shift_string(shift, buffer + 3, bsize - 3);
        }
    } else {
        // immediate value
        snprintf(buffer, ADIS_MIN(bsize, sizeof("0xFFFFFFFF")), 
            "0x%.8X", op & 0x00000FFF);
    }
}

void sdt_instr(uint32_t op) {

    char cond[4], srcdest[4], base[4], offset[16];

    get_condition_string(op, cond, sizeof(cond));
    get_srcdest_string(op, srcdest, sizeof(srcdest));
    get_base_string(op, base, sizeof(base));
    get_offset_string(op, offset, sizeof(offset));

    // TODO
}
