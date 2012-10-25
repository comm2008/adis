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

#include "dt_extra.h"
#include "sync.h"
#include "common.h"

#define ADIS_HW_BIT(_op)            (_op & 0x00000020)
#define ADIS_SIGNED_BIT(_op)        (_op & 0x00000040)
#define ADIS_UNPRIV_BIT(_op)        (_op & 0x00200000)

static inline int is_dt_dual(uint32_t op)
{
    return !((op & 0x00100040) ^ 0x00000040);
}

// Special offset calculating instruction used only by this instruction
// family
static void dtex_get_offset_string(uint32_t op, char *offset, size_t bsize)
{
    snprintf(offset, ADIS_MIN(bsize, sizeof("=0xF")),
        ADIS_IMMOP_BIT(op) ? "=0x%.1X" : "R%d", ADIS_RM(op));
}

void dt_extra_instr(uint32_t op)
{
    char addr[32], offset[8], *subinstr, *cond, unpriv;

    // Special bit combination for dual instructions,
    // other instructions are either signed, halfword,
    // both, or neither (regular data swap)
    if (is_dt_dual(op)) {
        subinstr = "D";
        unpriv = 0;
        goto have_subinstr;
    }

    if (!ADIS_HW_BIT(op) && !ADIS_SIGNED_BIT(op)) {
        // Regular SWP instruction, handled by sync_instr
        sync_instr(op);
        return;
    }

    if (ADIS_HW_BIT(op) && ADIS_SIGNED_BIT(op)) {
        subinstr = "SH";
    } else if (!ADIS_HW_BIT(op) && ADIS_SIGNED_BIT(op)) {
        subinstr = "SB";
    } else {
        subinstr = "H";
    }

    unpriv = ADIS_UNPRIV_BIT(op) ? 'T' : 0;

have_subinstr:
    cond = get_condition_string(op);
    dtex_get_offset_string(op, offset, sizeof(offset));
    get_addr_string(op, ADIS_RN(op), offset, addr, sizeof(addr));

    if (ADIS_LOAD_BIT(op)) {
        printf("LDR%s%c%s R%d,%s\n", subinstr, unpriv, cond, ADIS_RD(op), addr);
    } else {
        printf("STR%s%c%s R%d,%s\n", subinstr, unpriv, cond, ADIS_RD(op), addr);
    }
}
