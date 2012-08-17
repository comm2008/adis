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

#include "dt_block.h"
#include "common.h"

#define ADIS_INIT_ALLOC 16

#define ADIS_PSR_BIT(_op)       (_op & 0x00400000)

static char *get_addr_mode_string(uint32_t op)
{
    static char *addr_mode[4] = { "DA", "IA", "DB", "IB" };
    return addr_mode[ADIS_ADDOFFSET_BIT(op) | (ADIS_PREINDEX_BIT(op) << 1)];
}

static void get_register_list_string(uint32_t op, char **buffer)
{
    char *ret = malloc(sizeof(char)*ADIS_INIT_ALLOC);
    uint32_t pos = 0, max = ADIS_INIT_ALLOC, before = 0, i = 0;

    if (ret == NULL) {
        printf("BDT: Failed to allocate memory.\n");
        return;
    }

    // mask away top bits
    op &= 0x0000FFFF;
    ret[0] = '{';

    for ( ; (i < 16) && (op > 0); i++) {
        if ((pos + sizeof("Rxx,") + 1) >= max) {
            // reallocate
            max *= 2;
            if (realloc(ret, sizeof(char)*max) == NULL) {
                printf("BDT: Failed to allocate memory.\n");
                return;
            }
        }

        if ((op & 0x00000001) && !before) {
            // add to list
            pos++;
            snprintf(ret + pos, sizeof("Rxx,"), "R%d,", i);
            pos += i < 10 ? 2 : 3;
        } else if (!(op & 0x00000001) && before) {
            // add to list with dash preceding
            snprintf(ret + pos, sizeof("-Rxx,"), "-R%d,", i - 1);
            pos += i < 10 ? 3 : 4;
        }

        op >>= 1;
    }

    ret[pos] = '}';
    *buffer = ret;
}

void dt_block_instr(uint32_t op)
{
    char *addr_mode, *cond, psr, wb, *r_list = NULL;

    get_register_list_string(op, &r_list);
    if (r_list == NULL) {
        printf("ADIS_ERROR: Out of memory\n");
        return;
    }

    addr_mode = get_addr_mode_string(op);
    cond = get_condition_string(op);
    psr = ADIS_PSR_BIT(op) ? '^' : 0;
    wb = ADIS_WRITE_BIT(op) ? '!' : 0;

    if (ADIS_LOAD_BIT(op)) {
        printf("LDR%s%s R%d%c,%s%c\n", cond, addr_mode, ADIS_RN(op), wb,
            r_list, psr);
    } else {
        printf("STM%s%s R%d%c,%s%c\n", cond, addr_mode, ADIS_RN(op), wb,
            r_list, psr);
    }

    free(r_list);
}
