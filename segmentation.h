#include <stdint.h>
#include <stdlib.h>
#include <hansonInterface/seq.h>
#include <hansonInterface/array.h>
#define T Segment_T

typedef struct T *T;

//allocates memory for instructions and creates a struct with both 
T initialize_segment(Seq_T instructions, int num_instructions);

int map_segment(T segment, int size);

void unmap_segment(T segment, int offset);

void copy_segment(T segment, int offset);

uint32_t get_word(T segment, int index, int offset);

void set_word(T segment, int index, int offset, uint32_t value);

void print_words(T segment);

void print_unmapped(T segment);

int segment_length(T segment);

void free_segment(T *segment);

#undef T