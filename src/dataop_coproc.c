/*
 *  Copyright (C) 2012 Matthew Rheaume
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
#include <stdlib.h>

#include "dataop_coproc.h"
#include "common.h"

#define ADIS_OPCODE(_op)        ((_op & 0x00F00000) >> 20)

void dataop_coproc_instr(uint32_t op)
{
    uint32_t coproc_info;
    char *cond;

    cond = get_condition_string(op);
    coproc_info = ADIS_CPINFO(op);

    if (coproc_info > 0) {
        printf("CDP%s p%d,%d,c%d,c%d,c%d\n", cond, ADIS_CPNUM(op),
            ADIS_OPCODE(op), ADIS_RD(op), ADIS_RN(op), ADIS_RM(op));
    } else {
        printf("CDP%s, p%d,%d,c%d,c%d,c%d,%d\n", cond, ADIS_CPNUM(op),
            ADIS_OPCODE(op), ADIS_RD(op), ADIS_RN(op), 
            ADIS_RM(op), coproc_info);
    }
}
