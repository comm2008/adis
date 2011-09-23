#include "dataswap.h"

static void get_destination_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_source_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x0000000F);
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_base_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

void data_swap_instr(uint32_t op) {

    char cond[4], rDest[4], rSrc[4], rBase[4];

    get_condition_string(op, cond, sizeof(cond));
    get_destination_string(op, cond, sizeof(cond));
    get_source_string(op, cond, sizeof(cond));
    get_base_string(op, cond, sizeof(rBase));

    if (op & 0x00400000) {
        // swap byte
        printf("SWP%sB %s,%s,[%s]\n", cond, rDest, rSrc, rBase);
    } else {
        // swap word
        printf("SWP%s %s,%s,[%s]\n", cond, rDest, rSrc, rBase);
    }
}

