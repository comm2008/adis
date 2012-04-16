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

#include "sync.h"
#include "common.h"

#define ADIS_DBLWORD_BIT(_op)        (op & 0x00200000)
#define ADIS_EXCL_BIT(_op)          (op & 0x00800000)

static void swap_instr(uint32_t op) {

    char *cond;
    cond = get_condition_string(op);

    if (ADIS_BYTE_BIT(op)) {
        // swap byte
        printf("SWP%sB R%d,R%d,[R%d]\n", cond, ADIS_RD(op), ADIS_RM(op),
            ADIS_RN(op));
    } else {
        // swap word
        printf("SWP%s R%d,R%d,[R%d]\n", cond, ADIS_RD(op), ADIS_RM(op),
            ADIS_RN(op));
    }
}

void sync_instr(uint32_t op) {

    char *cond, *size;

    if (!ADIS_EXCL_BIT(op)) {
        swap_instr(op);
        return;
    }

    cond = get_condition_string(op);

    if (ADIS_BYTE_BIT(op) && ADIS_DBLWORD_BIT(op)) {
        size = "H";
    } else if (ADIS_BYTE_BIT(op)) {
        size = "B";
    } else if (ADIS_DBLWORD_BIT(op)) {
        // Special case - two destination / source registers so just
        // decode the instruction separately
        if (ADIS_LOAD_BIT(op)) {
            printf("LDREXD%s, R%d,R%d,[R%d]\n", cond, ADIS_RD(op),
                ADIS_RD(op) + 1, ADIS_RN(op));
        } else {
            printf("STREXD%s R%d,R%d,R%d,[R%d]\n", cond, ADIS_RD(op),
                ADIS_RM(op), ADIS_RM(op) + 1, ADIS_RN(op));
        }
        // All done at this point
        return;
    } else {
        size = "";
    }

    if (ADIS_LOAD_BIT(op)) {
        printf("LDREX%s%s R%d,[R%d]\n", size, cond, ADIS_RD(op), ADIS_RN(op));
    } else {
        printf("STREX%s%s R%d,R%d,[R%d]\n", size, cond, ADIS_RD(op),
            ADIS_RM(op), ADIS_RN(op));
    }
}

