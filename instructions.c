#include <stdio.h>
#include "instructions.h"

void instruction_CMV(int a, int b, int c, Array_T registers){
    if(get_reg(registers, c) != 0){
        set_reg(registers, a, get_reg(registers, b));
    }
}

// Segmented load
void instruction_SLOAD(int a, int b, int c, Array_T registers, Segment_T segmentation){
    uint32_t value = get_word(segmentation, get_reg(registers, b), get_reg(registers, c));
    // printf("value: %x\n", value);
    set_reg(registers, a, value);
}

// Segmented Load
void instructions_SSTORE(int a, int b, int c, Array_T registers, Segment_T segmentation){
    set_word(segmentation, get_reg(registers, a), get_reg(registers, b), get_reg(registers, c));
}

// Adds two registers together
void instructions_ADD(int a, int b, int c, Array_T registers){
    uint32_t add = get_reg(registers, b) + get_reg(registers, c);
    set_reg(registers, a, add);
}

// Multiplies two registers together
void instructions_MULT(int a, int b, int c, Array_T registers){
    uint32_t mult = get_reg(registers, b) * get_reg(registers, c);
    set_reg(registers, a, mult);
}

// Divides register b by register c and stores in a
void instructions_DIV(int a, int b, int c, Array_T registers){
    uint32_t div = get_reg(registers, b) / get_reg(registers, c);
    set_reg(registers, a, div);
}

// Bitwise NAND of b and c stored in a
void instructions_NAND(int a, int b, int c, Array_T registers){
    int32_t reg1 = get_reg(registers, b);
    int32_t reg2 = get_reg(registers, c);
    int32_t nand = ~(reg1 & reg2);
    set_reg(registers, a, nand);
}

// Halts with exit zero
void instructions_HALT(){
    exit(0);
}

// Maps segment to values of C
void instructions_MAP(int b, int c, Array_T registers, Segment_T segmentation){
    set_reg(registers, b, map_segment(segmentation, get_reg(registers, c)));
}

// Unmaps segment c
void instructions_UMAP(int c, Array_T registers, Segment_T segmentation){
    unmap_segment(segmentation, get_reg(registers, c));
}

// Prints the value of register c
void instructions_OUT(int c, Array_T registers){
    printf("%c", (char)get_reg(registers, c));
}

// Takes input from I/O and stores it in register c
void instructions_IN(int c, Array_T registers){
    uint32_t input =  getchar();
    if((int)input == EOF){
        input = ~0;
    }
    set_reg(registers, c, input);
}

// Copys a segment at index and then moves the program counter to index
uint32_t instructions_LOADP(int b, int c, Array_T registers, Segment_T segmentation){
    if(get_reg(registers, b) != 0){
        copy_segment(segmentation, get_reg(registers, b));
    }
    return get_reg(registers, c);
}

// Sets the value of a register
void instructions_LOADV(int reg, int value, Array_T registers){
    set_reg(registers, reg, value);
}

