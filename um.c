#include <stdio.h>
#include <stdint.h>
#include "instructions.h"
#include "registers.h"

enum ops {CMV = 0, SLOAD, SSTORE, ADD, MULT, DIVIDE, NAND, HALT, MAP, UMAP, OUT, IN, LOADP, LOADV};

int program_counter;

bool running;

// Returns an array of 3 register indexs
uint32_t* get_three_regs(uint32_t instruction){
    uint32_t C = Bitpack_getu(instruction, 3, 0);
    uint32_t B = Bitpack_getu(instruction, 3, 3);
    uint32_t A = Bitpack_getu(instruction, 3, 6);
    uint32_t *r_array = malloc(3*sizeof(uint32_t));
    r_array[0] = C;
    r_array[1] = B;
    r_array[2] = A;
    return r_array;
}

// Returns an array of 2 register indexs required for an instruction
uint32_t* get_two_regs(uint32_t instruction){
    uint32_t C = Bitpack_getu(instruction, 3, 0);
    uint32_t B = Bitpack_getu(instruction, 3, 3);
    uint32_t *r_array = malloc(2*sizeof(uint32_t));
    r_array[0] = C;
    r_array[1] = B;
    return r_array;
}

// Return register index and value for loadv instruction
uint32_t* get_load_reg(uint32_t instruction){
    uint32_t reg = Bitpack_getu(instruction, 3, 25);
    uint32_t value = Bitpack_getu(instruction, 25, 0);
    uint32_t *r_array = malloc(2*sizeof(uint32_t));
    r_array[0] = reg;
    r_array[1] = value;
    return r_array;
}

// Returns an array of 1 register index required for an instruction
uint32_t* get_one_reg(uint32_t instruction){
    uint32_t C = Bitpack_getu(instruction, 3, 0);
    uint32_t *r_array = malloc(1*sizeof(uint32_t));
    r_array[0] = C;
    return r_array;
}

// Reads each instruction and runs the required instruction function
void read_instruction(uint32_t instruction, Array_T registers, Segment_T segmentation){
    uint32_t opcode = (uint32_t)Bitpack_getu(instruction, 4, 28);
    uint32_t *r_array;
    switch(opcode){
        case CMV:
            r_array = get_three_regs(instruction);
            instruction_CMV(r_array[2], r_array[1], r_array[0], registers);
            free(r_array);
            break;
        case SLOAD:
            r_array = get_three_regs(instruction);
            instruction_SLOAD(r_array[2],  r_array[1], r_array[0], registers, segmentation);
            free(r_array);
            break;
        case SSTORE:
            r_array = get_three_regs(instruction);
            instructions_SSTORE(r_array[2], r_array[1], r_array[0], registers, segmentation);
            free(r_array);
            break;      
        case ADD: 
            r_array = get_three_regs(instruction);
            instructions_ADD(r_array[2], r_array[1], r_array[0], registers);
            free(r_array);
            break;     
        case MULT:       
            r_array = get_three_regs(instruction);
            instructions_MULT(r_array[2], r_array[1], r_array[0], registers);
            free(r_array);
            break;
        case DIVIDE:        
            r_array = get_three_regs(instruction);
            instructions_DIV(r_array[2], r_array[1], r_array[0], registers);
            free(r_array);
            break;
        case NAND:       
            r_array = get_three_regs(instruction);
             instructions_NAND(r_array[2], r_array[1], r_array[0], registers);
            free(r_array);
            break;
        case HALT:
            running = false;
            break;
        case MAP:      
            r_array = get_two_regs(instruction);
            instructions_MAP(r_array[1], r_array[0], registers, segmentation);
            free(r_array);
            break;
        case UMAP:    
            r_array = get_one_reg(instruction);
            instructions_UMAP(r_array[0], registers, segmentation);
            free(r_array);
            break;
        case OUT:
            r_array = get_one_reg(instruction);
            instructions_OUT(r_array[0], registers);
            free(r_array);
            break;
        case IN:
            r_array = get_one_reg(instruction);
            instructions_IN(r_array[0], registers);
            free(r_array);
            break;
        case LOADP:
            r_array = get_two_regs(instruction);
            program_counter = instructions_LOADP(r_array[1], r_array[0], registers, segmentation)-1;
            free(r_array);
            break;
        case LOADV:
            r_array = get_load_reg(instruction);
            instructions_LOADV(r_array[0], r_array[1], registers);
            free(r_array);
            break;
    }
}

// Reads the file and returns the Seq_T of instructions
Seq_T read_file(FILE *file){
    Seq_T instructions = Seq_new(0);
    int i=0;
    uint32_t byte = 0;
    uint32_t codeword = 0;
    while((int)byte != EOF){
        byte = getc(file);
        if((int)byte == EOF){
            break;
        }
        codeword = Bitpack_newu(codeword, 8, 24-((i%4) * 8), byte);
        i++;
        if(i%4 == 0){
            Seq_addhi(instructions, (void*)(uintptr_t)codeword);
            codeword = 0;
        }
    }
    return instructions;
}

// Goes through the whole programs and runs each instruction
void run_program(Seq_T instructions){
    program_counter = -1;
    int length = Seq_length(instructions);
    Segment_T segments = initialize_segment(instructions, length);
    Array_T registers = initialize_regs();
    running = true;
    while(running){
        program_counter++;
        read_instruction(get_word(segments,0, program_counter), registers, segments);
    }
    free_registers(&registers);
    free_segment(&segments);
}

// Takes the input file and runs the program functions
int main(int argc,char** argv){
    (void)argc;
    (void)argv;
    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        printf("invalid file\n");
        return 0;
    }else{
        Seq_T instructions = read_file(fp);
        fclose(fp);
        run_program(instructions);
    }
}


