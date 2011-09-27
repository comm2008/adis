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

#include "common.h"

void get_condition_string(uint32_t op, char* buffer, int bsize) {

    char* tmp;
    int cond = (op & 0xF0000000) >> 28;

    switch(cond) {
        case 0x00:
            // equal
            tmp = "EQ";
            break;
        case 0x01:
            // not equal
            tmp = "NE";
            break;
        case 0x02:
            // unsigned higher or same
            tmp = "CS";
            break;
        case 0x03:
            // unsigned lower
            tmp = "CC";
            break;
        case 0x04:
            // negative
            tmp = "MI";
            break;
        case 0x05:
            // positive or zero
            tmp = "PL";
            break;
        case 0x06:
            // overflow
            tmp = "VS";
            break;
        case 0x07:
            // no overflow
            tmp = "VC";
            break;
        case 0x08:
            // unsigned higher
            tmp = "HI";
            break;
        case 0x09:
            // unsigned lower or same
            tmp = "LS";
            break;
        case 0x0A:
            // greater or equal
            tmp = "GE";
            break;
        case 0x0B:
            // less than
            tmp = "LT";
            break;
        case 0x0C:
            // greater than
            tmp = "GT";
            break;
        case 0x0D:
            // less than or equal
            tmp = "LE";
            break;
        case 0x0E:
            // always
            tmp = "AL";
            break;
        case 0x0F:
            // never
            tmp = "NV";
            break;
    }

    memcpy(buffer, tmp, ADIS_MIN(bsize, sizeof(tmp))); 
}

void get_shift_string(uint32_t shift, char* buffer, int bsize) {

    char* sType;

    if (shift & 0x01) {
        // shifted by amount in register
        int sReg = shift & 0xE0 >> 4;
        switch (shift & 0x06 >> 1) {
            case 0x00:
                // logical left
                sType = "LSL";
                break;
            case 0x01:
                // logical right
                sType = "LSR";
                break;
            case 0x02:
                // arithmetic right
                sType = "ASR";
                break;
            case 0x03:
                sType = "ROR";
                break;
        }
        snprintf(buffer, ADIS_MIN(bsize, sizeof("XXX Rxx")),
            "%s R%d", sType, sReg);
    } else {
        int imm = shift & 0xF0 >> 3;
        if (imm != 0) {
            switch (shift & 0x06 >> 1) {
                case 0x00:
                    // logical left
                    sType = "LSL";
                    break;
                case 0x01:
                    // logical right
                    sType = "LSR";
                    break;
                case 0x02:
                    // arithmetic right
                    sType = "ASR";
                    break;
                case 0x03:
                    // rotate right
                    sType = "ROR";
                    break;
            }
            snprintf(buffer, ADIS_MIN(bsize, sizeof("XXX #xx")),
                "%s #%d", sType, imm);
        } else if (bsize > 0) {
            buffer[0] = 0;
        }
    }
}           
