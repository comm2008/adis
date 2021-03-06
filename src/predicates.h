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

#ifndef __ADIS_PREDICATES_H__
#define __ADIS_PREDICATES_H__

#include <stdint.h>

// synchronization primitive (SWP, STREX, LDRX)
__attribute__((always_inline)) static inline int is_sync_primitive(uint32_t op)
{
    return !((op & 0x0F0000F0) ^ 0x01000090);
}

// multiply and multiply-accumulate operation
__attribute__((always_inline)) static inline int is_multi(uint32_t op)
{
    return !((op & 0x0F0000F0) ^ 0x00000050);
}

// halfword multiply and multiply-accumulate operations
__attribute__((always_inline)) static inline int is_halfword_multi(uint32_t op)
{
    return !((op & 0x0F900090) ^ 0x01000080);
}

// data-processing (register)
__attribute__((always_inline)) static inline int is_dp_reg(uint32_t op)
{
    return (!((op & 0x02000010) ^ 0x00000000)) &&
           ((op & 0x01900000) ^ 0x01000000);
}

// data-processing (register-shifted register)
__attribute__((always_inline)) static inline int is_dp_rsr(uint32_t op)
{
    return (!((op & 0x02000090) ^ 0x00000010)) &&
           ((op & 0x01900000) ^ 0x01000000);
}

// data-processing (immediate)
__attribute__((always_inline)) static inline int is_dp_imm(uint32_t op)
{
    return (!((op & 0x0200000) ^ 0x02000000)) &&
           ((op & 0x01900000) ^ 0x01000000);
}

// data-processing (other - MOVW and MOVT)
__attribute__((always_inline)) static inline int is_dp_other(uint32_t op)
{
    return !((op & 0x0FB00000) ^ 0x01000000);
}

// misc instructions
__attribute__((always_inline)) static inline int is_misc(uint32_t op)
{
    return !((op & 0x0F900080) ^ 0x010000000);
}

// single data transfer
__attribute__((always_inline)) static inline int is_dt_single(uint32_t op)
{
    return !((op & 0x0C000000) ^ 0x04000000);
}

// block data transfer
__attribute__((always_inline)) static inline int is_dt_block(uint32_t op)
{
    return !((op & 0x0E000000) ^ 0x08000000);
}

// halfword, signed, and dual data transfers (including unprivileged)
__attribute__((always_inline)) static inline int is_dt_extra(uint32_t op)
{
    return !((op & 0x0E0000F0) ^ 0x000000B0) ||
           !((op & 0x0E0000D0) ^ 0x000000D0);
}

// branch
__attribute__((always_inline)) static inline int is_branch(uint32_t op)
{
    return !((op & 0x0E000000) ^ 0x0A000000);
}

// coproc data transfer
__attribute__((always_inline)) static inline int is_dt_coproc(uint32_t op)
{
    return !((op & 0x0E000000) ^ 0x0C000000);
}

// coproc data operation
__attribute__((always_inline)) static inline int is_dataop_coproc(uint32_t op)
{
    return !((op & 0x0F000010) ^ 0x0E000000);
}

// coproc register transfer
__attribute__((always_inline)) static inline int is_rt_coproc(uint32_t op)
{
    return !((op & 0x0F000010) ^ 0x0E000010);
}

// software interrupt
__attribute__((always_inline)) static inline int is_sw_interrupt(uint32_t op)
{
    return !((op & 0x0F000000) ^ 0x0F000000);
}

#endif  // __ADIS_PREDICATES_H__
