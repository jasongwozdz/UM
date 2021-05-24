#include <stdio.h>
#include <stdbool.h>
#include "segmentation.h"

#define T Segment_T

// Struct for holding the unmapped ids and the mapped ids
struct T
{
    Array_T unmapped_ids;
    Seq_T segment;
};

// Initialize the Segment_T
T initialize_segment(Seq_T instructions, int num_instructions){
    (void)num_instructions;
    Seq_T main_segment = Seq_new(num_instructions);
    Seq_addlo(main_segment, instructions);
    Array_T unmapped_ids = Array_new(0, sizeof(uint32_t));
    T segment = malloc(sizeof(struct T));
    segment->segment = main_segment;
    segment->unmapped_ids = unmapped_ids;
    return segment;
}

// Maps a segment and initializes it to all zeros
int map_segment(T segment, int size){   
    if(Array_length(segment->unmapped_ids) == 0){
        uint32_t offset = Seq_length(segment->segment);
        Seq_T i_segment = Seq_new(size);
        for(int i=0;i<size;i++){
            Seq_addlo(i_segment, 0);
        }
        Seq_addhi(segment->segment, i_segment);
        return offset;
    }else{
        uint32_t *elem = (uint32_t*)Array_get(segment->unmapped_ids, Array_length(segment->unmapped_ids)-1);
        uint32_t offset = *elem;
        Array_resize(segment->unmapped_ids, Array_length(segment->unmapped_ids)-1);
        Seq_T i_segment = Seq_new(size);
        for(int i=0;i<size;i++){
            Seq_addlo(i_segment, 0);
        }
        Seq_put(segment->segment,offset,i_segment);
        return offset;
    }
}

// Unmaps the segment and adds its id to unmapped_ids
void unmap_segment(T segment, int offset){
    if(Array_length(segment->unmapped_ids) == 0){
        Array_resize(segment->unmapped_ids, Array_length(segment->unmapped_ids)+1);
        uint32_t *elem = (uint32_t*)Array_get(segment->unmapped_ids, 0);
        *elem = offset;
        Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, offset);
        Seq_free(&curr_segment);
        Seq_put(segment->segment, offset, NULL);
    }else if(Seq_length(segment->segment) >= offset){
        Array_resize(segment->unmapped_ids, Array_length(segment->unmapped_ids)+1);
         uint32_t *elem = (uint32_t*)Array_get(segment->unmapped_ids, Array_length(segment->unmapped_ids)-1);
        *elem = offset;
        Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, offset);
        Seq_free(&curr_segment);
    }else{
        printf("offset out of segment range\n");
    }
}

// Copys the segment at offset and then deletes the first segment and replaces it with the copied segment
void copy_segment(T segment, int offset){
    Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, offset);
    Seq_T seg_copy = Seq_new(Seq_length(segment->segment));
    for(int i=0;i<Seq_length(curr_segment);i++){
        Seq_addhi(seg_copy,(void*)(uintptr_t)Seq_get(curr_segment, i));
    }
    Seq_remlo(segment->segment);
    Seq_addlo(segment->segment, seg_copy);
}

// Gets the word from a segment at the index given
uint32_t get_word(T segment, int index, int offset){
    Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, index);
    return((uint32_t)(uintptr_t)Seq_get(curr_segment, offset));
}

// Sets the word at index to value
void set_word(T segment, int index, int offset, uint32_t value){
    Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, index);
    uintptr_t elem = (uintptr_t)value;
    Seq_put(curr_segment, offset, (void*)elem);
}

// Finds the num in the array. Returns true if found, false otherwise
bool array_find(Array_T arr, int num){
    for(int i=0;i<Array_length(arr);i++){
        int *elem = (int*)Array_get(arr, i);
        if(*elem == num){
            return true;
        }
    }
    return false;
}

// Prints the current words
void print_words(T segment){
    printf("printing words\n");
    Seq_T curr_segment;
    for(int i=0;i<Seq_length(segment->segment);i++){
        curr_segment = (Seq_T)Seq_get(segment->segment, i);
        // if(Array_length(unmapped_ids) > 1 && array_find(unmapped_ids, i)){
        //     continue;
        // }
        printf("segment: %d\n", i);
        for(int j=0;j<Seq_length(curr_segment);j++){
            printf("%x\n", ((uint32_t)(uintptr_t)Seq_get(curr_segment, j)));
        }
        printf("\n");
    }
    printf("done \n\n");
}

// Prints the current unmapped ids
void print_unmapped(T segment){
    printf("printing unmapped ids\n");
    if(segment->unmapped_ids != NULL){
        for(int i=0;i<Array_length(segment->unmapped_ids);i++){
            int *elem = (int*)Array_get(segment->unmapped_ids, i);
            printf("%d\n", *elem);
        }
    }
    printf("done\n\n");
}

// Returns the length of the given segment
int segment_length(T segment){
    Seq_T curr_segment = (Seq_T)Seq_get(segment->segment, 0);
    return Seq_length(curr_segment);
}

// Frees the memory of the given segment
void free_segment(T *segment){
    for(int i=0;i<Seq_length((*segment)->segment);i++){
        if(array_find((*segment)->unmapped_ids, i)){
            continue;
        }else{
            Seq_T curr_segment = (Seq_T)Seq_get((*segment)->segment, i);
            Seq_free(&curr_segment);
        }
    }
    Seq_free(&(*segment)->segment);
    Array_free(&(*segment)->unmapped_ids);
    free(*segment);
}