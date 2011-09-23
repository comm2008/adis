#include <stdio.h>
#include <stdint.h>

#include "predicates.h"
#include "dataproc.h"
#include "multi.h"
#include "dataswap.h"
#include "branch.h"
#include "sdt.h"

static int readop(/*out */ uint32_t * op) {

    char c;
    int i;

    for (i = 0; i < 4; i++) {
        c = getc(stdin);
        if (c == EOF) {
            return 0;
        }

        *op |= (c & 0xFF);

        if (i != 3) {
            *op <<= 8;
        }
    }

    return 1;
}

int main(int argc, char* argv[]) {

    uint32_t op;
    uint32_t count = 0;

    while (readop(&op)) {
        printf("op: 0x%.8X\n", op);
        printf("0x%.8X:\t", count);
        if (is_data_swap(op)) {
            data_swap_instr(op);
        }  else if (is_multi(op)) {
            multi_instr(op);
        } else if (is_dp_psr(op)) {
            dp_psr_instr(op);
        } else if (is_branch(op)) {
            branch_instr(op);
        } else if (is_single_data_transfer(op)) {
            sdt_instr(op);
        } else {
            printf("Unrecognized instruction 0x%x\n", op);
            return 1;
        }

        count += 4;
    }

    return 0;
}
