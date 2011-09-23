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

