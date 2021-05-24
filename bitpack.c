#include "bitpack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


Except_T Bitpack_Overflow = { "Overflow packing bits" };

static inline uint64_t left_shift(uint64_t word, unsigned offset){
    assert(offset <= 64);
    uint64_t output;
    if(offset==63){
        output = 0;
    }else{
        output = (word << offset);
    }
    return output;
} 

static inline int64_t arithmetic_right_shift(int64_t word, unsigned offset){
    assert(offset <= 64);
    int64_t output;
    if(offset==64){
        output = 0;
    }else{
        output = word >> offset;
    }
    return output;
} 

static inline uint64_t logical_right_shift(uint64_t word, unsigned offset){
    assert(offset <= 64);
    uint64_t output;
    if(offset==64){
        output = 0;
    }else{
        output = word >> offset;
    }
    return output;
}

bool Bitpack_fitsu(uint64_t n, unsigned width){
    assert(width <= 64);
    uint64_t max;
    max = left_shift(1, width)-1;
    if (n <= max){
        return true;
    }else{
        return false;
    } 
}

bool Bitpack_fitss(int64_t n, unsigned width){
    assert(width <= 64);
    int64_t max;
    int64_t min;
    max = (int64_t)left_shift(1, width - 1)-1;
    min = ~max;

    if (min <= n && n <= max){
        return true;
    }else{
        return false;
    }
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
    assert(width <= 64);
    assert(width + lsb <= 64);
    word = left_shift(word, 64-(lsb+width));
    word = logical_right_shift(word, 64-(width));
    return word;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){
    assert(width <= 64);
    assert(width + lsb <= 64);
    word = left_shift(word, 64-(lsb+width));
    word = arithmetic_right_shift(word, 64-(width));
    return (int64_t)word;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value){
    assert(width <= 64);
    assert(width + lsb <= 64);
    if(Bitpack_fitsu(value, width) == false){
        RAISE(Bitpack_Overflow);
    }
    uint64_t left;
    uint64_t right;
    right = logical_right_shift(left_shift(word, 64-lsb), 64-lsb);
    left = left_shift(logical_right_shift(word, lsb+width), lsb+width);
    value = left_shift(value, lsb);
    word = (left | value | right);
    return(word);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value){
    assert(width <= 64);
    assert(width + lsb <= 64);
    if(Bitpack_fitss(value, width) == false){
        RAISE(Bitpack_Overflow);
    }
    uint64_t left;
    uint64_t right;
    right = logical_right_shift(left_shift(word, 64-lsb), 64-lsb);
    left = left_shift(logical_right_shift(word, lsb+width), lsb+width);
    value = left_shift(value, lsb);
    word = (left | value | right);
    return(word);
}

