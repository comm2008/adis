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

// For some reason, these two registers are reversed
// in this instruction
#undef ADIS_RD
#undef ADIS_RN

#define ADIS_RD(_op)    ((_op & 0x000F0000) >> 16)
#define ADIS_RN(_op)    ((_op & 0x0000F000) >> 12)
#define ADIS_RS(_op)    ((_op & 0x00000F00) >> 8)

// Same as ADIS_RD and ADIS_RN, just different names
// for long multiplication
#define ADIS_RDHI(_op)          ((_op & 0x000F0000) >> 16)
#define ADIS_RDLO(_op)          ((_op & 0x0000F000) >> 12)

#define ADIS_ACCUM_BIT(_op)     (_op & 0x00200000)
#define ADIS_SIGNED_BIT(_op)    (_op & 0x00400000)
#define ADIS_LONG_BIT(_op)      (_op & 0x00800000)

static int is_mls_instr(uint32_t op) {
    return !((op & 0x00F00000) ^ 0x00600000);
}

// Used for long multiplication instructions
static char *get_operation_string(uint32_t op) {

    char *ret;

    if (ADIS_SIGNED_BIT(op) && ADIS_ACCUM_BIT(op)) {
        ret = "UMLAL";
    } else if (ADIS_SIGNED_BIT(op) && !ADIS_ACCUM_BIT(op)) {
        ret = "UMULL";
    } else if (!ADIS_SIGNED_BIT(op) && ADIS_ACCUM_BIT(op)) {
        ret = "SMLAL";
    } else {
        ret = "SMULL";
    }

    return ret;
}

static void long_multi_instr(uint32_t op) {

    char *cond, *opstr, *setcond;

    cond = get_condition_string(op);
    opstr = get_operation_string(op);
    
    if (ADIS_SETCOND_BIT(op)) {
        setcond = "S";
    } else {
        setcond = "";
    }

    printf("%s%s%s R%d,R%d,R%d,R%d\n", opstr, cond, setcond, ADIS_RDLO(op),
        ADIS_RDHI(op), ADIS_RM(op), ADIS_RS(op));
}

void multi_instr(uint32_t op) {

    char *cond, *setcond;
    cond = get_condition_string(op);

    if (is_mls_instr(op)) {
        // We can process this immediately without checking other bits
        printf("MLS%s R%d,R%d,R%d,R%d\n", cond, ADIS_RD(op), ADIS_RN(op),
            ADIS_RS(op), ADIS_RM(op));
        return;
    } else if (ADIS_LONG_BIT(op)) {
        // long multiplication instruction
        long_multi_instr(op);
        return;
    }

    // If we get here, this is just a regular multiplication instruction
    if (ADIS_SETCOND_BIT(op)) {
        setcond = "S";
    } else {
        setcond = "";
    }

    if (ADIS_ACCUM_BIT(op)) {
        // multiply and accumulate
        printf("MLA%s%s R%d,R%d,R%d,R%d\n", cond, setcond, ADIS_RD(op), 
            ADIS_RM(op), ADIS_RS(op), ADIS_RN(op));
    } else {
        printf("MUL%s%s R%d,R%d,R%d\n", cond, setcond, ADIS_RD(op),
            ADIS_RM(op), ADIS_RS(op));
    }
}
