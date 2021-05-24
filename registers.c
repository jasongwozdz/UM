#include <stdio.h>
#include "registers.h"

// Initializes an Array_T of registers
Array_T initialize_regs(){
    Array_T registers = Array_new(8, sizeof(uint32_t));
    for(int i=0;i<8;i++){
        uint32_t *elem = (uint32_t*)Array_get(registers, i);
        *elem = 0;
    }
    return registers;
}

// Sets the register at the given index to value
void set_reg(Array_T registers, int index, uint32_t value){
    uint32_t *elem = (uint32_t*)Array_get(registers, index);
    *elem = value;
}

// Gets the value at the given index in the register
uint32_t get_reg(Array_T registers, int index){
    uint32_t *elem = (uint32_t*)Array_get(registers, index);
    return *elem;
}

// Prints current values of the registers
void print_registers(Array_T registers){
    printf("\nprinting regs\n");
    for(int i=0;i<8;i++){
        uint32_t *elem = (uint32_t*)Array_get(registers, i);
        printf("reg %d: %x\n", i, *elem);
    }
    printf("done\n\n");
}

// Frees the registers array
void free_registers(Array_T *registers){
    Array_free(registers);
}

