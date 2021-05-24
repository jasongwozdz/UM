#include <stdint.h>
#include <stdlib.h>
#include <hansonInterface/array.h>

Array_T initialize_regs();

void set_reg(Array_T registers, int index, uint32_t value);

uint32_t get_reg(Array_T registers, int index);

void print_registers(Array_T registers);

void free_registers(Array_T *registers);