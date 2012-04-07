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

#include "satop.h"
#include "common.h"

#define ADIS_SUBTRACT_BIT(_op)  (_op & 0x00200000)
#define ADIS_DOUBLE_BIT(_op)    (_op & 0x00400000)

static char *get_operation_string(uint32_t op) {

    char *ret;

    if (ADIS_SUBTRACT_BIT(op) && ADIS_DOUBLE_BIT(op)) {
        ret = "QDSUB";
    } else if (ADIS_SUBTRACT_BIT(op) && !ADIS_DOUBLE_BIT(op)) {
        ret = "QSUB";
    } else if (!ADIS_SUBTRACT_BIT(op) && ADIS_DOUBLE_BIT(op)) {
        ret = "QDADD";
    } else {
        ret = "QADD";
    }

    return ret;
}

void saturating_instr(uint32_t op) {

    char *cond, *opstr;

    cond = get_condition_string(op);
    opstr = get_operation_string(op);

    printf("%s%s R%d,R%d,R%d\n", opstr, cond, ADIS_RD(op), ADIS_RM(op),
        ADIS_RN(op));
}
