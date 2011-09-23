#include "swi.h"

void swi_instr(uint32_t op) {

    char cond[4];

    get_condition_string(op, cond, sizeof(cond));

    printf("SWI%s 0x%X\n", cond, op & 0x00FFFFFF);
}
