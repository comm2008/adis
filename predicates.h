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
int is_single_data_transfer(uint32_t op);

// block data transfer
int is_block_data_transfer(uint32_t op);

// branch
int is_branch(uint32_t op);

// coproc data transfer
int is_cp_data_transfer(uint32_t op);

// coproc data operation
int is_cp_data_operation(uint32_t op);

// coproc register transfer
int is_cp_register_transfer(uint32_t op);

// software interrupt
int is_sw_interrupt(uint32_t op);

#endif  // __ADIS_PREDICATES_H__
