#include "multi.h"

static void get_destination_string(uint32_t op, char* buffer, int bsize) {
    
    int regnum = (op & 0x000F0000) >> 16;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_first_operand_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x00000F00) >> 8;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_second_operand_string(uint32_t op, char* buffer, int bsize) {

    int regnum = (op & 0x0000000F);
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

static void get_third_operand_string(uint32_t op, char* buffer, int bsize) {
    
    int regnum = (op & 0x0000F000) >> 12;
    snprintf(buffer, ADIS_MIN(bsize, sizeof("Rxx")), "R%d", regnum);
}

void multi_instr(uint32_t op) {

    char cond[4], rDest[4], rFirst[4], rSecond[4];
    char *setcond;

    get_condition_string(op, cond, sizeof(cond));
    get_destination_string(op, rDest, sizeof(rDest));
    get_first_operand_string(op, rFirst, sizeof(rFirst));
    get_second_operand_string(op, rSecond, sizeof(rSecond));

    if (op & 0x00100000) {
        setcond = "S";
    } else {
        setcond = "";
    }

    if (op & 0x00200000) {
        // multiply and accumulate
        char rThird[4];

        get_third_operand_string(op, rThird, sizeof(rThird));
        printf("MLA%s%s %s,%s,%s,%s\n", cond, setcond, rDest, 
            rFirst, rSecond, rThird);
    } else {
        printf("MUL%s%s %s,%s,%s\n", cond, setcond, rDest,
            rFirst, rSecond);
    }
}
