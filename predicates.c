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

#include "predicates.h"

int is_data_swap(uint32_t op) {
    return !((op & 0x0FB00FF0) ^ 0x01000090);
}

int is_multi(uint32_t op) {
    return !((op & 0x0FC00050) ^ 0x00000050);
}

int is_dp_psr(uint32_t op) {
    return !((op & 0x0C000000) ^ 0x00000000);
}

int is_single_data_transfer(uint32_t op) {
    return !((op & 0x0C000000) ^ 0x04000000);
}

int is_block_data_transfer(uint32_t op) {
    return !((op & 0x0E000000) ^ 0x08000000);
}

int is_branch(uint32_t op) {
    return !((op & 0x0E000000) ^ 0x0A000000);
}

int is_cp_data_transfer(uint32_t op) {
    return !((op & 0x0E000000) ^ 0x0C000000);
}

int is_cp_data_operation(uint32_t op) {
    return !((op & 0x0F000010) ^ 0x0E000000);
}

int is_cp_register_transfer(uint32_t op) {
    return !((op & 0x0F000010) ^ 0x0E000010);
}

int is_sw_interrupt(uint32_t op) {
    return !((op & 0x0F000000) ^ 0x0F000000);
}

