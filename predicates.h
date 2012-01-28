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

// data swap operation
int is_data_swap(uint32_t op);

// multiplication operation
int is_multi(uint32_t op);

// data processing or psr transfer
int is_dp_psr(uint32_t op);

// single data transfer
int is_dt_single(uint32_t op);

// block data transfer
int is_dt_block(uint32_t op);

// branch
int is_branch(uint32_t op);

// coproc data transfer
int is_dt_coproc(uint32_t op);

// coproc data operation
int is_dataop_coproc(uint32_t op);

// coproc register transfer
int is_rt_coproc(uint32_t op);

// software interrupt
int is_sw_interrupt(uint32_t op);

#endif  // __ADIS_PREDICATES_H__
