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

typedef enum _PSRFlag {
    PSRFlagNone,
    PSRFlagBits
} PSRFlag;

static inline int is_mrs(uint32_t op) {
    return !((op & 0x0FBF0FFF) ^ 0x010F0000);
}

static inline int is_msr(uint32_t op) {
    return !((op & 0x0FBFFFF0) ^ 0x012CF000);
}

static inline int is_msr_flg(uint32_t op) {
    return !((op & 0x0DBFF000) ^ 0x0128F000);
}

static inline int is_single_op(uint32_t op) {
    int opcode = ADIS_OPCODE(op);
    return (opcode == 0x1101) || (opcode == 0x1111);
}

static inline int is_no_result(uint32_t op) {
    int opcode = ADIS_OPCODE(op);
    return (opcode >= 0x1000) && (opcode <= 0x1011);
}

static void get_operation_string(uint32_t op, char *buffer, size_t bsize) {

    char *tmp;
    uint32_t opcode = ADIS_OPCODE(op);

    switch(opcode) {
        case 0x00:
            // AND operation
            tmp = "AND";
            break;
        case 0x01:
            // XOR / EOR operation
            tmp = "TMP";
            break;
        case 0x02:
            // subtraction
            tmp = "SUB";
            break;
        case 0x03:
            // reverse subtraction
            tmp = "RSB";
            break;
        case 0x04:
            // addition
            tmp = "ADD";
            break;
        case 0x05:
            // add + carry
            tmp = "ADC";
            break;
        case 0x06:
            // subtract + carry
            tmp = "SBC";
            break;
        case 0x07:
            // reverse subtract + carry
            tmp = "RSC";
            break;
        case 0x08:
            // set condition codes op1 AND op2
            tmp = "TST";
            break;
        case 0x09:
            // set condition codes op1 EOR op2
            tmp = "TEQ";
            break;
        case 0x0A:
            // set condition codes op1 - op2
            tmp = "CMP";
            break;
        case 0x0B:
            // set condition codes op1 + op2
            tmp = "CMN";
            break;
        case 0x0C:
            // OR operation
            tmp = "ORR";
            break;
        case 0x0D:
            // MOV (Rd := op2)
            tmp = "MOV";
            break;
        case 0x0E:
            // BIC (Rd := op1 AND NOT op2)
            tmp = "BIC";
            break;
        case 0x0F:
            // MVN (Rd := NOT op2)
            tmp = "MVN";
            break;
    }

    memcpy(buffer, tmp, ADIS_MIN(bsize, sizeof(tmp)));
}

static void data_proc_instr(uint32_t op) {

    char opstr[4], offset[16], *cond, *setcond;
    
    cond = get_condition_string(op);
    get_operation_string(op, opstr, sizeof(opstr));
    get_offset_string(op, offset, sizeof(offset), 1);

    if (is_no_result(op)) {
        printf("%s%s R%d,%s\n", opstr, cond, ADIS_RN(op), offset);
    } else {
        // check if condition code flag is set
        if (ADIS_SETCOND_BIT(op)) {
            setcond = "S";
        } else {
            setcond = "";
        }

        if (is_single_op(op)) {
            printf("%s%s%s R%d,%s\n", opstr, cond, setcond, 
                ADIS_RD(op), offset);
        } else {
            printf("%s%s%s R%d,R%d,%s\n", opstr, cond, setcond,
                ADIS_RD(op), ADIS_RN(op), offset);
        }
    }
}

static void mrs_instr(uint32_t op) {
    
    char *cond, *psr;
    cond = get_condition_string(op);

    if (ADIS_SPSR_BIT(op)) {
        // source PSR = SPSR_<currentmode>
        psr = "SPSR";
    } else {
        psr = "CPSR";
    }

    printf("MRS%s R%d,%s\n", cond, ADIS_RD(op), psr);
}

static void msr_instr(uint32_t op, PSRFlag flg) {
    
    char offset[16], *cond, *psr;

    cond = get_condition_string(op);
    get_offset_string(op, offset, sizeof(offset), 1);

    if (ADIS_SPSR_BIT(op)) {
        // destination psr = SPSR_<currentmode>
        if (flg == PSRFlagBits) {
            psr = "SPSR_flg";
        } else {
            psr = "SPSR";
        }
    } else {
        if (flg == PSRFlagBits) {
            psr = "CPSR_flg";
        } else {
            psr = "CPSR";
        }
    }

    printf("MSR%s %s,%s\n", cond, psr, offset);
}

void dp_psr_instr(uint32_t op) {

    if (is_msr(op)) {
        msr_instr(op, PSRFlagNone);
    } else if (is_msr_flg(op)) { 
        msr_instr(op, PSRFlagBits);
    } else if (is_mrs(op)) {
        mrs_instr(op);
    } else {
        data_proc_instr(op);
    }

}
