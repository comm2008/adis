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

#define ADIS_MOVT_BIT(_op)      ((_op & 0x00400000))

#define ADIS_DATAPROC_AND   0x00
#define ADIS_DATAPROC_EOR   0x01
#define ADIS_DATAPROC_SUB   0x02
#define ADIS_DATAPROC_RSB   0x03
#define ADIS_DATAPROC_ADD   0x04
#define ADIS_DATAPROC_ADC   0x05
#define ADIS_DATAPROC_SBC   0x06
#define ADIS_DATAPROC_RSC   0x07
#define ADIS_DATAPROC_TST   0x08
#define ADIS_DATAPROC_TEQ   0x09
#define ADIS_DATAPROC_CMP   0x0A
#define ADIS_DATAPROC_CMN   0x0B
#define ADIS_DATAPROC_ORR   0x0C
#define ADIS_DATAPROC_MOV   0x0D
#define ADIS_DATAPROC_BIC   0x0E
#define ADIS_DATAPROC_MVN   0x0F
#define ADIS_DATAPROC_LSL   0x10
#define ADIS_DATAPROC_LSR   0x11
#define ADIS_DATAPROC_ASR   0x12
#define ADIS_DATAPROC_ROR   0x13
#define ADIS_DATAPROC_RRX   0x14
#define ADIS_DATAPROC_ADR   0x15

static inline int is_single_op(uint32_t opc)
{
    return (opc == ADIS_DATAPROC_MOV) ||
           (opc == ADIS_DATAPROC_MVN) ||
           (opc == ADIS_DATAPROC_ADR);
}

static inline int is_no_result(uint32_t opc)
{
    return (opc >= ADIS_DATAPROC_TST) && (opc <= ADIS_DATAPROC_CMN);
}

static inline char *get_operation_string(uint32_t opc)
{
    static char *opstr[22] = { "AND", "EOR", "SUB", "RSB",
                               "ADD", "ADC", "SBC", "RSC",
                               "TST", "TEQ", "CMP", "CMN",
                               "ORR", "MOV", "BIC", "MVN",
                               "LSL", "LSR", "ASR", "ROR",
                               "RRX", "ADR" };
    return opstr[opc];
}

static void data_proc_instr(uint32_t op, uint32_t opc)
{
    char offset[16], setcond, *cond, *opstr;
    get_offset_string(op, offset, sizeof(offset), 1);
    cond = get_condition_string(op);
    opstr = get_operation_string(opc);

    if (!is_no_result(opc)) {
        // check if condition code flag is set
        setcond = ADIS_SETCOND_BIT(op) ? 'S' : 0;

        if (is_single_op(opc)) {
            printf("%s%s%c R%d,%s\n", opstr, cond, setcond, ADIS_RD(op), offset);
        } else {
            printf("%s%s%c R%d,R%d,%s\n", opstr, cond, setcond, ADIS_RD(op),
                ADIS_RN(op), offset);
        }
    } else {
        printf("%s%s R%d,%s\n", opstr, cond, ADIS_RN(op), offset);
    }
}

void dp_reg_instr(uint32_t op)
{
    int op1 = ADIS_OPCODE(op), op2 = op & 0x00000F80, opc;

    if ((op1 != 0b1101) || (op1 == 0b1101 && !op2)) {
        opc = op1;
    } else {
        int op3 = (op & 0x00000060) >> 4;
        opc = ADIS_DATAPROC_LSL + (op3 == 0b11 ? op3 + !op2 : op3);
    }

    data_proc_instr(op, opc);
}

void dp_rsr_instr(uint32_t op)
{
    int op1 = ADIS_OPCODE(op), opc;

    if (op1 != 0b1101) {
        opc = op1;
    } else {
        int op2 = (op & 0x00000060) >> 4;
        opc = ADIS_DATAPROC_LSL + op2;
    }

    data_proc_instr(op, opc);
}

void dp_imm_instr(uint32_t op)
{
    int opc = ADIS_OPCODE(op);

    if (ADIS_RN(op) == 0b1111 &&
        (opc == ADIS_DATAPROC_SUB || opc == ADIS_DATAPROC_ADD)) {
        opc = ADIS_DATAPROC_ADR;
    }

    data_proc_instr(op, opc);
}

void dp_other_instr(uint32_t op)
{
    char *cond, subinstr;

    cond = get_condition_string(op);
    subinstr = ADIS_MOVT_BIT(op) ? 'T' : 'W';

    printf("MOV%c R%d,=0x%x\n", subinstr, ADIS_RD(op),
        ((op & 0x000F0000) >> 4 | (op & 0x00000FFF)));
}
