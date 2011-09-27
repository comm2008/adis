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

typedef enum _PSR_FLAG {
    PSR_FLAG_NONE,
    PSR_FLAG_BITS
} PSR_FLAG;

static int is_mrs(uint32_t op) {
    return !((op & 0x0FBF0FFF) ^ 0x010F0000);
}

static int is_msr(uint32_t op) {
    return !((op & 0x0FBFFFF0) ^ 0x012CF000);
}

static int is_msr_flg(uint32_t op) {
    return !((op & 0x0DBFF000) ^ 0x0128F000);
}

static int is_single_op(uint32_t op) {

    int opcode = (op & 0x01E00000) >> 21;

    return (opcode == 0x1101) || (opcode == 0x1111);
}

static int is_no_result(uint32_t op) {
    
    int opcode = (op & 0x01E00000) >> 21;

    return (opcode >= 0x1000) && (opcode <= 0x1011);
}

static void get_op_string(uint32_t op, char* buffer, int bsize) {

    char* tmp;
    int opcode = (op & 0x01E00000) >> 21;

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

static void get_destination_string(uint32_t op, char* buffer, int bsize) {
    
    int regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_first_operand_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_second_operand_string(uint32_t op, char* buffer, int bsize) {

    int shift;

    if (!(op & 0x02000000)) {
        
        int reg = op & 0x0000000F;
        shift = (op & 0x00000FF0) >> 4;

        snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", reg);
        
        if (reg < 10) {
            get_shift_string(shift, buffer + 2, bsize - 2);
        } else {
            get_shift_string(shift, buffer + 3, bsize - 3);
        }
    } else {
        // operand is immediate value
        int imm = op & 0x000000FF;
        shift = (op & 0x00000F00) >> 8;

        if (shift) {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("#xxx,ROR #xxx")),
                "#%d,ROR #%d", imm, shift);
        } else {
            snprintf(buffer, ADIS_MIN(bsize, sizeof("#xxx")), "#%d", imm);
        }

    }
}

static void data_proc_instr(uint32_t op) {

    char cond[4], opstr[4], rSecond[16];
    char *setcond;
    
    get_op_string(op, opstr, sizeof(opstr));
    get_condition_string(op, cond, sizeof(cond));
    get_second_operand_string(op, rSecond, sizeof(rSecond));

    if (is_no_result(op)) {
        char rFirst[4];
        get_first_operand_string(op, rFirst, sizeof(rFirst));
       
        printf("%s%s %s,%s\n", opstr, cond, rFirst, rSecond);
    } else {
        char rDest[4];
        // check if condition code flag is set
        if (op & 0x00100000) {
            setcond = "S";
        } else {
            setcond = "";
        }

        get_destination_string(op, rDest, sizeof(rDest));

        if (is_single_op(op)) {
            printf("%s%s%s %s,%s\n", opstr, cond, setcond, 
                rDest, rSecond);
        } else {
            char rFirst[4];
            get_first_operand_string(op, rFirst, sizeof(rFirst));
            printf("%s%s%s %s,%s,%s\n", opstr, cond, setcond,
                rDest, rFirst, rSecond);
        }
    }
}

static void mrs_instr(uint32_t op) {
    
    char cond[4], rDest[4];
    char* psr;

    get_condition_string(op, cond, sizeof(cond));
    get_destination_string(op, rDest, sizeof(rDest));

    if (op & 0x00400000) {
        // source PSR = SPSR_<currentmode>
        psr = "SPSR";
    } else {
        psr = "CPSR";
    }

    printf("MRS%s %s,%s\n", cond, rDest, psr);
}

static void msr_instr(uint32_t op, PSR_FLAG flg) {
    
    char cond[4], rSecond[16];
    char* psr;

    get_condition_string(op, cond, sizeof(cond));
    get_second_operand_string(op, rSecond, sizeof(rSecond));

    if (op & 0x00400000) {
        // destination psr = SPSR_<currentmode>
        if (flg == PSR_FLAG_BITS) {
            psr = "SPSR_flg";
        } else {
            psr = "SPSR";
        }
    } else {
        if (flg == PSR_FLAG_BITS) {
            psr = "CPSR_flg";
        } else {
            psr = "CPSR";
        }
    }

    printf("MSR%s %s, %s\n", cond, psr, rSecond);
}

void dp_psr_instr(uint32_t op) {

    if (is_msr(op)) {
        msr_instr(op, PSR_FLAG_NONE);
    } else if (is_msr_flg(op)) { 
        msr_instr(op, PSR_FLAG_BITS);
    } else if (is_mrs(op)) {
        mrs_instr(op);
    } else {
        data_proc_instr(op);
    }

}
