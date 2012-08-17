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

#include "dataproc.h"
#include "common.h"

#define ADIS_OPCODE(_op)        ((_op & 0x01E00000) >> 21)

#define ADIS_SPSR_BIT(_op)      (_op & 0x00400000)

#define ADIS_PSRFLAG_NONE       0x0
#define ADIS_PSRFLAG_BITS       0x1

static inline int is_mrs(uint32_t op)
{
    return !((op & 0x0FBF0FFF) ^ 0x010F0000);
}

static inline int is_msr(uint32_t op)
{
    return !((op & 0x0FBFFFF0) ^ 0x012CF000);
}

static inline int is_msr_flg(uint32_t op) {
    return !((op & 0x0DBFF000) ^ 0x0128F000);
}

static inline int is_single_op(uint32_t op)
{
    int opcode = ADIS_OPCODE(op);
    return (opcode == 0x1101) || (opcode == 0x1111);
}

static inline int is_no_result(uint32_t op)
{
    int opcode = ADIS_OPCODE(op);
    return (opcode >= 0x1000) && (opcode <= 0x1011);
}

static void data_proc_instr(uint32_t op)
{
    char offset[16], *cond, setcond;
    static char *opstr[16] = { "AND", "EOR", "SUB", "RSB",
                               "ADD", "ADC", "SBC", "RSC",
                               "TST", "TEQ", "CMP", "CMN",
                               "ORR", "MOV", "BIC", "MVN" };

    get_offset_string(op, offset, sizeof(offset), 1);
    cond = get_condition_string(op);

    if (!is_no_result(op)) {
        // check if condition code flag is set
        setcond = ADIS_SETCOND_BIT(op) ? 'S' : 0;

        if (is_single_op(op)) {
            printf("%s%s%c R%d,%s\n", opstr[ADIS_OPCODE(op)], cond,
                setcond, ADIS_RD(op), offset);
        } else {
            printf("%s%s%c R%d,R%d,%s\n", opstr[ADIS_OPCODE(op)], cond,
                setcond, ADIS_RD(op), ADIS_RN(op), offset);
        }
    } else {
        printf("%s%s R%d,%s\n", opstr[ADIS_OPCODE(op)], cond, ADIS_RN(op), offset);
    }
}

static void mrs_instr(uint32_t op)
{
    char *cond, *psr;

    cond = get_condition_string(op);
    psr = ADIS_SPSR_BIT(op) ? "SPSR" : "CPSR";

    printf("MRS%s R%d,%s\n", cond, ADIS_RD(op), psr);
}

static void msr_instr(uint32_t op, uint32_t flg)
{
    char offset[16], *cond, *psr;

    get_offset_string(op, offset, sizeof(offset), 1);
    cond = get_condition_string(op);
    psr = ADIS_SPSR_BIT(op) ?
          (flg == ADIS_PSRFLAG_BITS ? "SPSR_flg" : "SPSR") :
          (flg == ADIS_PSRFLAG_BITS ? "CPSR_flg" : "CPSR");

    printf("MSR%s %s,%s\n", cond, psr, offset);
}

void dp_psr_instr(uint32_t op)
{
    if (is_msr(op)) {
        msr_instr(op, ADIS_PSRFLAG_NONE);
    } else if (is_msr_flg(op)) { 
        msr_instr(op, ADIS_PSRFLAG_BITS);
    } else if (is_mrs(op)) {
        mrs_instr(op);
    } else {
        data_proc_instr(op);
    }

}
