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

#define ADIS_COND(_op)          ((_op & 0xF0000000) >> 28)

void get_offset_string(uint32_t op, char *buffer, size_t bsize, uint8_t dp)
{
    uint32_t shift;

    if (ADIS_IMMOP_BIT(op)) {
        // shift + register
        uint32_t reg = ADIS_RM(op);
        shift = (op & 0x00000FF0) >> 4;

        // dataproc instruction doesn't have +/-
        if (dp) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", reg);
        } else {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("xRxx")), "%sR%d",
                ADIS_ADDOFFSET_BIT(op) ? "+" : "-", reg);
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

char *get_condition_string(uint32_t op)
{
    static char *cond[16] = { "EQ", "NE", "CS", "CC",
                              "MI", "PL", "VS", "VC",
                              "HI", "LS", "GE", "LT",
                              "GT", "LE", "AL", "NV" };
    return cond[ADIS_COND(op)];
}

void get_shift_string(uint32_t shift, char *buffer, size_t bsize)
{
    static char *shiftstr[4] = {"LSL", "LSR", "ASR", "ROR"};

    if (shift & 0x01) {
        // shifted by amount in register
        uint32_t s_reg = shift & 0xE0 >> 4;
        snprintf(buffer, ADIS_MIN(bsize, sizeof(",XXX Rxx")),
            ",%s R%d", shiftstr[shift & 0x06 >> 1], s_reg);
    } else {
        uint32_t imm = shift & 0xF0 >> 3;
        if (imm != 0) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof(",XXX #xx")),
                ",%s #%d", shiftstr[shift & 0x06 >> 1], imm);
        } else if (bsize > 0) {
            buffer[0] = 0;
        }
    }
}

/*
 * Awful variable names because I didn't want the definition to go
 * over 80 characters:
 *
 *      r_bs -> base register
 *      offst -> offset string
 *      bfr -> buffer
 *      bsz -> buffer size
 */

void
get_addr_string(uint32_t op, uint8_t r_bs, char *offst, char *bfr, size_t bsz)
{
    // pre-indexed
    if (ADIS_PREINDEX_BIT(op)) {
        snprintf(bfr, ADIS_MIN(bsz, sizeof(r_bs) + sizeof(offst) + 6),
            "[R%d,%s]%s", r_bs, offst, ADIS_WRITE_BIT(op) ? "!" : "");
    } else {
        snprintf(bfr, ADIS_MIN(bsz, sizeof(r_bs) + sizeof(offst) + 5),
            "[R%d],%s", r_bs, offst);
    }
}
