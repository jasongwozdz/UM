#include <stdint.h>
#include <stdlib.h>
#include "bitpack.h"
#include "registers.h"
#include "segmentation.h"

// enum ops {CMV = 0, SLOAD, SSTORE, ADD, MULT, DIVIDE, NAND, HALT, MAP, UMAP, OUT, IN, LOADP, LOADV};

// void read_instruction(uint32_t instruction, Array_T registers);

void instruction_CMV(int a, int b, int c, Array_T registers);

void instruction_SLOAD(int a, int b, int c, Array_T registers, Segment_T segmentation);

void instructions_SSTORE(int a, int b, int c, Array_T registers, Segment_T segmentation);

void instructions_ADD(int a, int b, int c, Array_T registers);

void instructions_MULT(int a, int b, int c, Array_T registers);

void instructions_DIV(int a, int b, int c, Array_T registers);

void instructions_NAND(int a, int b, int c, Array_T registers);

void instructions_HALT();

void instructions_MAP(int b, int c, Array_T registers, Segment_T segmentation);

void instructions_UMAP(int c, Array_T registers, Segment_T segmentation);

void instructions_OUT(int c, Array_T registers);

void instructions_IN(int c, Array_T registers);

uint32_t instructions_LOADP(int b, int c, Array_T registers, Segment_T segmentation);

void instructions_LOADV(int reg, int value, Array_T registers);