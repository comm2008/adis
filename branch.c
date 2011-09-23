#include "branch.h"

void branch_instr(uint32_t op) {

    char cond[4];

    get_condition_string(op, cond, sizeof(cond));

    if (op & 0x01000000) {
        printf("BL%s 0x%.8X\n", cond, op & 0x00FFFFFF);
    } else {
        printf("B%s 0x%.8X\n", cond, op & 0x00FFFFFF);
    }
}
