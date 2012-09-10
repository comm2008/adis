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

#include "misc.h"
#include "common.h"

#define ADIS_MISC_BX        0x0
#define ADIS_MISC_CLZ       0x1
#define ADIS_MISC_BXJ       0x2
#define ADIS_MISC_BLX       0x3
#define ADIS_MISC_BKPT      0x4
#define ADIS_MISC_SMC       0x5
#define ADIS_MISC_SAT       0x6
#define ADIS_MISC_UNKNOWN   0x7

static int get_misc_instr(uint32_t op)
{
    int op1 = (op & 0x00600000) >> 21;
    int op2 = (op & 0x00000070) >> 4;
    int instr = ADIS_MISC_UNKNOWN;

    // TODO(mhrheaume): There is probably a better way to do
    // this.
    switch (op2) {
    case (0b001):
        switch (op1) {
        case (0b01):
            instr = ADIS_MISC_BX;
            break;
        case (0b11):
            instr = ADIS_MISC_CLZ;
            break;
        default:
            break;
        }
        break;
    case (0b010):
        switch (op1) {
        case (0b01):
            instr = ADIS_MISC_BXJ;
            break;
        default:
            break;
        }
        break;
    case (0b011):
        switch (op1) {
        case (0b01):
            instr = ADIS_MISC_BLX;
            break;
        default:
            break;
        }
        break;
    case (0b101):
        instr = ADIS_MISC_SAT;
        break;
    case (0b111):
        switch (op1) {
        case (0b01):
            instr = ADIS_MISC_BKPT;
            break;
        case (0b11):
            instr = ADIS_MISC_SMC;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return instr;
}
        
static char *get_misc_instr_string(int misc_type)
{
    static char *opstr[6] = {"BX", "CLZ", "BXJ", "BLX", "BKPT", "SMC"};
    return opstr[misc_type];
}

static char *get_saturating_operation_string(uint32_t op)
{
    static char *opstr[4] = {"QADD", "QDADD", "QSUB", "QDSUB"};
    return opstr[(op & 0x00600000) >> 21];
}

static void saturating_instr(uint32_t op)
{
    char *cond, *opstr;

    cond = get_condition_string(op);
    opstr = get_saturating_operation_string(op);

    printf("%s%s R%d,R%d,R%d\n", opstr, cond, ADIS_RD(op), ADIS_RM(op),
        ADIS_RN(op));
}

void misc_instr(uint32_t op)
{
    int misc_type;
    char *cond, *opstr;

    misc_type = get_misc_instr(op);

    if (misc_type == ADIS_MISC_UNKNOWN) {
        printf("Unrecognized instruction 0x%x\n", op);
        return;
    }

    if (misc_type == ADIS_MISC_SAT) {
        saturating_instr(op);
        return;
    }

    cond = get_condition_string(op);
    opstr = get_misc_instr_string(misc_type);

    printf("%s%s ", opstr, cond);

    if (misc_type == ADIS_MISC_CLZ) {
        printf("R%d,R%d\n", ADIS_RD(op), ADIS_RM(op));
    } else if (misc_type == ADIS_MISC_BKPT) {
        printf("=0x%x\n", ((op & 0x000FFF00) << 4) || (op & 0x0000000F));
    } else if (misc_type == ADIS_MISC_SMC) {
        printf("=0x%x\n", op & 0x0000000F);
    } else {
        printf("R%d\n", ADIS_RM(op));
    }
}
