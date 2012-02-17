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
#include <string.h>

#include "common.h"

void get_offset_string(uint32_t op, char *buffer, size_t bsize, uint8_t dp) {

    uint32_t shift;

    if (op & 0x02000000) {
        // shift + register
        uint32_t reg = op & 0x0000000F;
        shift = (op & 0x00000FF0) >> 4;

        // dataproc instruction doesn't have +/-
        if (dp) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", reg);
        } else {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("xRxx")), "%sR%d", 
                op & 0x00800000 ? "+" : "-", reg);
        }

        if (reg < 10) {
            get_shift_string(shift, buffer - dp + 3, bsize + dp - 3);
        } else {
            get_shift_string(shift, buffer - dp + 4, bsize + dp - 4);
        }
    } else {
        // immediate value
        uint32_t imm = op & (dp ? 0x000000FF : 0x00000FFF);
        shift = (op & 0x00000F00) >> 8;
        // first two cases are only for dataproc
        if (dp && shift) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("#xxx,ROR #xxx")),
                "#%d,ROR #%d", imm, shift);
        } else if (dp) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("#xxx")), "#%d", imm);
        } else {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("=0xFFF")), 
                "=0x%.3X", imm);
        }
    }
}

char *get_condition_string(uint32_t op) {

    char *tmp;
    uint32_t cond = (op & 0xF0000000) >> 28;

    switch(cond) {
        case 0x00:
            // equal
            tmp = "EQ";
            break;
        case 0x01:
            // not equal
            tmp = "NE";
            break;
        case 0x02:
            // unsigned higher or same
            tmp = "CS";
            break;
        case 0x03:
            // unsigned lower
            tmp = "CC";
            break;
        case 0x04:
            // negative
            tmp = "MI";
            break;
        case 0x05:
            // positive or zero
            tmp = "PL";
            break;
        case 0x06:
            // overflow
            tmp = "VS";
            break;
        case 0x07:
            // no overflow
            tmp = "VC";
            break;
        case 0x08:
            // unsigned higher
            tmp = "HI";
            break;
        case 0x09:
            // unsigned lower or same
            tmp = "LS";
            break;
        case 0x0A:
            // greater or equal
            tmp = "GE";
            break;
        case 0x0B:
            // less than
            tmp = "LT";
            break;
        case 0x0C:
            // greater than
            tmp = "GT";
            break;
        case 0x0D:
            // less than or equal
            tmp = "LE";
            break;
        case 0x0E:
            // always
            tmp = "AL";
            break;
        case 0x0F:
            // never
            tmp = "NV";
            break;
    }

    return tmp;
}

void get_shift_string(uint32_t shift, char *buffer, size_t bsize) {

    char *s_type;

    if (shift & 0x01) {
        // shifted by amount in register
        uint32_t s_reg = shift & 0xE0 >> 4;
        switch (shift & 0x06 >> 1) {
            case 0x00:
                // logical left
                s_type = "LSL";
                break;
            case 0x01:
                // logical right
                s_type = "LSR";
                break;
            case 0x02:
                // arithmetic right
                s_type = "ASR";
                break;
            case 0x03:
                s_type = "ROR";
                break;
        }
        snprintf(buffer, ADIS_MIN(bsize, sizeof(",XXX Rxx")),
            ",%s R%d", s_type, s_reg);
    } else {
        uint32_t imm = shift & 0xF0 >> 3;
        if (imm != 0) {
            switch (shift & 0x06 >> 1) {
                case 0x00:
                    // logical left
                    s_type = "LSL";
                    break;
                case 0x01:
                    // logical right
                    s_type = "LSR";
                    break;
                case 0x02:
                    // arithmetic right
                    s_type = "ASR";
                    break;
                case 0x03:
                    // rotate right
                    s_type = "ROR";
                    break;
            }
            snprintf(buffer, ADIS_MIN(bsize, sizeof(",XXX #xx")),
                ",%s #%d", s_type, imm);
        } else if (bsize > 0) {
            buffer[0] = 0;
        }
    }
}

void get_addr_string(uint32_t op, uint8_t r_base, char *buffer, size_t bsize) {

    char offset[16];

    get_offset_string(op, offset, sizeof(offset), 0);

    // pre-indexed
    if (op & 0x01000000) {
        snprintf(buffer, ADIS_MIN(bsize, sizeof(r_base) + sizeof(offset) + 6),
            "[R%d,%s]%s", r_base, offset, op & 0x00200000 ? "!" : "");
    } else {
        snprintf(buffer, ADIS_MIN(bsize, sizeof(r_base) + sizeof(offset) + 5),
            "[R%d],%s", r_base, offset);
    }
}
